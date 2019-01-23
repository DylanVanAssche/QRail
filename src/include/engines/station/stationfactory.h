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
#include <QtCore/QDir>
#include <QtCore/QStandardPaths>
#include <QtCore/QFuture>
#include <QtCore/QPair>
#include <QtSql/QSqlQuery>
#include <QtConcurrent/QtConcurrent>
#include <algorithm>

#include "qrail.h"
#include "engines/station/stationstation.h"
#include "engines/station/stationnullstation.h"
#include "database/databasemanager.h"
#include "../../qtcsv/include/qtcsv/stringdata.h"
#include "../../qtcsv/include/qtcsv/stringdata.h"
#include "../../qtcsv/include/qtcsv/reader.h"
#include "../../qtcsv/include/qtcsv/writer.h"

// Uncomment to enable verbose output
//#define VERBOSE_CACHE

#define SEARCH_RADIUS_NEAREST_STATION 3.0 // 3.0 km

namespace QRail {
namespace StationEngine {
class QRAIL_SHARED_EXPORT Factory : public QObject
{
    Q_OBJECT
public:
    //! Gets a StationEngine::Factory instance.
    /*!
        \return An instance of StationEngine::Factory.
        \public
        Constructs a StationEngine::Factory if none exists and returns the instance.
     */
    static Factory *getInstance();
    //! Retrieves a station by its URI.
    /*!
        \param uri the URI of the station you want to retrieve.
        \return An instance of StationEngine::Station with all the data about the requested station.
        \public
        Searches the database by the URI for a certain station.<br>
        In case something goes wrong, a StationEngine::NullStation instance is returned.
     */
    QRail::StationEngine::Station *getStationByURI(const QUrl &uri);
    //! Gets all the station in the area.
    /*!
        \param position a GPS coordinate to define the center of the search circle.
        \param radius the radius of the search circle in kilometres.
        \param maxResults limits the amount of results this method can return.
        \return A QList<QPair<QRail::StationEngine::Station *, qreal>> with a StationEngine::Station object and the distance to station.
        \public
        Fetches nearby stations from database using the Haversine formula (Google's solution).<br>
        In case something goes wrong, a StationEngine::NullStation instance is
        pushed to the QList<QPair<StationEngine::Station *, qreal>> &nearbyStations. <br>
        If you supply invalid input data, an empty QList is returned.
     */
    QList<QPair<QRail::StationEngine::Station *, qreal>> getStationsInTheAreaByPosition(const QGeoCoordinate &position,
                                                                                        const qreal &radius,
                                                                                        const quint32 &maxResults);
    //! Gets the closest station in the area.
    /*!
        \param position a GPS coordinate to define the center of the search circle.
        \param radius the radius of the search circle in kilometres.
        \return A QPair with the StationEngine::Station object and it's distance from the given position.
        \public
        Fetches nearby stations from database using the Haversine formula.<br>
        In case something goes wrong, a StationEngine::NullStation instance is returned.
     */
    QPair<QRail::StationEngine::Station *, qreal> getNearestStationByPosition(const QGeoCoordinate &position,
                                                                              const qreal radius);
    //! Find matching stations by their name.
    /*!
        \param query a QString search query.
        \return a QList<StationEngine::Station *station> of matching station with the query.
        \public
        Ideal to implement a search engine based on the station name.
     */
    QList<QRail::StationEngine::Station *> getStationsByName(const QString &query);

private:
    QRail::Database::Manager *m_db;
    QMap<QUrl, StationEngine::Station *> m_cache;
    bool initDatabase();
    bool insertStationWithFacilitiesIntoDatabase(const QStringList &station, const QStringList &facilities);
    bool insertStationWithoutFacilitiesIntoDatabase(const QStringList &station);
    bool insertPlatformIntoDatabase(const QStringList &stop);
    StationEngine::Station *fetchStationFromCache(const QUrl &uri) const;
    void addStationToCache(StationEngine::Station *station);
    QMap<QUrl, QString> getPlatformsByStationURI(const QUrl &uri);
    QRail::Database::Manager *db() const;
    void setDb(QRail::Database::Manager *db);
    static StationEngine::Factory *m_instance;
    explicit Factory(QObject *parent = nullptr);
};
}
}

#endif // CSAFactory_H
