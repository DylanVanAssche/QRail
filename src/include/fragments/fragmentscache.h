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
#include <QtCore/QMap>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QStandardPaths>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QDebug>
#include "fragments/fragmentspage.h"
#include "fragments/fragmentsfragment.h"
#define MAX_COST 24*60*50*1000 // Allocate space for 50 Kb pages (24 hours, 60 pages/hour) = 72 Mb RAM
#define PAGE_FILE_NAME "/page.jsonld"

namespace QRail {
namespace Fragments {
class Cache : public QObject
{
    Q_OBJECT
public:
    explicit Cache(QObject *parent = nullptr);
    void cachePage(QRail::Fragments::Page *page);
    void updateFragment(QRail::Fragments::Fragment *fragment);
    QRail::Fragments::Page *getPageByURI(QUrl uri);
    QRail::Fragments::Page *getPageByFragment(QRail::Fragments::Fragment *fragment);
    bool hasPage(QUrl uri);
    bool isEmpty();

private:
    QMap<QUrl, QRail::Fragments::Page*> m_cache;
    QRail::Fragments::Page *getPageFromDisk(QUrl uri);
    QDir m_cacheDir;
    QJsonValue convertGTFSTypeToJson(QRail::Fragments::Fragment::GTFSTypes type);
    QRail::Fragments::Fragment::GTFSTypes convertJsonToGTFSType(QJsonValue type);
};
}
}


#endif // FRAGMENTSCACHE_H
