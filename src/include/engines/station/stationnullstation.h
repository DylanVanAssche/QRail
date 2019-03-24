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
#ifndef STATIONNULLSTATION_H
#define STATIONNULLSTATION_H

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
/**
 * @class NullStation
 * @brief An StationEngine::NullStation contains a empty station.
 * A station contains all the information about a stop.
 * \warning Receiving a NullStation from the QRail library indicates that something must be terrible wrong! Check you debug output carefully.
 */
class QRAIL_SHARED_EXPORT NullStation: public Station
{
public:
    //! Gets a StationEngine::NullStation instance
    /*!
        \return a StationEngine::NullStation instance.
        \public
        Constructs a StationEngine::NullStation if none exists and returns the instance.
     */
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

#endif // STATIONNULLSTATION_H
