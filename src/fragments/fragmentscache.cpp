/*
 *   This file is part of QRail.
 *
 *   QRail is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   QRail is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with QRail.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "fragments/fragmentscache.h"

using namespace QRail;
using namespace Fragments;
Cache::Cache(QObject *parent) : QObject(parent)
{
    // Disk cache directory creation
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/fragments";

    // Create the 'fragments' folder to save our caching data
    QDir m_cacheDir = QDir(path);
    m_cacheDir.mkpath(path);
}

void Cache::cachePage(Page *page)
{
    // Add the page to the LRU cache and return true if success
    qDebug() << "Inserted page:" << page->uri();
    m_cache.insert(page->uri(), page);
    qDebug() << "Number of entries in cache:" << m_cache.count();

    // Cache the page on disk
    QJsonObject obj;
    obj.insert("uri", QJsonValue::fromVariant(page->uri().toString()));
    obj.insert("timestamp", QJsonValue::fromVariant(page->timestamp().toString(Qt::ISODate)));
    obj.insert("hydraPrevious", QJsonValue::fromVariant(page->hydraPrevious()).toString());
    obj.insert("hydraNext", QJsonValue::fromVariant(page->hydraNext().toString()));
    QJsonArray fragments;
    foreach(QRail::Fragments::Fragment *frag, page->fragments()) {
        QJsonObject f;
        f.insert("uri", QJsonValue::fromVariant(frag->uri().toString()));
        f.insert("departureStationURI", QJsonValue::fromVariant(frag->departureStationURI().toString()));
        f.insert("arrivalStationURI", QJsonValue::fromVariant(frag->arrivalStationURI().toString()));
        f.insert("departureTime", QJsonValue::fromVariant(frag->departureTime().toString(Qt::ISODate)));
        f.insert("arrivalTime", QJsonValue::fromVariant(frag->arrivalTime().toString(Qt::ISODate)));
        f.insert("departureDelay", QJsonValue::fromVariant(frag->departureDelay()));
        f.insert("arrivalDelay", QJsonValue::fromVariant(frag->arrivalDelay()));
        f.insert("tripURI", QJsonValue::fromVariant(frag->tripURI().toString()));
        f.insert("routeURI", QJsonValue::fromVariant(frag->routeURI().toString()));
        f.insert("direction", QJsonValue::fromVariant(frag->direction()));
        f.insert("pickupType", this->convertGTFSTypeToJson(frag->pickupType()));
        f.insert("dropOffType", this->convertGTFSTypeToJson(frag->dropOffType()));
        fragments.append(f);
    }
    obj.insert("fragments", fragments);
    QJsonDocument doc = QJsonDocument(obj);

    // Save QJsonDocument to disk
    QString path = m_cacheDir.absolutePath();
    path.append("/" + page->uri().toString());
    QDir jsonFileDir(path);
    jsonFileDir.mkpath(path);
    qDebug() << "PAGE file path:" << path;

    path.append(PAGE_FILE_NAME);
    QFile jsonFile(path);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
}

void Cache::updateFragment(Fragment *updatedFragment)
{
    // We look between the departureTime and departureTime + departureDelay for the old fragment
    QDateTime departureTime = updatedFragment->departureTime().addSecs(-updatedFragment->departureDelay());
    QDateTime departureTimeWithDelay = updatedFragment->departureTime();
    QList<QUrl> pagesURI = m_cache.keys();

    foreach(QUrl pageURI, pagesURI) {
        QUrlQuery query = QUrlQuery(pageURI);
        QDateTime pageTime = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
        qDebug() << "Page time:" << pageTime;
        if(pageTime >= departureTime && pageTime < departureTimeWithDelay) {
            QRail::Fragments::Page* page = m_cache.value(pageURI);
            QList<QRail::Fragments::Fragment *> fragments = page->fragments();

            // Check if the connection is in the fragments of this page
            for(qint32 fragCounter=0; fragCounter < fragments.length(); fragCounter++) {
                QRail::Fragments::Fragment *fragment = fragments.at(fragCounter);

                // Check if this fragment is the one we're looking for
                if(fragment->uri() == updatedFragment->uri()) {
                    qDebug() << "Found fragment!";

                    // Departure delay changed, removing old fragment and inserting new fragment
                    if(fragment->departureDelay() != updatedFragment->departureDelay()) {
                        qDebug() << "Deleting old fragment, inserting new one";
                        // Remove old fragment in the old page
                        page->fragments().removeAt(fragCounter);

                        // Insert updated fragment in the new page
                        for(qint32 pageCounter=0; pageCounter < pagesURI.length()-1; pageCounter++) {
                            QUrl currentPageURI = pagesURI.at(pageCounter);
                            QUrl nextPageURI = pagesURI.at(pageCounter);
                            QUrlQuery currentPageQuery = QUrlQuery(currentPageURI);
                            QUrlQuery nextPageQuery = QUrlQuery(nextPageURI);
                            QDateTime currentPageTime = QDateTime::fromString(currentPageQuery.queryItemValue("departureTime"), Qt::ISODate);
                            QDateTime nextPageTime = QDateTime::fromString(nextPageQuery.queryItemValue("departureTime"), Qt::ISODate);

                            // Look for the new page
                            if(currentPageTime <= departureTime && departureTime < nextPageTime) {
                                qDebug() << "Found inserting page, inserting fragment now";
                                QRail::Fragments::Page* currentPage = m_cache.value(currentPageURI);
                                currentPage->fragments().append(updatedFragment);

                                // Keep page sorted
                                std::sort(currentPage->fragments().begin(), currentPage->fragments().end(), [](const QRail::Fragments::Fragment * a,
                                const QRail::Fragments::Fragment * b) -> bool {
                                    QDateTime timeA = a->departureTime();
                                    QDateTime timeB = b->departureTime();
                                    return timeA < timeB;
                                });
                            }
                        }

                        // Update completed
                        return;
                    }

                    // Arrival delay changed or cancelled (type changed), updating fragment in page
                    if(fragment->arrivalDelay() != updatedFragment->arrivalDelay()) {
                        qDebug() << "Updating old fragment";
                        page->fragments().replace(fragCounter, updatedFragment);

                        // Update completed
                        return;
                    }
                }
            }
        }
    }
}

Page *Cache::getPageByURI(QUrl uri)
{
    // Try to get the page from the RAM cache
    if(m_cache.contains(uri)) {
        //qDebug() << "Memory cache" << m_cache.value(uri);
        return m_cache.value(uri);
    }

    // If the requested page isn't cached, the Fragments::Factory will fetch it from the network
    return this->getPageFromDisk(uri);
}

Page *Cache::getPageByFragment(Fragment *fragment)
{
    foreach(QRail::Fragments::Page *page, m_cache.values()) {
        if(fragment->departureTime() < page->timestamp()) {
            continue;
        }
        else {
            return page;
        }
    }

    // In case we can't a page
    qCritical() << "Unable to find page for fragment:" << fragment->uri().toString();
    return nullptr;
}

bool Cache::hasPage(QUrl uri)
{
    return m_cache.contains(uri);
}

bool Cache::isEmpty()
{
    return m_cache.count() == 0;
}

Page *Cache::getPageFromDisk(QUrl uri)
{
    // The page can be available on disk, but not in the RAM cache
    QString path = m_cacheDir.absolutePath();
    path.append("/" + uri.toString());
    path.append(PAGE_FILE_NAME);
    //qDebug() << "PAGE file path:" << path;

    if(QFileInfo::exists(path)) {
        // Read page from disk
        //qDebug() << "Page found in disk cache";
        QFile jsonFile;
        jsonFile.setFileName(path);
        jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString data = jsonFile.readAll();
        jsonFile.close();
        QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject obj = d.object();

        // Convert QJsonObject to QRail::Fragments::Page *
        QRail::Fragments::Page *page = new QRail::Fragments::Page();
        page->setURI(QUrl(obj["uri"].toString()));
        page->setHydraNext(QUrl(obj["hydraNext"].toString()));
        page->setHydraPrevious(QUrl(obj["hydraPrevious"].toString()));
        page->setTimestamp(QDateTime::fromString(obj["timestamp"].toString(), Qt::ISODate));
        QJsonArray fragmentsJson = obj["fragments"].toArray();
        QList<QRail::Fragments::Fragment *> fragments;
        foreach(QJsonValue item, fragmentsJson) {
            QJsonObject frag = item.toObject();
            QRail::Fragments::Fragment *fragment = new QRail::Fragments::Fragment();
            fragment->setURI(QUrl(frag["uri"].toString()));
            fragment->setDepartureStationURI(QUrl(frag["departureStationURI"].toString()));
            fragment->setArrivalStationURI(QUrl(frag["arrivalStationURI"].toString()));
            fragment->setDepartureTime(QDateTime::fromString(frag["departureTime"].toString(), Qt::ISODate));
            fragment->setArrivalTime(QDateTime::fromString(frag["arrivalTime"].toString(), Qt::ISODate));
            fragment->setDepartureDelay(frag["departureDelay"].toInt());
            fragment->setArrivalDelay(frag["arrivalDelay"].toInt());
            fragment->setTripURI(QUrl(frag["tripURI"].toString()));
            fragment->setRouteURI(QUrl(frag["routeURI"].toString()));
            fragment->setDirection(frag["direction"].toString());
            fragment->setPickupType(this->convertJsonToGTFSType(frag["pickupType"]));
            fragment->setDropOffType(this->convertJsonToGTFSType(frag["dropOffType"]));

            // Add fragment to list
            fragments.append(fragment);
        }
        page->setFragments(fragments);

        // Insert page in memory cache and return it
        m_cache.insert(page->uri(), page);
        return page;
    }

    // No page available, return NULL pointer
    qWarning() << "Cannot find page in disk cache";
    return nullptr;
}

QJsonValue Cache::convertGTFSTypeToJson(Fragment::GTFSTypes type)
{
    if(type == Fragment::GTFSTypes::REGULAR) {
        return QJsonValue::fromVariant(QString("gtfs:Regular"));
    }
    else if(type == Fragment::GTFSTypes::NOTAVAILABLE) {
        return QJsonValue::fromVariant(QString("gtfs:NotAvailable"));
    }
    else if(type == Fragment::GTFSTypes::MUSTPHONE) {
        return QJsonValue::fromVariant(QString("gtfs:MustPhone"));
    }
    else if(type == Fragment::GTFSTypes::MUSTCOORDINATEWITHDRIVER) {
        return QJsonValue::fromVariant(QString("gtfs:MustCoordinateWithDriver"));
    }

    return QJsonValue::fromVariant(QString("gtfs:Unknown"));
}

Fragment::GTFSTypes Cache::convertJsonToGTFSType(QJsonValue type)
{
    QString t = type.toString();
    if(t == QString("gtfs:Regular")) {
        return QRail::Fragments::Fragment::GTFSTypes::REGULAR;
    }
    else if(t == QString("gtfs:NotAvailable")) {
        return QRail::Fragments::Fragment::GTFSTypes::NOTAVAILABLE;
    }
    else if(t == QString("gtfs:MustPhone")) {
        return QRail::Fragments::Fragment::GTFSTypes::MUSTPHONE;
    }
    else if(t == QString("gtfs:MustCoordinateWithDriver")) {
        return QRail::Fragments::Fragment::GTFSTypes::MUSTCOORDINATEWITHDRIVER;
    }

    return QRail::Fragments::Fragment::GTFSTypes::UNKNOWN;
}
