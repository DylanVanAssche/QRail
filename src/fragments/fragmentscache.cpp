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
    qDebug() << "Fragments:";
    qDebug() << page->fragments();
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
    qDebug() << "Fragment updated";

    // Save QJsonDocument to disk
    QString path = m_cacheDir.absolutePath();
    path.append("/" + page->uri().toString());
    QDir jsonFileDir(path);
    jsonFileDir.mkpath(path);
    qDebug() << "Fragment opened as:" << path;

    path.append(PAGE_FILE_NAME);
    QFile jsonFile(path);
    jsonFile.open(QFile::WriteOnly);
    jsonFile.write(doc.toJson());
    jsonFile.close();
    qDebug() << "Fragment written as:" << path;
}

QUrl Cache::updateFragment(Fragment *updatedFragment)
{
    qDebug() << "Updating fragment";
    // We look between the departureTime and departureTime + departureDelay for the old fragment
    QDateTime departureTime = updatedFragment->departureTime().addSecs(-updatedFragment->departureDelay());
    QDateTime departureTimeWithDelay = updatedFragment->departureTime();
    QList<QUrl> pagesURI = m_cache.keys();
    QUrl updatedPageURI;

    for(qint64 pageCounter=0; pageCounter < pagesURI.length(); pageCounter++) {
        QUrl pageURI = pagesURI[pageCounter];
        QUrlQuery query = QUrlQuery(pageURI);
        QDateTime pageTime = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
        if(pageTime <= departureTimeWithDelay) { // just the page before the departure time must be considered too! This can be optimized...
            //qDebug() << "PAGE TIME:" << pageTime << "| DEP:" << departureTime << "| DEP+DEL:" << departureTimeWithDelay;
            QRail::Fragments::Page* page = m_cache.value(pageURI);
            QList<QRail::Fragments::Fragment *> fragments = page->fragments();

            // Check if the connection is in the fragments of this page
            for(qint32 fragCounter=0; fragCounter < fragments.length(); fragCounter++) {
                QRail::Fragments::Fragment *fragment = fragments.at(fragCounter);

                // Check if this fragment is the one we're looking for
                if(fragment->uri() == updatedFragment->uri()) {
                    //qDebug() << "Found fragment!";

                    // Departure delay changed, removing old fragment and inserting new fragment
                    if(fragment->departureDelay() != updatedFragment->departureDelay()) {
                        qDebug() << "Deleting old fragment, inserting new one";
                        // Remove old fragment in the old page
                        qDebug() << "----------PAGE HAS BEFORE:" << page->fragments().count() << " FRAGMENTS";
                        QList<QRail::Fragments::Fragment *> pageFrags = page->fragments();
                        QRail::Fragments::Fragment *frag = pageFrags.at(fragCounter);
                        pageFrags.removeAt(fragCounter);
                        // Memory managment
                        if(frag) {
                            delete frag;
                        }
                        page->setFragments(pageFrags);
                        updatedPageURI = page->uri();
                        qDebug() << "Departure delay update";
                        qDebug() << "----------PAGE HAS NOW:" << page->fragments().count() << " FRAGMENTS";
                        this->cachePage(page);

                        // Insert updated fragment in the new page
                        qDebug() << "Inserting updated fragment in other page...";
                        for(qint32 pageCounter=0; pageCounter < pagesURI.length() - 1; pageCounter++) {
                            QUrl currentPageURI = pagesURI.at(pageCounter);
                            QUrl nextPageURI = pagesURI.at(pageCounter + 1);
                            QUrlQuery currentPageQuery = QUrlQuery(currentPageURI);
                            QUrlQuery nextPageQuery = QUrlQuery(nextPageURI);
                            QDateTime currentPageTime = QDateTime::fromString(currentPageQuery.queryItemValue("departureTime"), Qt::ISODate);
                            QDateTime nextPageTime = QDateTime::fromString(nextPageQuery.queryItemValue("departureTime"), Qt::ISODate);
                            qDebug() << currentPageTime << "<=" << departureTime << "<" << nextPageTime;

                            // Look for the new page
                            if(currentPageTime <= departureTime && departureTime < nextPageTime) {
                                qDebug() << "Found inserting page, inserting fragment now";
                                QRail::Fragments::Page* currentPage = m_cache.value(currentPageURI);
                                qDebug() << "----------CURRENT PAGE HAS BEFORE:" << currentPage->fragments().count() << " FRAGMENTS";
                                QList<QRail::Fragments::Fragment *> currentPageFrags = currentPage->fragments();
                                currentPageFrags.append(updatedFragment);

                                // Keep page sorted
                                std::sort(currentPageFrags.begin(), currentPageFrags.end(), [](const QRail::Fragments::Fragment * a,
                                const QRail::Fragments::Fragment * b) -> bool {
                                    QDateTime timeA = a->departureTime();
                                    QDateTime timeB = b->departureTime();
                                    return timeA < timeB;
                                });
                                qDebug() << "Sorting OK";

                                currentPage->setFragments(currentPageFrags);
                                qDebug() << "Inserted into page";
                                qDebug() << "----------CURRENT PAGE HAS NOW:" << currentPage->fragments().count() << " FRAGMENTS";

                                this->cachePage(currentPage);
                                qDebug() << "Caching OK";
                                break;
                            }
                        }

                        // Update completed
                        return updatedPageURI;
                    }

                    // Arrival delay changed or cancelled (type changed), updating fragment in page
                    if(fragment->arrivalDelay() != updatedFragment->arrivalDelay()) {
                        qDebug() << "Updating old fragment";
                        QList<QRail::Fragments::Fragment *> pageFrags = page->fragments();
                        QRail::Fragments::Fragment * frag = pageFrags.at(fragCounter);
                        pageFrags.replace(fragCounter, updatedFragment);
                        // Memory managment
                        if(frag) {
                            delete frag;
                        }
                        updatedPageURI = page->uri();                     
                        qDebug() << "Arrival delay update";
                        page->setFragments(pageFrags);
                        qDebug() << "fragments=" << page->fragments();
                        this->cachePage(page);

                        // Update completed
                        return updatedPageURI;
                    }
                }
            }
        }
    }

    return QUrl();
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
    // If our fragment is already later than our last page or before our first page, don't even bother to search for the page.
    if(m_cache.count() > 0) {
        if(fragment->departureTime() > m_cache.last()->timestamp() || fragment->departureTime() < m_cache.first()->timestamp()) {
            return nullptr;
        }
    }

    foreach(QRail::Fragments::Page *page, m_cache.values()) {
        //qDebug() << "Fragment < page: " << fragment->departureTime() << "|" << page->timestamp();
        if(fragment->departureTime() < page->timestamp()) {
            continue;
        }
        else {
            return page;
        }
    }

    // In case we can't find a page (page hasn't been downloaded yet)
    //qWarning() << "Unable to find page for fragment:" << fragment->uri().toString() << "timestamp:" << fragment->departureTime();
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
