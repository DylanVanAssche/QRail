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

#include "csavehiclestop.h"

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Station constructor: empty
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::VehicleStop with the given parent.
 */
CSA::VehicleStop::VehicleStop(QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);
}

// Getters & Setters
/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the vehicle information
 * @return CSA::Vehicle *vehicleInformation
 * @package CSA
 * @public
 * Gets the vehicle information and returns it.
 */
CSA::Vehicle *CSA::VehicleStop::vehicleInformation() const
{
    return m_vehicleInformation;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the vehicle information
 * @param CSA::Vehicle *vehicle
 * @package CSA
 * @public
 * Sets the vehicle information to the given CSA::Vehicle *vehicleInformation.
 * Emits the vehicleInformationChanged signal.
 */
void CSA::VehicleStop::setVehicleInformation(CSA::Vehicle *vehicleInformation)
{
    m_vehicleInformation = vehicleInformation;
    emit this->vehicleInformationChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station for this stop
 * @return CSA::Station *station
 * @package CSA
 * @public
 * Gets the station for this stop and returns it.
 */
CSA::Station *CSA::VehicleStop::station() const
{
    return m_station;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the station for this stop
 * @param CSA::Station *station
 * @package CSA
 * @public
 * Sets the station for this stop to the given CSA::Station *station.
 * Emits the stationChanged signal.
 */
void CSA::VehicleStop::setStation(CSA::Station *station)
{
    m_station = station;
    emit this->stationChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform for this stop
 * @return const QString platform
 * @package CSA
 * @public
 * Gets the platform for this stop and returns it.
 */
QString CSA::VehicleStop::platform() const
{
    return m_platform;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the platform for this stop
 * @param const QString &platform
 * @package CSA
 * @public
 * Sets the platform for this stop to the given QString &platform.
 * Emits the platformChanged signal.
 */
void CSA::VehicleStop::setPlatform(const QString &platform)
{
    m_platform = platform;
    emit this->platformChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPlatformNormal
 * @return const bool isPlatformNormal
 * @package CSA
 * @public
 * Gets the isPlatformNormal and returns it.
 */
bool CSA::VehicleStop::isPlatformNormal() const
{
    return m_isPlatformNormal;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isPlatformNormal
 * @param const bool &isPlatformNormal
 * @package CSA
 * @public
 * Sets the isPlatformNormal to the given bool &isPlatformNormal.
 * Emits the isPlatformNormalChanged signal.
 */
void CSA::VehicleStop::setIsPlatformNormal(const bool &isPlatformNormal)
{
    m_isPlatformNormal = isPlatformNormal;
    emit this->isPlatformNormalChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasLeft
 * @return const bool hasLeft
 * @package CSA
 * @public
 * Gets the hasLeft and returns it.
 */
bool CSA::VehicleStop::hasLeft() const
{
    return m_hasLeft;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasLeft
 * @param const bool &hasLeft
 * @package CSA
 * @public
 * Sets the hasLeft to the given bool &hasLeft.
 * Emits the hasLeftChanged signal.
 */
void CSA::VehicleStop::setHasLeft(const bool &hasLeft)
{
    m_hasLeft = hasLeft;
    emit this->hasLeftChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure time
 * @return const QDateTime departureTime
 * @package CSA
 * @public
 * Gets the departure time for this stop and returns it.
 */
QDateTime CSA::VehicleStop::departureTime() const
{
    return m_departureTime;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure time
 * @param const QDateTime &departureTime
 * @package CSA
 * @public
 * Sets the departure time for this stop to the given QDateTime &departureTime.
 * Emits the departureTimeChanged signal.
 */
void CSA::VehicleStop::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
    emit this->departureTimeChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure delay
 * @return const qint16 departureDelay
 * @package CSA
 * @public
 * Gets the departure delay for this stop and returns it.
 */
qint16 CSA::VehicleStop::departureDelay() const
{
    return m_departureDelay;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure delay
 * @param const qint16 departureDelay
 * @package CSA
 * @public
 * Sets the departure delay for this stop to the given qint16 departureDelay.
 * Emits the departureDelayChanged signal.
 */
void CSA::VehicleStop::setDepartureDelay(const qint16 &departureDelay)
{
    m_departureDelay = departureDelay;
    emit this->departureDelayChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isDepartureCanceled
 * @return const bool isDepartureCanceled
 * @package CSA
 * @public
 * Gets the isDepartureCanceled and returns it.
 */
bool CSA::VehicleStop::isDepartureCanceled() const
{
    return m_isDepartureCanceled;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isDepartureCanceled
 * @param const bool &isDepartureCanceled
 * @package CSA
 * @public
 * Sets the isDepartureCanceled for this stop to the given bool &isDepartureCanceled.
 * Emits the isDepartureCanceledChanged signal.
 */
void CSA::VehicleStop::setIsDepartureCanceled(const bool &isDepartureCanceled)
{
    m_isDepartureCanceled = isDepartureCanceled;
    emit this->isDepartureCanceledChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival time
 * @return const QDateTime arrivalTime
 * @package CSA
 * @public
 * Gets the arrival time for this stop and returns it.
 */
QDateTime CSA::VehicleStop::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival time
 * @param const QDateTime &arrivalTime
 * @package CSA
 * @public
 * Sets the arrival time for this stop to the given QDateTime &arrivalTime.
 * Emits the arrivalTimeChanged signal.
 */
void CSA::VehicleStop::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival delay
 * @return const qint16 arrivalDelay
 * @package CSA
 * @public
 * Gets the arrival delay for this stop and returns it.
 */
qint16 CSA::VehicleStop::arrivalDelay() const
{
    return m_arrivalDelay;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival delay
 * @return const qint16 arrivalDelay
 * @package CSA
 * @public
 * Gets the arrival delay for this stop and returns it.
 * Emits the arrivalDelayChanged signal.
 */
void CSA::VehicleStop::setArrivalDelay(const qint16 &arrivalDelay)
{
    m_arrivalDelay = arrivalDelay;
    emit this->arrivalDelayChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isArrivalCanceled
 * @return const bool isArrivalCanceled
 * @package CSA
 * @public
 * Gets the isArrivalCanceled and returns it.
 */
bool CSA::VehicleStop::isArrivalCanceled() const
{
    return m_isArrivalCanceled;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isArrivalCanceled
 * @param const bool &isArrivalCanceled
 * @package CSA
 * @public
 * Sets the isArrivalCanceled for this stop to the given bool &isArrivalCanceled.
 * Emits the isArrivalCanceledChanged signal.
 */
void CSA::VehicleStop::setIsArrivalCanceled(const bool &isArrivalCanceled)
{
    m_isArrivalCanceled = isArrivalCanceled;
    emit this->isArrivalCanceledChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure URI
 * @return const QUrl departureURI
 * @package CSA
 * @public
 * Gets the departureURI and returns it.
 */
QUrl CSA::VehicleStop::departureURI() const
{
    return m_departureURI;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure URI
 * @param const QUrl &departureURI
 * @package CSA
 * @public
 * Sets the departure URI for this stop to the given QUrl &departureURI.
 * Emits the departureURIChanged signal.
 */
void CSA::VehicleStop::setDepartureURI(const QUrl &departureURI)
{
    m_departureURI = departureURI;
    emit this->departureURIChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the occupancy level
 * @return const CSA::Vehicle::OccupancyLevel occupancyLevel
 * @package CSA
 * @public
 * Gets the occupancy level and returns it.
 */
CSA::Vehicle::OccupancyLevel CSA::VehicleStop::occupancyLevel() const
{
    return m_occupancyLevel;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the occupancy level
 * @param const CSA::Vehicle::OccupancyLevel &occupancyLevel
 * @package CSA
 * @public
 * Sets the occupancy level for this stop to the given CSA::Vehicle::OccupancyLevel &occupancyLevel.
 * Emits the occupancyLevelChanged signal.
 */
void CSA::VehicleStop::setOccupancyLevel(const CSA::Vehicle::OccupancyLevel &occupancyLevel)
{
    m_occupancyLevel = occupancyLevel;
    emit this->occupancyLevelChanged();
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type
 * @return const CSA::VehicleStop::Type type
 * @package CSA
 * @public
 * Gets the type and returns it.
 */
CSA::VehicleStop::Type CSA::VehicleStop::type() const
{
    return m_type;
}

/**
 * @file csavehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type
 * @param const CSA::VehicleStop::Type &type
 * @package CSA
 * @public
 * Sets the type to the given CSA::VehicleStop::Type &type
 * Emits the typeChanged signal.
 */
void CSA::VehicleStop::setType(const CSA::VehicleStop::Type &type)
{
    m_type = type;
    emit this->typeChanged();
}
