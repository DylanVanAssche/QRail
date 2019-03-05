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
    m_cache = QCache(MAX_COST);

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

void Cache::updateFragment(Fragment *fragment)
{
    // lookup fragment from departureTime -> departureTime + departureDelay
    QDateTime departureTime = fragment->departureTime().addSecs(-fragment->departureDelay());
    QDateTime departureTimeWithDelay = fragment->departureTime();

    // remove old fragment and insert the new one if departureDelay is different
    // return

    // replace fragment if only arrivalDelay or type has been changed
}

Page *Cache::getPageByURI(QUrl uri)
{
    // Try to get the page from the RAM cache
    if(m_cache.contains(uri)) {
        return m_cache::object(uri);
    }

    QJsonObject jsonPage = this->findPageOnDisk(uri);
    if(!jsonPage.isEmpty()) {
        // Convert to page object
        // return page
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
        // Read page and return JSON
    }

    qWarning() << "Cannot find page in disk cache";
    return QJsonObject();
}

