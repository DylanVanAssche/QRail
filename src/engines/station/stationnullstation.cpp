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

#include "engines/station/stationnullstation.h"
StationEngine::NullStation* StationEngine::NullStation::m_instance = nullptr;

/**
 * @file routernullstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::NullStation constructor
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
 * @param const QMap<StationEngine::Station::Day, QPair<QTime, QTime>> &openingHours
 * @param const qreal &averageStopTimes
 * @param QObject *parent
 * @package StationEngine
 * @private
 * Constructs a StationEngine::NullStation according to the Null design pattern.
 */
StationEngine::NullStation::NullStation(const QUrl &uri, const QMap<QLocale::Language, QString> &name, const QLocale::Country &country, const QGeoCoordinate &position, const QGeoAddress &address, const bool &hasTicketVendingMachine, const bool &hasLuggageLockers, const bool &hasFreeParking, const bool &hasTaxi, const bool &hasBicycleSpots, const bool &hasBlueBike, const bool &hasBus, const bool &hasTram, const bool &hasMetro, const bool &hasWheelchairAvailable, const bool &hasRamp, const qint16 &disabledParkingSpots, const bool &hasElevatedPlatform, const bool &hasEscalatorUp, const bool &hasEscalatorDown, const bool &hasElevatorPlatform, const bool &hasHearingAidSignal, const QMap<StationEngine::Station::Day, QPair<QTime, QTime> > &openingHours, const qreal &averageStopTimes, QObject *parent) :
    Station(uri, name, country, position, address, hasTicketVendingMachine, hasLuggageLockers, hasFreeParking, hasTaxi, hasBicycleSpots, hasBlueBike, hasBus, hasTram, hasMetro, hasWheelchairAvailable, hasRamp, disabledParkingSpots, hasElevatedPlatform, hasEscalatorDown, hasEscalatorUp, hasElevatorPlatform, hasHearingAidSignal, openingHours, averageStopTimes, parent)
{

}

/**
 * @file routernullstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a StationEngine::NullStation instance
 * @param QObject *parent = nullptr
 * @return StationEngine::NullStation *station
 * @package StationEngine
 * @public
 * Constructs a StationEngine::NullStation if none exists and returns the instance.
 */
StationEngine::NullStation *StationEngine::NullStation::getInstance()
{
    // Singleton pattern
    if(m_instance == nullptr) {
        qDebug() << "Generating new NullStation";
        m_instance = new StationEngine::NullStation(
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
                    QMap<StationEngine::Station::Day, QPair<QTime, QTime> >(),
                    0.0
                    );
    }
    return m_instance;
}
