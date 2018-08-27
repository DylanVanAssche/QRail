/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of QRail.                                               *
 *                                                                            *
 *   QRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   QRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with QRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#include "engines/router/routerroute.h"

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Route constructor: full
 * @param const QList<RouterEngine::RouteLeg *> &legs
 * @param const QList<RouterEngine::Transfer *> &transfers
 * @param const QList<AlertsEngine::Message *> &tripAlerts
 * @param const QList<AlertsEngine::Message *> &vehicleAlerts
 * @param const QList<AlertsEngine::Message *> &remarks
 * @param QObject *parent
 * @package RouterEngine
 * @public
 * Constructs a Route with a list of legs and transfers
 */
RouterEngine::Route::Route(const QList<RouterEngine::RouteLeg *> &legs, const QList<RouterEngine::Transfer *> &transfers, const QList<AlertsEngine::Message *> &tripAlerts, const QList<AlertsEngine::Message *> &vehicleAlerts, const QList<AlertsEngine::Message *> &remarks, QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_legs = legs;
    m_transfers = transfers;
    m_tripAlerts = tripAlerts;
    m_vehicleAlerts = vehicleAlerts;
    m_remarks = remarks;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Route constructor: full
 * @param const QList<RouterEngine::RouteLeg *> &legs
 * @param const QList<RouterEngine::Transfer *> &transfers
 * @param const QList<Message *> &tripAlerts
 * @param const QList<AlertsEngine::Message *> &vehicleAlerts
 * @param const QList<AlertsEngine::Message *> &remarks
 * @param QObject *parent
 * @package RouterEngine
 * @public
 * Constructs a Route with a list of legs. The transfers are generated from the list of legs.
 */
RouterEngine::Route::Route(const QList<RouterEngine::RouteLeg *> &legs, QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_legs = legs;

    // Calculate Transfer for easy data access
    m_transfers = QList<RouterEngine::Transfer *>();

    // Departure RouteLegEnd
    m_transfers.append(new RouterEngine::Transfer(
                           this->legs().first(),
                           nullptr,
                           this)
                       );

    // Intermediate legs (transfers)
    for(qint16 i = 1; i < this->legs().size(); i++) {
        m_transfers.append(new RouterEngine::Transfer(
                               this->legs().at(i),
                               this->legs().at(i-1),
                               this)
                           );
    }

    // Arrival RouteLegEnd
    m_transfers.append(new RouterEngine::Transfer(
                           nullptr,
                           this->legs().last(),
                           this)
                       );

    m_tripAlerts = QList<AlertsEngine::Message *>();
    m_vehicleAlerts = QList<AlertsEngine::Message *>(); // TODO: Bert Marcelis implements this one as a 2D array
    m_remarks = QList<AlertsEngine::Message *>();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of legs
 * @return const QList<RouterEngine::RouteLeg *> legs
 * @package RouterEngine
 * @public
 * Gets the list of legs and returns it.
 */
QList<RouterEngine::RouteLeg *> RouterEngine::Route::legs() const
{
    return m_legs;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of legs
 * @param const QList<RouterEngine::RouteLeg *> &legs
 * @package RouterEngine
 * @public
 * Sets the list of legs to the given QList<RouterEngine::RouteLeg *> &legs.
 */
void RouterEngine::Route::setLegs(const QList<RouterEngine::RouteLeg *> &legs)
{
    m_legs = legs;
    emit this->legsChanged();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of transfers
 * @return const QList<RouterEngine::Transfer *> transfers
 * @package RouterEngine
 * @public
 * Gets the list of transfers and returns it.
 */
QList<RouterEngine::Transfer *> RouterEngine::Route::transfers() const
{
    return m_transfers;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of transfers
 * @param const QList<RouterEngine::Transfer *> &transfers
 * @package RouterEngine
 * @public
 * Sets the list of transfers to the given QList<RouterEngine::Transfer *> &transfers.
 */
void RouterEngine::Route::setTransfers(const QList<RouterEngine::Transfer *> &transfers)
{
    m_transfers = transfers;
    emit this->transfersChanged();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of trip alerts
 * @return const QList<AlertsEngine::Message *> tripAlerts
 * @package RouterEngine
 * @public
 * Gets the list of trip alerts and returns it.
 */
QList<AlertsEngine::Message *> RouterEngine::Route::tripAlerts() const
{
    return m_tripAlerts;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of trip alerts
 * @param const QList<AlertsEngine::Message *> &tripAlerts
 * @package RouterEngine
 * @public
 * Sets the list of trip alerts to the given QList<AlertsEngine::Message *> &tripAlerts.
 */
void RouterEngine::Route::setTripAlerts(const QList<AlertsEngine::Message *> &tripAlerts)
{
    m_tripAlerts = tripAlerts;
    emit this->tripAlertsChanged();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of vehicle alerts
 * @return const QList<AlertsEngine::Message *> vehicleAlerts
 * @package RouterEngine
 * @public
 * Gets the list of vehicle alerts and returns it.
 */
QList<AlertsEngine::Message *> RouterEngine::Route::vehicleAlerts() const
{
    return m_vehicleAlerts;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of vehicle alerts
 * @param const QList<AlertsEngine::Message *> &vehicleAlerts
 * @package RouterEngine
 * @public
 * Sets the list of vehicle alerts to the given QList<AlertsEngine::Message *> &vehicleAlerts.
 */
void RouterEngine::Route::setVehicleAlerts(const QList<AlertsEngine::Message *> &vehicleAlerts)
{
    m_vehicleAlerts = vehicleAlerts;
    emit this->vehicleAlertsChanged();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the list of remarks
 * @return const QList<AlertsEngine::Message *> remarks
 * @package RouterEngine
 * @public
 * Gets the list of remarks and returns it.
 */
QList<AlertsEngine::Message *> RouterEngine::Route::remarks() const
{
    return m_remarks;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the list of remarks
 * @param const QList<AlertsEngine::Message *> &remarks
 * @package RouterEngine
 * @public
 * Sets the list of remarks to the given QList<AlertsEngine::Message *> &remarks.
 */
void RouterEngine::Route::setRemarks(const QList<AlertsEngine::Message *> &remarks)
{
    m_remarks = remarks;
    emit this->remarksChanged();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the duration
 * @return const qint64 duration
 * @package RouterEngine
 * @public
 * Gets the duration of the route and returns it.
 */
qint64 RouterEngine::Route::duration() const
{
    return this->departureTime().secsTo(this->arrivalTime());
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure time
 * @return const QDateTime departureTime
 * @package RouterEngine
 * @public
 * @note Delays are already included in this timestamp.
 * Gets the departure time of the route and returns it.
 */
QDateTime RouterEngine::Route::departureTime() const
{
    return this->departureStation()->departure()->time();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure delay
 * @return const qint16 departureDelay
 * @package RouterEngine
 * @public
 * Gets the departure delay of the route and returns it.
 */
qint16 RouterEngine::Route::departureDelay() const
{
    return this->departureStation()->departure()->delay();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival time
 * @return const QDateTime arrivalTime
 * @package RouterEngine
 * @public
 * @note Delays are already included in this timestamp.
 * Gets the arrival time of the route and returns it.
 */
QDateTime RouterEngine::Route::arrivalTime() const
{
    return this->arrivalStation()->arrival()->time();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival delay
 * @return const qint16 arrivalDelay
 * @package RouterEngine
 * @public
 * Gets the arrival delay of the route and returns it.
 */
qint16 RouterEngine::Route::arrivalDelay() const
{
    return this->arrivalStation()->arrival()->delay();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the number of transfers
 * @return const qint16 transfers
 * @package RouterEngine
 * @public
 * Gets the number of transfers and returns it.
 */
qint16 RouterEngine::Route::transferCount() const
{
    // Departure and arrival stations aren't transfer stops
    return this->legs().size() - 1;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the number of stations
 * @return const qint16 stations
 * @package RouterEngine
 * @public
 * Gets the number of stations and returns it.
 */
qint16 RouterEngine::Route::stationCount() const
{
    // Number of transfers + arrival & departure station
    return this->transferCount() + 2;
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform for the departure
 * @return const QString departurePlatform
 * @package RouterEngine
 * @public
 * Gets the platform for the departure of the route and returns it.
 */
QString RouterEngine::Route::departurePlatform() const
{
    return this->departureStation()->departure()->platform();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isDeparturePlatformNormal
 * @return const bool isDeparturePlatformNormal
 * @package RouterEngine
 * @public
 * Gets the isDeparturePlatformNormal and returns it.
 */
bool RouterEngine::Route::isDeparturePlatformNormal() const
{
    return this->departureStation()->departure()->isNormalPlatform();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform for the arrival
 * @return const QString arrivalPlatform
 * @package RouterEngine
 * @public
 * Gets the platform for the arrival of the route and returns it.
 */
QString RouterEngine::Route::arrivalPlatform() const
{
    return this->arrivalStation()->arrival()->platform();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isArrivalPlatformNormal
 * @return const bool isArrivalPlatformNormal
 * @package RouterEngine
 * @public
 * Gets the isArrivalPlatformNormal and returns it.
 */
bool RouterEngine::Route::isArrivalPlatformNormal() const
{
    return this->arrivalStation()->arrival()->isNormalPlatform();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the RouterEngine::Transfer departureStation
 * @return RouterEngine::Transfer *departureStation
 * @package RouterEngine
 * @public
 * Gets the departure station RouterEngine::Transfer and returns it.
 */
RouterEngine::Transfer *RouterEngine::Route::departureStation() const
{
    return this->transfers().first();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the RouterEngine::Transfer arrivalStation
 * @return RouterEngine::Transfer *arrivalStation
 * @package RouterEngine
 * @public
 * Gets the arrival station RouterEngine::Transfer and returns it.
 */
RouterEngine::Transfer *RouterEngine::Route::arrivalStation() const
{
    return this->transfers().last();
}

/**
 * @file routerroute.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPartiallyCanceled
 * @return const bool isPartiallyCanceled
 * @package RouterEngine
 * @public
 * Gets the isPartiallyCanceled of the route and returns it.
 */
bool RouterEngine::Route::isPartiallyCanceled() const
{
    foreach(RouteLeg *leg, this->legs()) {
        if(leg->departure()->isCanceled()) {
            return true;
        }
    }
    return false;
}
