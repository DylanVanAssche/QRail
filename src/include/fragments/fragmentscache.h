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
#ifndef FRAGMENTSCACHE_H
#define FRAGMENTSCACHE_H

#include <QtCore/QObject>
#include <QtCore/QCache>
#include <QtCore/QUrl>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include "fragments/fragmentspage.h"
#include "fragments/fragmentsfragment.h"
#define MAX_COST 24*60*50*1000 // Allocate space for 50 Kb pages (24 hours, 60 pages/hour) = 72 Mb RAM

namespace QRail {
namespace Fragments {
class Cache : public QObject
{
    Q_OBJECT
public:
    explicit Cache(QObject *parent = nullptr);
    bool cachePage(QRail::Fragments::Page *page);
    void updateFragment(QRail::Fragments::Fragment *fragment);
    QRail::Fragments::Page *getPageByURI(QUrl uri);
    bool hasPage(QUrl uri);

signals:
    void pageUpdated(QUrl uri);

private:
    QCache<QUrl, QRail::Fragments::Page*> m_cache;
    QRail::Fragments::Page *Cache::getPageFromDisk(QUrl uri);
    QDir m_cacheDir;
};
}
}


#endif // FRAGMENTSCACHE_H
