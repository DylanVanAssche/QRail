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

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QLocale>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonValue>

#include "network/networkmanager.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "engines/vehicle/vehiclestop.h"
#include "engines/station/stationfactory.h"
#include "engines/station/stationstation.h"

namespace VehicleEngine {
class Factory : public QObject
{
    Q_OBJECT
public:
    static VehicleEngine::Factory *getInstance();
    void getVehicleByURI(const QUrl &uri, const QLocale::Language &language);
    QLocale::Language language() const;
    void setLanguage(const QLocale::Language &language);

signals:
    void vehicleReady(VehicleEngine::Vehicle *vehicle);
    void getResource(const QUrl &uri);
    void error(const QString &message);

private slots:
    void processHTTPReply(QNetworkReply *reply);

private:
    Network::Manager *m_http;
    StationEngine::Factory *m_stationFactory;
    QLocale::Language m_language;
    bool validateData(const QJsonObject &data, const QStringList &properties);
    Network::Manager *http() const;
    void setHttp(Network::Manager *http);
    StationEngine::Factory *stationFactory() const;
    void setStationFactory(StationEngine::Factory *stationFactory);
    VehicleEngine::Stop *generateStopFromJSON(const QJsonObject &stop);
    VehicleEngine::Stop::OccupancyLevel generateOccupancyLevelFromJSON(const QJsonObject &occupancy) const;
    static VehicleEngine::Factory *m_instance;
    explicit Factory(QObject *parent = nullptr);
};
}

#endif // VEHICLEFACTORY_H