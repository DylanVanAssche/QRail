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
    if(!m_cacheDir.exists()) {
        m_cacheDir.mkpath(path);
    }
}

void Cache::cachePage(Page *page)
{
    // Add the page to the LRU cache and return true if success
    qDebug() << "Inserted page:" << page->uri();
    m_cache.insert(page->uri(), page);

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
        //f.insert("pickupType", QJsonValue::fromVariant(frag->pickupType()));
        //f.insert("dropOffType", QJsonValue::fromVariant(frag->dropOffType()));
        fragments.append(f);
    }
    obj.insert("fragments", fragments);
    QJsonDocument doc = QJsonDocument(obj);

    // Save QJsonDocument to disk
    QString path = m_cacheDir.filePath(page->uri().toString());
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
        return m_cache.value(uri);
    }

    // If the requested page isn't cached, the Fragments::Factory will fetch it from the network
    return this->getPageFromDisk(uri);
}

bool Cache::hasPage(QUrl uri)
{
    return m_cache.contains(uri);
}

Page *Cache::getPageFromDisk(QUrl uri)
{
    // The page can be available on disk, but not in the RAM cache
    QString path = m_cacheDir.filePath(uri.toString());
    qDebug() << "Page file cache path:" << path;
    if(QDir(path).exists()) {
        // Read page from disk
        qDebug() << "Page found in disk cache";
        QFile jsonFile;
        jsonFile.setFileName(path);
        jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString data = jsonFile.readAll();
        jsonFile.close();
        QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
        QJsonObject obj = d.object();

        // Convert QJsonObject to QRail::Fragments::Page *
        QRail::Fragments::Page *page = new QRail::Fragments::Page();
        page->setURI(QUrl(d["uri"].toString()));
        page->setHydraNext(QUrl(d["hydraNext"].toString()));
        page->setHydraPrevious(QUrl(d["hydraPrevious"].toString()));
        page->setTimestamp(QDateTime::fromString(d["timestamp"].toString(), Qt::ISODate));
        QJsonArray fragments = d["fragments"].toArray();
        foreach(QJsonValue item, fragments) {
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
            //fragment->setPickupType(frag["pickupType"].toString());
            //fragment->setDropOffType(frag["dropOffType"].toString());
        }
    }

    // No page available, return NULL pointer
    qWarning() << "Cannot find page in disk cache";
    return nullptr;
}
