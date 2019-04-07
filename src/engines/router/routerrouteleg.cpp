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
#include "engines/router/routerrouteleg.h"
using namespace QRail;

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief RouteLeg constructor
 * @param const QRail::RouterEngine::RouteLeg::Type &type
 * @param QRail::VehicleEngine::Vehicle *vehicleInformation
 * @param QRail::RouterEngine::RouteLegEnd *departure
 * @param QRail::RouterEngine::RouteLegEnd *arrival
 * @param QObject *parent
 * @package RouterEngine
 * @public
 * Constructs a RouteLeg with a certain leg type, vehicle information and it's departure and arrival legs.
 */
QRail::RouterEngine::RouteLeg::RouteLeg(const QRail::RouterEngine::RouteLeg::Type &type,
                                        QRail::VehicleEngine::Vehicle *vehicleInformation,
                                        QRail::RouterEngine::RouteLegEnd *departure,
                                        QRail::RouterEngine::RouteLegEnd *arrival,
                                        QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_type = type;
    m_vehicleInformation = vehicleInformation;
    m_departure = departure;
    m_arrival = arrival;
}

RouterEngine::RouteLeg::~RouteLeg()
{
    if(m_vehicleInformation) {
        m_vehicleInformation->deleteLater();
    }

    if(m_departure) {
        m_departure->deleteLater();
    }

    if(m_arrival) {
        m_arrival->deleteLater();
    }
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type of the leg
 * @return const QRail::RouterEngine::RouteLeg::Type type
 * @package RouterEngine
 * @public
 * Gets the type of the leg and returns it.
 */
QRail::RouterEngine::RouteLeg::Type QRail::RouterEngine::RouteLeg::type() const
{
    return m_type;
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const QRail::RouterEngine::RouteLeg::Type &type
 * @package RouterEngine
 * @public
 * Sets the type of route to the given QRail::RouterEngine::RouteLeg::Type &type.
 * Emits the typeChanged signal.
 */
void QRail::RouterEngine::RouteLeg::setType(const QRail::RouterEngine::RouteLeg::Type &type)
{
    m_type = type;
    emit this->typeChanged();
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the vehicle information
 * @return const QRail::VehicleEngine::Vehicle *vehicle
 * @package RouterEngine
 * @public
 * Gets the vehicle information and returns it.
 */
QRail::VehicleEngine::Vehicle *QRail::RouterEngine::RouteLeg::vehicleInformation() const
{
    return m_vehicleInformation;
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the vehicleInformation
 * @param const QRail::VehicleEngine::Vehicle *vehicleInformation
 * @package RouterEngine
 * @public
 * Sets the type of route to the given QRail::VehicleEngine::Vehicle *vehicleInformation.
 * Emits the vehicleInformationChanged signal.
 */
void QRail::RouterEngine::RouteLeg::setVehicleInformation(QRail::VehicleEngine::Vehicle
                                                          *vehicleInformation)
{
    m_vehicleInformation = vehicleInformation;
    emit this->vehicleInformationChanged();
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the deparature leg end.
 * @return const QRail::RouterEngine::RouteLegEnd *departure
 * @package RouterEngine
 * @public
 * Gets the departure leg end and returns it.
 */
QRail::RouterEngine::RouteLegEnd *QRail::RouterEngine::RouteLeg::departure() const
{
    return m_departure;
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const QRail::RouterEngine::RouteLeg *departure
 * @package RouterEngine
 * @public
 * Sets the departure leg end to the given QRail::RouterEngine::RouteLeg *departure.
 * Emits the departureChanged signal.
 */
void QRail::RouterEngine::RouteLeg::setDeparture(QRail::RouterEngine::RouteLegEnd *departure)
{
    m_departure = departure;
    emit this->departureChanged();
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival leg end.
 * @return const QRail::RouterEngine::RouteLegEnd *arrival
 * @package RouterEngine
 * @public
 * Gets the arrival leg end and returns it.
 */
QRail::RouterEngine::RouteLegEnd *QRail::RouterEngine::RouteLeg::arrival() const
{
    return m_arrival;
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type of the leg
 * @param const QRail::RouterEngine::RouteLegEnd *arrival
 * @package RouterEngine
 * @public
 * Sets the arrival to the given QRail::RouterEngine::RouteLegEnd *arrival.
 * Emits the arrivalChanged signal.
 */
void QRail::RouterEngine::RouteLeg::setArrival(QRail::RouterEngine::RouteLegEnd *arrival)
{
    m_arrival = arrival;
    emit this->arrivalChanged();
}

/**
 * @file routerrouteleg.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station
 * @return const StationEngine::Station *station
 * @package RouterEngine
 * @public
 * Gets the stations for the route and returns it.
 * Returns a StationEngine::NullStation instance if the station can't be determined.
 */
StationEngine::Station *QRail::RouterEngine::RouteLeg::station() const
{
    if (this->departure()) {
        return this->departure()->station();
    } else if (this->arrival()) {
        return this->arrival()->station();
    } else {
        return StationEngine::NullStation::getInstance();
    }
}
