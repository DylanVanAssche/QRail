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

#include "csarouteleg.h"

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief RouteLeg constructor
 * @param const CSA::RouteLeg::Type &type
 * @param CSA::Vehicle *vehicleInformation
 * @param CSA::RouteLegEnd *departure
 * @param CSA::RouteLegEnd *arrival
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a RouteLeg with a certain leg type, vehicle information and it's departure and arrival legs.
 */
CSA::RouteLeg::RouteLeg(const CSA::RouteLeg::Type &type, CSA::Vehicle *vehicleInformation, CSA::RouteLegEnd *departure, CSA::RouteLegEnd *arrival, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_type = type;
    m_vehicleInformation = vehicleInformation;
    m_intermediaryStops = QList<CSA::VehicleStop *>();
    m_departure = departure;
    m_arrival = arrival;
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type of the leg
 * @return const CSA::RouteLeg::Type type
 * @package CSA
 * @public
 * Gets the type of the leg and returns it.
 */
CSA::RouteLeg::Type CSA::RouteLeg::type() const
{
    return m_type;
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const CSA::RouteLeg::Type &type
 * @package CSA
 * @public
 * Sets the type of route to the given CSA::RouteLeg::Type &type.
 * Emits the typeChanged signal.
 */
void CSA::RouteLeg::setType(const CSA::RouteLeg::Type &type)
{
    m_type = type;
    emit this->typeChanged();
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the vehicle information
 * @return const CSA::Vehicle *vehicle
 * @package CSA
 * @public
 * Gets the vehicle information and returns it.
 */
CSA::Vehicle *CSA::RouteLeg::vehicleInformation() const
{
    return m_vehicleInformation;
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the vehicleInformation
 * @param const CSA::Vehicle *vehicleInformation
 * @package CSA
 * @public
 * Sets the type of route to the given CSA::Vehicle *vehicleInformation.
 * Emits the vehicleInformationChanged signal.
 */
void CSA::RouteLeg::setVehicleInformation(CSA::Vehicle *vehicleInformation)
{
    m_vehicleInformation = vehicleInformation;
    emit this->vehicleInformationChanged();
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the intermediary stops
 * @return const QList<CSA::VehicleStop *> intermediaryStops
 * @package CSA
 * @public
 * Gets the intermediary stops and returns it.
 */
QList<CSA::VehicleStop *> CSA::RouteLeg::intermediaryStops() const
{
    return m_intermediaryStops;
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const CSA::RouteLeg::Type &type
 * @package CSA
 * @public
 * Sets the type of route to the given CSA::RouteLeg::Type &type.
 * Emits the intermediaryStopsChanged signal.
 */
void CSA::RouteLeg::setIntermediaryStops(const QList<CSA::VehicleStop *> &intermediaryStops)
{
    m_intermediaryStops = intermediaryStops;
    emit this->intermediaryStopsChanged();
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the deparature leg end.
 * @return const CSA::RouteLegEnd *departure
 * @package CSA
 * @public
 * Gets the departure leg end and returns it.
 */
CSA::RouteLegEnd *CSA::RouteLeg::departure() const
{
    return m_departure;
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const CSA::RouteLeg *departure
 * @package CSA
 * @public
 * Sets the departure leg end to the given CSA::RouteLeg *departure.
 * Emits the departureChanged signal.
 */
void CSA::RouteLeg::setDeparture(CSA::RouteLegEnd *departure)
{
    m_departure = departure;
    emit this->departureChanged();
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival leg end.
 * @return const CSA::RouteLegEnd *arrival
 * @package CSA
 * @public
 * Gets the arrival leg end and returns it.
 */
CSA::RouteLegEnd *CSA::RouteLeg::arrival() const
{
    return m_arrival;
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const CSA::RouteLegEnd *arrival
 * @package CSA
 * @public
 * Sets the arrival to the given CSA::RouteLegEnd *arrival.
 * Emits the arrivalChanged signal.
 */
void CSA::RouteLeg::setArrival(CSA::RouteLegEnd *arrival)
{
    m_arrival = arrival;
    emit this->arrivalChanged();
}

/**
 * @file csarouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station
 * @return const CSA::Station *station
 * @package CSA
 * @public
 * Gets the stations for the route and returns it.
 * Returns a CSA::NullStation instance if the station can't be determined.
 */
CSA::Station *CSA::RouteLeg::station() const
{
    if(this->departure()) {
        return this->departure()->station();
    }
    else if(this->arrival()) {
        return this->arrival()->station();
    }
    else {
        return CSA::NullStation::getInstance();
    }
}
