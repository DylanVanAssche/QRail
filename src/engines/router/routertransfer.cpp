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

#include "engines/router/routertransfer.h"

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief RouterEngine::Transfer constructor
 * @param RouterEngine::RouteLeg *departureLeg
 * @param RouterEngine::RouteLeg *arrivalLeg
 * @param QObject *parent = nullptr
 * @package RouterEngine
 * @public
 * Constructs a RouterEngine::Transfer for the journey extraction.
 */
RouterEngine::Transfer::Transfer(RouterEngine::RouteLeg *departureLeg, RouterEngine::RouteLeg *arrivalLeg, QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_departureLeg = departureLeg;
    m_arrivalLeg = arrivalLeg;

    // When both legs are available, we have a TRANSFER
    if(departureLeg && arrivalLeg) {
        m_type = RouterEngine::Transfer::Type::TRANSFER;
        m_departure = departureLeg->departure();
        m_arrival = arrivalLeg->arrival();
    }
    // Only departure is available, we have a DEPARTURE
    else if(departureLeg) {
        m_type = RouterEngine::Transfer::Type::DEPARTURE;
        m_departure = departureLeg->departure();
        m_arrival = nullptr;
    }
    else if(arrivalLeg) {
        m_type = RouterEngine::Transfer::Type::ARRIVAL;
        m_departure = nullptr;
        m_arrival = arrivalLeg->arrival();
    }
    else {
        m_type = RouterEngine::Transfer::Type::INVALID;
        m_departure = nullptr;
        m_arrival = nullptr;
        qCritical() << "A transfer needs at least a departure or arrival!";
    }
}

// Getters & Setters
/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure leg
 * @return RouterEngine::RouteLeg *departureLeg
 * @package RouterEngine
 * @public
 * Gets the departure leg and returns it.
 */
