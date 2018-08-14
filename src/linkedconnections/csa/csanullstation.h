/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of BeRail.                                               *
 *                                                                            *
 *   BeRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   BeRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with BeRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

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
#include "csastation.h"

namespace CSA {
class NullStation: public Station
{
public:
    static NullStation *getInstance(QObject *parent = nullptr);

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
            const bool &hasHearingAidSignal,
            const QMap<CSA::Station::Day, QPair<QTime, QTime>> &openingHours,
            const qreal &averageStopTimes,
            QObject *parent = nullptr);
    static CSA::NullStation *m_instance;
};
}

#endif // CSANULLSTATION_H
