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

#include "csaroute.h"

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Route constructor: full
 * @param const QList<CSA::RouteLeg *> &legs
 * @param const QList<CSA::Transfer *> &transfers
 * @param const QList<Message *> &tripAlerts
 * @param const QList<CSA::Message *> &vehicleAlerts
 * @param const QList<CSA::Message *> &remarks
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a Route with a list of legs and transfers
 */
CSA::Route::Route(const QList<CSA::RouteLeg *> &legs, const QList<CSA::Transfer *> &transfers, const QList<Message *> &tripAlerts, const QList<CSA::Message *> &vehicleAlerts, const QList<CSA::Message *> &remarks, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_legs = legs;
    m_transfers = transfers;
    m_tripAlerts = tripAlerts;
    m_vehicleAlerts = vehicleAlerts;
    m_remarks = remarks;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Route constructor: full
 * @param const QList<CSA::RouteLeg *> &legs
 * @param const QList<CSA::Transfer *> &transfers
 * @param const QList<Message *> &tripAlerts
 * @param const QList<CSA::Message *> &vehicleAlerts
 * @param const QList<CSA::Message *> &remarks
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a Route with a list of legs. The transfers are generated from the list of legs.
 */
CSA::Route::Route(const QList<CSA::RouteLeg *> &legs, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_legs = legs;

    // Calculate Transfer for easy data access
    m_transfers = QList<CSA::Transfer *>();

    // Departure RouteLegEnd
    m_transfers.append(new CSA::Transfer(
                           this->legs().first(),
                           nullptr,
                           this)
                       );

    // Intermediate legs (transfers)
    for(qint16 i = 1; i < this->legs().size(); i++) {
        m_transfers.append(new CSA::Transfer(
                               this->legs().at(i),
                               this->legs().at(i-1),
                               this)
                           );
    }

    // Arrival RouteLegEnd
    m_transfers.append(new CSA::Transfer(
                           nullptr,
                           this->legs().last(),
                           this)
                       );

    m_tripAlerts = QList<CSA::Message *>();
    m_vehicleAlerts = QList<CSA::Message *>(); // TODO: Bert Marcelis implements this one as a 2D array
    m_remarks = QList<CSA::Message *>();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of legs
 * @return const QList<CSA::RouteLeg *> legs
 * @package CSA
 * @public
 * Gets the list of legs and returns it.
 */
QList<CSA::RouteLeg *> CSA::Route::legs() const
{
    return m_legs;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of legs
 * @param const QList<CSA::RouteLeg *> &legs
 * @package CSA
 * @public
 * Sets the list of legs to the given QList<CSA::RouteLeg *> &legs.
 */
void CSA::Route::setLegs(const QList<CSA::RouteLeg *> &legs)
{
    m_legs = legs;
    emit this->legsChanged();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of transfers
 * @return const QList<CSA::Transfer *> transfers
 * @package CSA
 * @public
 * Gets the list of transfers and returns it.
 */
QList<CSA::Transfer *> CSA::Route::transfers() const
{
    return m_transfers;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of transfers
 * @param const QList<CSA::Transfer *> &transfers
 * @package CSA
 * @public
 * Sets the list of transfers to the given QList<CSA::Transfer *> &transfers.
 */
void CSA::Route::setTransfers(const QList<CSA::Transfer *> &transfers)
{
    m_transfers = transfers;
    emit this->transfersChanged();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of trip alerts
 * @return const QList<CSA::Message *> tripAlerts
 * @package CSA
 * @public
 * Gets the list of trip alerts and returns it.
 */
QList<CSA::Message *> CSA::Route::tripAlerts() const
{
    return m_tripAlerts;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of trip alerts
 * @param const QList<CSA::Message *> &tripAlerts
 * @package CSA
 * @public
 * Sets the list of trip alerts to the given QList<CSA::Message *> &tripAlerts.
 */
void CSA::Route::setTripAlerts(const QList<CSA::Message *> &tripAlerts)
{
    m_tripAlerts = tripAlerts;
    emit this->tripAlertsChanged();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of vehicle alerts
 * @return const QList<CSA::Message *> vehicleAlerts
 * @package CSA
 * @public
 * Gets the list of vehicle alerts and returns it.
 */
QList<CSA::Message *> CSA::Route::vehicleAlerts() const
{
    return m_vehicleAlerts;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of vehicle alerts
 * @param const QList<CSA::Message *> &vehicleAlerts
 * @package CSA
 * @public
 * Sets the list of vehicle alerts to the given QList<CSA::Message *> &vehicleAlerts.
 */
void CSA::Route::setVehicleAlerts(const QList<CSA::Message *> &vehicleAlerts)
{
    m_vehicleAlerts = vehicleAlerts;
    emit this->vehicleAlertsChanged();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of remarks
 * @return const QList<CSA::Message *> remarks
 * @package CSA
 * @public
 * Gets the list of remarks and returns it.
 */
QList<CSA::Message *> CSA::Route::remarks() const
{
    return m_remarks;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of remarks
 * @param const QList<CSA::Message *> &remarks
 * @package CSA
 * @public
 * Sets the list of remarks to the given QList<CSA::Message *> &remarks.
 */
void CSA::Route::setRemarks(const QList<CSA::Message *> &remarks)
{
    m_remarks = remarks;
    emit this->remarksChanged();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the duration
 * @return const qint64 duration
 * @package CSA
 * @public
 * Gets the duration of the route and returns it.
 */
qint64 CSA::Route::duration() const
{
    return this->departureTime().secsTo(this->arrivalTime());
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the duration with delays
 * @return const qint64 durationWithDelays
 * @package CSA
 * @public
 * Gets the duration with the delays included of the route and returns it.
 */
qint64 CSA::Route::durationWithDelays() const
{
    return this->departureDelayedTime().secsTo(this->arrivalDelayedTime());
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure time
 * @return const QDateTime departureTime
 * @package CSA
 * @public
 * Gets the departure time of the route and returns it.
 */
QDateTime CSA::Route::departureTime() const
{
    return this->departureStation()->departure()->time();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure delay
 * @return const qint16 departureDelay
 * @package CSA
 * @public
 * Gets the departure delay of the route and returns it.
 */
qint16 CSA::Route::departureDelay() const
{
    return this->departureStation()->departure()->delay();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure time with delays
 * @return const QDateTime departureDelayedTime
 * @package CSA
 * @public
 * Gets the departure time with the delays included of the route and returns it.
 */
QDateTime CSA::Route::departureDelayedTime() const
{
    return this->departureTime().addSecs(this->departureDelay());
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival time
 * @return const QDateTime arrivalTime
 * @package CSA
 * @public
 * Gets the arrival time of the route and returns it.
 */
QDateTime CSA::Route::arrivalTime() const
{
    return this->arrivalStation()->arrival()->time();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival delay
 * @return const qint16 arrivalDelay
 * @package CSA
 * @public
 * Gets the arrival delay of the route and returns it.
 */
qint16 CSA::Route::arrivalDelay() const
{
    return this->arrivalStation()->arrival()->delay();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival time with delays
 * @return const QDateTime arrivalDelayedTime
 * @package CSA
 * @public
 * Gets the arrival time with the delays included of the route and returns it.
 */
QDateTime CSA::Route::arrivalDelayedTime() const
{
    return this->arrivalTime().addSecs(this->arrivalDelay());
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the number of transfers
 * @return const qint16 transfers
 * @package CSA
 * @public
 * Gets the number of transfers and returns it.
 */
qint16 CSA::Route::transferCount() const
{
    // Departure and arrival stations aren't transfer stops
    return this->legs().size() - 1;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the number of stations
 * @return const qint16 stations
 * @package CSA
 * @public
 * Gets the number of stations and returns it.
 */
qint16 CSA::Route::stationCount() const
{
    // Number of transfers + arrival & departure station
    return this->transferCount() + 2;
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform for the departure
 * @return const QString departurePlatform
 * @package CSA
 * @public
 * Gets the platform for the departure of the route and returns it.
 */
QString CSA::Route::departurePlatform() const
{
    return this->departureStation()->departure()->platform();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isDeparturePlatformNormal
 * @return const bool isDeparturePlatformNormal
 * @package CSA
 * @public
 * Gets the isDeparturePlatformNormal and returns it.
 */
bool CSA::Route::isDeparturePlatformNormal() const
{
    return this->departureStation()->departure()->isNormalPlatform();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform for the arrival
 * @return const QString arrivalPlatform
 * @package CSA
 * @public
 * Gets the platform for the arrival of the route and returns it.
 */
QString CSA::Route::arrivalPlatform() const
{
    return this->arrivalStation()->arrival()->platform();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isArrivalPlatformNormal
 * @return const bool isArrivalPlatformNormal
 * @package CSA
 * @public
 * Gets the isArrivalPlatformNormal and returns it.
 */
bool CSA::Route::isArrivalPlatformNormal() const
{
    return this->arrivalStation()->arrival()->isNormalPlatform();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the CSA::Transfer departureStation
 * @return CSA::Transfer *departureStation
 * @package CSA
 * @public
 * Gets the departure station CSA::Transfer and returns it.
 */
CSA::Transfer *CSA::Route::departureStation() const
{
    return this->transfers().first();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the CSA::Transfer arrivalStation
 * @return CSA::Transfer *arrivalStation
 * @package CSA
 * @public
 * Gets the arrival station CSA::Transfer and returns it.
 */
CSA::Transfer *CSA::Route::arrivalStation() const
{
    return this->transfers().last();
}

/**
 * @file csaroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPartiallyCanceled
 * @return const bool isPartiallyCanceled
 * @package CSA
 * @public
 * Gets the isPartiallyCanceled of the route and returns it.
 */
bool CSA::Route::isPartiallyCanceled() const
{
    foreach(RouteLeg *leg, this->legs()) {
        if(leg->departure()->isCanceled()) {
            return true;
        }
    }
    return false;
}
