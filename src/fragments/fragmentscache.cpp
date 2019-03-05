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
    // Create LRU cache
    m_cache = QCache<QUrl, QRail::Fragments::Page*>(MAX_COST);

    // Disk cache directory creation
    // Get the caching directory of the application
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/fragments";

    // Create the 'fragments' folder to save our caching data
    m_cacheDir.mkpath(path);
}

bool Cache::cachePage(Page *page)
{
    // Add the page to the LRU cache and return true if success
    qDebug() << "Inserted page:" << page->uri();
    return m_cache.insert(page->uri(), page);
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
            QRail::Fragments::Page* page = m_cache.object(pageURI);
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
                                QRail::Fragments::Page* currentPage = m_cache.object(currentPageURI);
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
        return m_cache::object(uri);
    }

    QJsonObject jsonPage = this->findPageOnDisk(uri);
    if(!jsonPage.isEmpty()) {
        QUrl uri;
        QDateTime timestamp;
        QUrl hydraPrevious;
        QUrl hydraNext;
        QList<QRail::Fragments::Fragment *> fragments;
        QRail::Fragments::Page* diskPage = new QRail::Fragments::Page(uri, timestamp, hydraNext, hydraPrevious, fragments);
    }

    // The requested page isn't cached, the Fragments::Factory will fetch it from the network
    return nullptr;
}

QJsonObject Cache::findPageOnDisk(QUrl uri)
{
    // The page can be available on disk, but not in the RAM cache
    QString path = m_cacheDir.filePath(uri.toString());
    qDebug() << "Page file cache path:" << path;
    if(QDir(path).exists()) {
        qDebug() << "Page found in disk cache";
        QFile jsonFile;
        jsonFile.setFileName(path);
        jsonFile.open(QIODevice::ReadOnly | QIODevice::Text);
        QString data = jsonFile.readAll();
        jsonFile.close();
        QJsonDocument d = QJsonDocument::fromJson(data.toUtf8());
        return d.object();
    }

    qWarning() << "Cannot find page in disk cache";
    return QJsonObject();
}
