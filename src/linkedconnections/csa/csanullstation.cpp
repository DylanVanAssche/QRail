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

#include "csanullstation.h"
CSA::NullStation* CSA::NullStation::m_instance = nullptr;

/**
 * @file csanullstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::NullStation constructor
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const QGeoAddress &address
 * @param const bool &hasTicketVendingMachine
 * @param const bool &hasLuggageLockers
 * @param const bool &hasFreeParking
 * @param const bool &hasTaxi
 * @param const bool &hasBicyclSpots
 * @param const bool &hasBus
 * @param const bool &hasTram
 * @param const bool &hasMetro
 * @param const bool &hasWheelchairAvailable
 * @param const bool &hasRamp
 * @param const qint16 &disabledParkingSpots
 * @param const bool &hasElevatedPlatform
 * @param const bool &hasEscalatorUp
 * @param const bool &hasEscalatorDown
 * @param const bool &hasElevatorPlatform
 * @param const bool &hasHearingAidSignal
 * @param const QMap<CSA::Station::Day, QPair<QTime, QTime>> &openingHours
 * @param const qreal &averageStopTimes
 * @param QObject *parent
 * @package CSA
 * @private
 * Constructs a CSA::NullStation according to the Null design pattern.
 */
CSA::NullStation::NullStation(const QUrl &uri, const QMap<QLocale::Language, QString> &name, const QLocale::Country &country, const QGeoCoordinate &position, const QGeoAddress &address, const bool &hasTicketVendingMachine, const bool &hasLuggageLockers, const bool &hasFreeParking, const bool &hasTaxi, const bool &hasBicycleSpots, const bool &hasBlueBike, const bool &hasBus, const bool &hasTram, const bool &hasMetro, const bool &hasWheelchairAvailable, const bool &hasRamp, const qint16 &disabledParkingSpots, const bool &hasElevatedPlatform, const bool &hasEscalatorUp, const bool &hasEscalatorDown, const bool &hasElevatorPlatform, const bool &hasHearingAidSignal, const QMap<CSA::Station::Day, QPair<QTime, QTime> > &openingHours, const qreal &averageStopTimes, QObject *parent) :
    Station(uri, name, country, position, address, hasTicketVendingMachine, hasLuggageLockers, hasFreeParking, hasTaxi, hasBicycleSpots, hasBlueBike, hasBus, hasTram, hasMetro, hasWheelchairAvailable, hasRamp, disabledParkingSpots, hasElevatedPlatform, hasEscalatorDown, hasEscalatorUp, hasElevatorPlatform, hasHearingAidSignal, openingHours, averageStopTimes, parent)
{
    // Clean up when parent dies
    this->setParent(parent);
}

/**
 * @file csanullstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a CSA::NullStation instance
 * @param QObject *parent = nullptr
 * @return CSA::NullStation *station
 * @package CSA
 * @public
 * Constructs a CSA::NullStation if none exists and returns the instance.
 */
CSA::NullStation *CSA::NullStation::getInstance(QObject *parent)
{
    // Singleton pattern
    if(m_instance == nullptr) {
        qDebug() << "Generating new NullStation";
        m_instance = new CSA::NullStation(
                QUrl(),
                QMap<QLocale::Language, QString>(),
                QLocale::Country::Belgium,
                QGeoCoordinate(),
                QGeoAddress(),
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                false,
                0,
                false,
                false,
                false,
                false,
                false,
                QMap<CSA::Station::Day, QPair<QTime, QTime> >(),
                0.0,
                parent);
    }
    return m_instance;
}
