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

#include "csatransfer.h"

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Transfer constructor
 * @param CSA::RouteLeg *departureLeg
 * @param CSA::RouteLeg *arrivalLeg
 * @param QObject *parent = nullptr
 * @package CSA
 * @public
 * Constructs a CSA::Transfer for the journey extraction.
 */
CSA::Transfer::Transfer(CSA::RouteLeg *departureLeg, CSA::RouteLeg *arrivalLeg, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_departureLeg = departureLeg;
    m_arrivalLeg = arrivalLeg;

    // When both legs are available, we have a TRANSFER
    if(departureLeg && arrivalLeg) {
        m_type = CSA::Transfer::Type::TRANSFER;
        m_departure = departureLeg->departure();
        m_arrival = arrivalLeg->arrival();
    }
    // Only departure is available, we have a DEPARTURE
    else if(departureLeg) {
        m_type = CSA::Transfer::Type::DEPARTURE;
        m_departure = departureLeg->departure();
        m_arrival = nullptr;
    }
    else if(arrivalLeg) {
        m_type = CSA::Transfer::Type::ARRIVAL;
        m_departure = nullptr;
        m_arrival = arrivalLeg->arrival();
    }
    else {
        m_type = CSA::Transfer::Type::INVALID;
        m_departure = nullptr;
        m_arrival = nullptr;
        qCritical() << "A transfer needs at least a departure or arrival!";
    }
}

// Getters & Setters
/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure leg
 * @return CSA::RouteLeg *departureLeg
 * @package CSA
 * @public
 * Gets the departure leg and returns it.
 */
CSA::RouteLeg *CSA::Transfer::departureLeg() const
{
    return m_departureLeg;
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure leg
 * @param CSA::RouteLeg *departureLeg
 * @package CSA
 * @public
 * Sets the departure leg to the given CSA::RouteLeg *departureLeg.
 * Emits the departureLegChanged signal.
 */
void CSA::Transfer::setDepartureLeg(CSA::RouteLeg *departureLeg)
{
    m_departureLeg = departureLeg;
    emit this->departureLegChanged();
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival leg
 * @return CSA::RouteLeg *arrivalLeg
 * @package CSA
 * @public
 * Gets the arrival leg and returns it.
 */
CSA::RouteLeg *CSA::Transfer::arrivalLeg() const
{
    return m_arrivalLeg;
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival leg
 * @param CSA::RouteLeg *arrivalLeg
 * @package CSA
 * @public
 * Sets the arrival leg to the given CSA::RouteLeg *arrivalLeg.
 * Emits the arrivalLegChanged signal.
 */
void CSA::Transfer::setArrivalLeg(CSA::RouteLeg *arrivalLeg)
{
    m_arrivalLeg = arrivalLeg;
    emit this->arrivalLegChanged();
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure
 * @return CSA::RouteLegEnd *departure
 * @package CSA
 * @public
 * Gets the departure and returns it.
 */
CSA::RouteLegEnd *CSA::Transfer::departure() const
{
    return m_departure;
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure leg
 * @param CSA::RouteLegEnd *departure
 * @package CSA
 * @public
 * Sets the departure to the given CSA::RouteLegEnd *departure.
 * Emits the departureChanged signal.
 */
void CSA::Transfer::setDeparture(CSA::RouteLegEnd *departure)
{
    m_departure = departure;
    emit this->departureChanged();
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival
 * @return CSA::RouteLegEnd *arrival
 * @package CSA
 * @public
 * Gets the arrival and returns it.
 */
CSA::RouteLegEnd *CSA::Transfer::arrival() const
{
    return m_arrival;
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival
 * @param CSA::RouteLegEnd *arrival
 * @package CSA
 * @public
 * Sets the arrival to the given CSA::RouteLegEnd *arrival.
 * Emits the arrivalChanged signal.
 */
void CSA::Transfer::setArrival(RouteLegEnd *arrival)
{
    m_arrival = arrival;
    emit this->arrivalChanged();
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type
 * @return const CSA::Transfer::Type type
 * @package CSA
 * @public
 * Gets the type and returns it.
 */
CSA::Transfer::Type CSA::Transfer::type() const
{
    return m_type;
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type
 * @param const CSA::Transfer::Type &type
 * @package CSA
 * @public
 * Sets the type to the given CSA::Transfer::Type &type.
 * Emits the typeChanged signal.
 */
void CSA::Transfer::setType(const CSA::Transfer::Type &type)
{
    m_type = type;
    emit this->typeChanged();
}

// High level API
/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the uri of the underlying route
 * @return const QUrl uri
 * @package CSA
 * @public
 * Gets the uri of the underlying route and returns it.
 */
QUrl CSA::Transfer::uri() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->uri();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->uri();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return QUrl();
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station of the underlying route
 * @return CSA::Station *station
 * @package CSA
 * @public
 * Gets the station of the underlying route and returns it.
 */
CSA::Station *CSA::Transfer::station() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->station();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->station();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return CSA::NullStation::getInstance();
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the time of the underlying route
 * @return const QDateTime time
 * @package CSA
 * @public
 * Gets the time of the underlying route and returns it.
 */
QDateTime CSA::Transfer::time() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->time();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->time();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return QDateTime();
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the delay of the underlying route
 * @return const qint16 delay
 * @package CSA
 * @public
 * Gets the delay of the underlying route and returns it.
 */
qint16 CSA::Transfer::delay() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->delay();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->delay();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return 0;
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the delayed time of the underlying route
 * @return const QDateTime delayedTime
 * @package CSA
 * @public
 * Gets the time with the delays included of the underlying route and returns it.
 */
QDateTime CSA::Transfer::delayedTime() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->time().addSecs(this->delay());
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->time().addSecs(this->delay());
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return QDateTime();
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform of the underlying route
 * @return const QString platform
 * @package CSA
 * @public
 * Gets the platform of the underlying route and returns it.
 */
QString CSA::Transfer::platform() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->platform();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->platform();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return QString();
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isCanceled of the underlying route
 * @return const bool isCanceled
 * @package CSA
 * @public
 * Gets the isCanceled of the underlying route and returns it.
 */
bool CSA::Transfer::isCanceled() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->isCanceled();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->isCanceled();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return false;
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isNormalPlatform of the underlying route
 * @return const bool isNormalPlatform
 * @package CSA
 * @public
 * Gets the isNormalPlatform of the underlying route and returns it.
 */
bool CSA::Transfer::isNormalPlatform() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->isNormalPlatform();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->isNormalPlatform();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return false;
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPassed of the underlying route
 * @return const bool isPassed
 * @package CSA
 * @public
 * Gets the isPassed of the underlying route and returns it.
 */
bool CSA::Transfer::isPassed() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->isPassed();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->isPassed();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return false;
    }
}

/**
 * @file csatransfer.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the occupancy level of the underlying route
 * @return const CSA::Vehicle::OccupancyLevel occupancyLevel
 * @package CSA
 * @public
 * Gets the occupancy level of the underlying route and returns it.
 */
CSA::Vehicle::OccupancyLevel CSA::Transfer::occupancyLevel() const
{
    if(this->type() == CSA::Transfer::Type::DEPARTURE || this->type() == CSA::Transfer::Type::TRANSFER) {
        return this->departure()->occupancyLevel();
    }
    else if(this->type() == CSA::Transfer::Type::ARRIVAL) {
        return this->arrival()->occupancyLevel();
    }
    else {
        qCritical() << "CSA::Transfer must have at least an arrival or departure leg!";
        return CSA::Vehicle::OccupancyLevel::UNSUPPORTED;
    }
}