RouterEngine::RouteLeg *RouterEngine::Transfer::departureLeg() const
{
    return m_departureLeg;
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure leg
 * @param RouterEngine::RouteLeg *departureLeg
 * @package RouterEngine
 * @public
 * Sets the departure leg to the given RouterEngine::RouteLeg *departureLeg.
 * Emits the departureLegChanged signal.
 */
void RouterEngine::Transfer::setDepartureLeg(RouterEngine::RouteLeg *departureLeg)
{
    m_departureLeg = departureLeg;
    emit this->departureLegChanged();
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival leg
 * @return RouterEngine::RouteLeg *arrivalLeg
 * @package RouterEngine
 * @public
 * Gets the arrival leg and returns it.
 */
RouterEngine::RouteLeg *RouterEngine::Transfer::arrivalLeg() const
{
    return m_arrivalLeg;
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival leg
 * @param RouterEngine::RouteLeg *arrivalLeg
 * @package RouterEngine
 * @public
 * Sets the arrival leg to the given RouterEngine::RouteLeg *arrivalLeg.
 * Emits the arrivalLegChanged signal.
 */
void RouterEngine::Transfer::setArrivalLeg(RouterEngine::RouteLeg *arrivalLeg)
{
    m_arrivalLeg = arrivalLeg;
    emit this->arrivalLegChanged();
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure
 * @return RouterEngine::RouteLegEnd *departure
 * @package RouterEngine
 * @public
 * Gets the departure and returns it.
 */
RouterEngine::RouteLegEnd *RouterEngine::Transfer::departure() const
{
    return m_departure;
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure leg
 * @param RouterEngine::RouteLegEnd *departure
 * @package RouterEngine
 * @public
 * Sets the departure to the given RouterEngine::RouteLegEnd *departure.
 * Emits the departureChanged signal.
 */
void RouterEngine::Transfer::setDeparture(RouterEngine::RouteLegEnd *departure)
{
    m_departure = departure;
    emit this->departureChanged();
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival
 * @return RouterEngine::RouteLegEnd *arrival
 * @package RouterEngine
 * @public
 * Gets the arrival and returns it.
 */
RouterEngine::RouteLegEnd *RouterEngine::Transfer::arrival() const
{
    return m_arrival;
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival
 * @param RouterEngine::RouteLegEnd *arrival
 * @package RouterEngine
 * @public
 * Sets the arrival to the given RouterEngine::RouteLegEnd *arrival.
 * Emits the arrivalChanged signal.
 */
void RouterEngine::Transfer::setArrival(RouteLegEnd *arrival)
{
    m_arrival = arrival;
    emit this->arrivalChanged();
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type
 * @return const RouterEngine::Transfer::Type type
 * @package RouterEngine
 * @public
 * Gets the type and returns it.
 */
RouterEngine::Transfer::Type RouterEngine::Transfer::type() const
{
    return m_type;
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type
 * @param const RouterEngine::Transfer::Type &type
 * @package RouterEngine
 * @public
 * Sets the type to the given RouterEngine::Transfer::Type &type.
 * Emits the typeChanged signal.
 */
void RouterEngine::Transfer::setType(const RouterEngine::Transfer::Type &type)
{
    m_type = type;
    emit this->typeChanged();
}

// High level API
/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the uri of the underlying route
 * @return const QUrl uri
 * @package RouterEngine
 * @public
 * Gets the uri of the underlying route and returns it.
 */
QUrl RouterEngine::Transfer::uri() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->uri();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->uri();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return QUrl();
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station of the underlying route
 * @return StationEngine::Station *station
 * @package RouterEngine
 * @public
 * Gets the station of the underlying route and returns it.
 */
StationEngine::Station *RouterEngine::Transfer::station() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->station();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->station();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return StationEngine::NullStation::getInstance();
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the time of the underlying route
 * @return const QDateTime time
 * @package RouterEngine
 * @public
 * Gets the time of the underlying route and returns it.
 */
QDateTime RouterEngine::Transfer::time() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->time();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->time();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return QDateTime();
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the delay of the underlying route
 * @return const qint16 delay
 * @package RouterEngine
 * @public
 * Gets the delay of the underlying route and returns it.
 */
qint16 RouterEngine::Transfer::delay() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->delay();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->delay();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return 0;
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the delayed time of the underlying route
 * @return const QDateTime delayedTime
 * @package RouterEngine
 * @public
 * Gets the time with the delays included of the underlying route and returns it.
 */
QDateTime RouterEngine::Transfer::delayedTime() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->time().addSecs(this->delay());
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->time().addSecs(this->delay());
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return QDateTime();
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform of the underlying route
 * @return const QString platform
 * @package RouterEngine
 * @public
 * Gets the platform of the underlying route and returns it.
 */
QString RouterEngine::Transfer::platform() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->platform();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->platform();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return QString();
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isCanceled of the underlying route
 * @return const bool isCanceled
 * @package RouterEngine
 * @public
 * Gets the isCanceled of the underlying route and returns it.
 */
bool RouterEngine::Transfer::isCanceled() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->isCanceled();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->isCanceled();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return false;
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isNormalPlatform of the underlying route
 * @return const bool isNormalPlatform
 * @package RouterEngine
 * @public
 * Gets the isNormalPlatform of the underlying route and returns it.
 */
bool RouterEngine::Transfer::isNormalPlatform() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->isNormalPlatform();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->isNormalPlatform();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return false;
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPassed of the underlying route
 * @return const bool isPassed
 * @package RouterEngine
 * @public
 * Gets the isPassed of the underlying route and returns it.
 */
bool RouterEngine::Transfer::isPassed() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->isPassed();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->isPassed();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return false;
    }
}

/**
 * @file routertransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the occupancy level of the underlying route
 * @return const VehicleEngine::Stop::OccupancyLevel occupancyLevel
 * @package RouterEngine
 * @public
 * Gets the occupancy level of the underlying route and returns it.
 */
VehicleEngine::Stop::OccupancyLevel RouterEngine::Transfer::occupancyLevel() const
{
    if(this->type() == RouterEngine::Transfer::Type::DEPARTURE || this->type() == RouterEngine::Transfer::Type::TRANSFER) {
        return this->departure()->occupancyLevel();
    }
    else if(this->type() == RouterEngine::Transfer::Type::ARRIVAL) {
        return this->arrival()->occupancyLevel();
    }
    else {
        qCritical() << "RouterEngine::Transfer must have at least an arrival or departure leg!";
        return VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED;
    }
}
