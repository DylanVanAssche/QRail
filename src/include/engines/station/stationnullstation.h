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

#ifndef CSANULLSTATION_H
#define CSANULLSTATION_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QMap>
#include <QtCore/QLocale>
#include <QtCore/QTime>
#include <QtCore/QPair>
#include <QtPositioning/QGeoAddress>
#include <QtPositioning/QGeoCoordinate>

#include "engines/station/stationstation.h"
#include "qrail.h"

namespace QRail {
namespace StationEngine {
class QRAIL_SHARED_EXPORT NullStation: public Station
{
public:
    static NullStation *getInstance();

private:
    explicit NullStation(
        const QUrl &uri,
        const QMap<QLocale::Language, QString> &name,
        const QLocale::Country &country,
        const QGeoCoordinate &position,
        const QGeoAddress &address,
        const bool &hasTicketVendingMachine,
        const bool &hasLuggageLockers,
        const bool &hasFreeParking,
        const bool &hasTaxi,
        const bool &hasBicycleSpots,
        const bool &hasBlueBike,
        const bool &hasBus,
        const bool &hasTram,
        const bool &hasMetro,
        const bool &hasWheelchairAvailable,
        const bool &hasRamp,
        const qint16 &disabledParkingSpots,
        const bool &hasElevatedPlatform,
        const bool &hasEscalatorUp,
        const bool &hasEscalatorDown,
        const bool &hasElevatorPlatform,
        const bool &hasAudioInductionLoop,
        const QMap<StationEngine::Station::Day, QPair<QTime, QTime>> &openingHours,
        const qreal &averageStopTimes,
        const quint32 &officialTransferTimes,
        QObject *parent = nullptr);
    static StationEngine::NullStation *m_instance;
};
}
}

#endif // CSANULLSTATION_H
