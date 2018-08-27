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

#ifndef CSAFactory_H
#define CSAFactory_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QStringList>
#include <QtCore/QUrl>
#include <QtCore/QMap>
#include <QtCore/QFile>
#include <QtSql/QSqlQuery>

#include "qrail.h"
#include "engines/station/stationstation.h"
#include "engines/station/stationnullstation.h"
#include "database/databasemanager.h"
#include "../../qtcsv/include/qtcsv/stringdata.h"
#include "../../qtcsv/include/qtcsv/stringdata.h"
#include "../../qtcsv/include/qtcsv/reader.h"
#include "../../qtcsv/include/qtcsv/writer.h"

// TO DO: proper path for the DB
#define DB_PATH "/home/nemo/stations.db"

namespace StationEngine {
class QRAIL_SHARED_EXPORT Factory : public QObject
{
    Q_OBJECT
public:
    static Factory *getInstance();
    Station *getStationByURI(const QUrl &uri);

private:
    Database::Manager *m_db;
    QMap<QUrl, StationEngine::Station*> m_cache;
    bool initDatabase();
    QFuture<bool> insertStationWithFacilitiesIntoDatabase(const QStringList &station, const QStringList &facilities);
    QFuture<bool> insertStationWithoutFacilitiesIntoDatabase(const QStringList &station);
    QFuture<bool> insertPlatformIntoDatabase(const QStringList &stop);
    StationEngine::Station *fetchStationFromCache(const QUrl &uri) const;
    void addStationToCache(StationEngine::Station *station);
    QMap<QUrl, QString> getPlatformsByStationURI(const QUrl &uri);
    Database::Manager *db() const;
    void setDb(Database::Manager *db);
    static StationEngine::Factory *m_instance;
    explicit Factory(QObject *parent = nullptr);
};
}

#endif // CSAFactory_H
