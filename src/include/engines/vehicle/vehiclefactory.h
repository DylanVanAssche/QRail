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
#ifndef VEHICLEFACTORY_H
#define VEHICLEFACTORY_H

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonValue>
#include <QtCore/QLocale>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QMap>
#include <QtCore/QRegularExpression>
#include <QtCore/QRegularExpressionMatch>
#include <QtCore/QMutex>

#include "engines/station/stationfactory.h"
#include "engines/station/stationstation.h"
#include "engines/vehicle/vehiclestop.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "engines/vehicle/vehiclenullvehicle.h"
#include "network/networkmanager.h"
#include "qrail.h"

// Uncomment to enable verbose output
//#define VERBOSE_HTTP_STATUS
#define VERBOSE_CACHE

namespace QRail {
namespace VehicleEngine {
class Factory : public QObject
{
    Q_OBJECT
public:
    static QRail::VehicleEngine::Factory *getInstance();
    void getVehicleByURI(const QUrl &uri, const QLocale::Language &language);
    QLocale::Language language() const;
    void setLanguage(const QLocale::Language &language);

signals:
    void finished(QSharedPointer<QRail::VehicleEngine::Vehicle> vehicle);
    void getResource(const QUrl &uri, QObject *caller);
    void error(const QString &message);

private:
    mutable QMutex vehicleProcessingMutex;
    QMap<QString, QSharedPointer<QRail::VehicleEngine::Vehicle>> m_cache;
    QRail::Network::Manager *m_http;
    StationEngine::Factory *m_stationFactory;
    QLocale::Language m_language;
    bool validateData(const QJsonObject &data, const QStringList &properties);
    QRail::Network::Manager *http() const;
    void setHttp(QRail::Network::Manager *http);
    StationEngine::Factory *stationFactory() const;
    void setStationFactory(StationEngine::Factory *stationFactory);
    QSharedPointer<QRail::VehicleEngine::Stop> generateStopFromJSON(const QJsonObject &stop);
    QRail::VehicleEngine::Stop::OccupancyLevel
    generateOccupancyLevelFromJSON(const QJsonObject &occupancy) const;
    QSharedPointer<QRail::VehicleEngine::Vehicle> fetchVehicleFromCache(const QUrl &uri);
    void addVehicleToCache(const QUrl &uri, QSharedPointer<QRail::VehicleEngine::Vehicle> vehicle);
    QString stripIDFromVehicleURI(const QUrl &uri);
    void processHTTPReply(QSharedPointer<QNetworkReply> reply);
    static QRail::VehicleEngine::Factory *m_instance;
    explicit Factory(QObject *parent = nullptr);
};
} // namespace VehicleEngine
} // namespace QRail

#endif // VEHICLEFACTORY_H
