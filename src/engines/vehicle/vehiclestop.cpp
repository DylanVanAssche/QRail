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

#include "engines/vehicle/vehiclestop.h"

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: empty
 * @param QObject *parent
 * @package VehicleEngine
 * @public
 * Constructs a VehicleEngine::Stop with the given parent.
 */
VehicleEngine::Stop::Stop(QObject *parent) : QObject(parent)
{

}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: full
 * @param StationEngine::Station *station
 * @param const QString &platform
 * @param const bool &isPlatformNormal
 * @param const bool &hasLeft
 * @param const QDateTime &departureTime
 * @param const qint16 &departureDelay
 * @param const bool &isDepartureCanceled
 * @param const QDateTime &arrivalTime
 * @param const qint16 &arrivalDelay
 * @param const bool &isArrivalCanceled
 * @param const bool &isExtraStop
 * @param const VehicleEngine::Stop::OccupancyLevel &occupancyLevel
 * @param const VehicleEngine::Stop::Type &type
 * @param QObject *parent
 * @package VehicleEngine
 * @public
 * Constructs a VehicleEngine::Stop with the given parent.
 */
VehicleEngine::Stop::Stop(StationEngine::Station *station, const QString &platform, const bool &isPlatformNormal, const bool &hasLeft, const QDateTime &departureTime, const qint16 &departureDelay, const bool &isDepartureCanceled, const QDateTime &arrivalTime, const qint16 &arrivalDelay, const bool &isArrivalCanceled, const bool &isExtraStop, const VehicleEngine::Stop::OccupancyLevel &occupancyLevel, const VehicleEngine::Stop::Type &type, QObject *parent): QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_station = station;
    m_platform = platform;
    m_isPlatformNormal = isPlatformNormal;
    m_hasLeft = hasLeft;
    m_departureTime = departureTime;
    m_departureDelay = departureDelay;
    m_isDepartureCanceled = isDepartureCanceled;
    m_arrivalTime = arrivalTime;
    m_arrivalDelay = arrivalDelay;
    m_isArrivalCanceled = isArrivalCanceled;
    m_isExtraStop = isExtraStop;
    m_occupancyLevel = occupancyLevel;
    m_type = type;
}

// Getters & Setters
/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station for this stop
 * @return StationEngine::Station *station
 * @package VehicleEngine
 * @public
 * Gets the station for this stop and returns it.
 */
StationEngine::Station *VehicleEngine::Stop::station() const
{
    return m_station;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the station for this stop
 * @param StationEngine::Station *station
 * @package VehicleEngine
 * @public
 * Sets the station for this stop to the given StationEngine::Station *station.
 * Emits the stationChanged signal.
 */
void VehicleEngine::Stop::setStation(StationEngine::Station *station)
{
    m_station = station;
    emit this->stationChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platform for this stop
 * @return const QString platform
 * @package VehicleEngine
 * @public
 * Gets the platform for this stop and returns it.
 */
QString VehicleEngine::Stop::platform() const
{
    return m_platform;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the platform for this stop
 * @param const QString &platform
 * @package VehicleEngine
 * @public
 * Sets the platform for this stop to the given QString &platform.
 * Emits the platformChanged signal.
 */
void VehicleEngine::Stop::setPlatform(const QString &platform)
{
    m_platform = platform;
    emit this->platformChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPlatformNormal
 * @return const bool isPlatformNormal
 * @package VehicleEngine
 * @public
 * Gets the isPlatformNormal and returns it.
 */
bool VehicleEngine::Stop::isPlatformNormal() const
{
    return m_isPlatformNormal;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isPlatformNormal
 * @param const bool &isPlatformNormal
 * @package VehicleEngine
 * @public
 * Sets the isPlatformNormal to the given bool &isPlatformNormal.
 * Emits the isPlatformNormalChanged signal.
 */
void VehicleEngine::Stop::setIsPlatformNormal(const bool &isPlatformNormal)
{
    m_isPlatformNormal = isPlatformNormal;
    emit this->isPlatformNormalChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasLeft
 * @return const bool hasLeft
 * @package VehicleEngine
 * @public
 * Gets the hasLeft and returns it.
 */
bool VehicleEngine::Stop::hasLeft() const
{
    return m_hasLeft;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasLeft
 * @param const bool &hasLeft
 * @package VehicleEngine
 * @public
 * Sets the hasLeft to the given bool &hasLeft.
 * Emits the hasLeftChanged signal.
 */
void VehicleEngine::Stop::setHasLeft(const bool &hasLeft)
{
    m_hasLeft = hasLeft;
    emit this->hasLeftChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure time
 * @return const QDateTime departureTime
 * @package VehicleEngine
 * @public
 * Gets the departure time for this stop and returns it.
 */
QDateTime VehicleEngine::Stop::departureTime() const
{
    return m_departureTime;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure time
 * @param const QDateTime &departureTime
 * @package VehicleEngine
 * @public
 * Sets the departure time for this stop to the given QDateTime &departureTime.
 * Emits the departureTimeChanged signal.
 */
void VehicleEngine::Stop::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
    emit this->departureTimeChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure delay
 * @return const qint16 departureDelay
 * @package VehicleEngine
 * @public
 * Gets the departure delay for this stop and returns it.
 */
qint16 VehicleEngine::Stop::departureDelay() const
{
    return m_departureDelay;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure delay
 * @param const qint16 departureDelay
 * @package VehicleEngine
 * @public
 * Sets the departure delay for this stop to the given qint16 departureDelay.
 * Emits the departureDelayChanged signal.
 */
void VehicleEngine::Stop::setDepartureDelay(const qint16 &departureDelay)
{
    m_departureDelay = departureDelay;
    emit this->departureDelayChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isDepartureCanceled
 * @return const bool isDepartureCanceled
 * @package VehicleEngine
 * @public
 * Gets the isDepartureCanceled and returns it.
 */
bool VehicleEngine::Stop::isDepartureCanceled() const
{
    return m_isDepartureCanceled;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isDepartureCanceled
 * @param const bool &isDepartureCanceled
 * @package VehicleEngine
 * @public
 * Sets the isDepartureCanceled for this stop to the given bool &isDepartureCanceled.
 * Emits the isDepartureCanceledChanged signal.
 */
void VehicleEngine::Stop::setIsDepartureCanceled(const bool &isDepartureCanceled)
{
    m_isDepartureCanceled = isDepartureCanceled;
    emit this->isDepartureCanceledChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival time
 * @return const QDateTime arrivalTime
 * @package VehicleEngine
 * @public
 * Gets the arrival time for this stop and returns it.
 */
QDateTime VehicleEngine::Stop::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival time
 * @param const QDateTime &arrivalTime
 * @package VehicleEngine
 * @public
 * Sets the arrival time for this stop to the given QDateTime &arrivalTime.
 * Emits the arrivalTimeChanged signal.
 */
void VehicleEngine::Stop::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival delay
 * @return const qint16 arrivalDelay
 * @package VehicleEngine
 * @public
 * Gets the arrival delay for this stop and returns it.
 */
qint16 VehicleEngine::Stop::arrivalDelay() const
{
    return m_arrivalDelay;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival delay
 * @return const qint16 arrivalDelay
 * @package VehicleEngine
 * @public
 * Gets the arrival delay for this stop and returns it.
 * Emits the arrivalDelayChanged signal.
 */
void VehicleEngine::Stop::setArrivalDelay(const qint16 &arrivalDelay)
{
    m_arrivalDelay = arrivalDelay;
    emit this->arrivalDelayChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isArrivalCanceled
 * @return const bool isArrivalCanceled
 * @package VehicleEngine
 * @public
 * Gets the isArrivalCanceled and returns it.
 */
bool VehicleEngine::Stop::isArrivalCanceled() const
{
    return m_isArrivalCanceled;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isArrivalCanceled
 * @param const bool &isArrivalCanceled
 * @package VehicleEngine
 * @public
 * Sets the isArrivalCanceled for this stop to the given bool &isArrivalCanceled.
 * Emits the isArrivalCanceledChanged signal.
 */
void VehicleEngine::Stop::setIsArrivalCanceled(const bool &isArrivalCanceled)
{
    m_isArrivalCanceled = isArrivalCanceled;
    emit this->isArrivalCanceledChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the occupancy level
 * @return const VehicleEngine::Stop::OccupancyLevel occupancyLevel
 * @package VehicleEngine
 * @public
 * Gets the occupancy level and returns it.
 */
VehicleEngine::Stop::OccupancyLevel VehicleEngine::Stop::occupancyLevel() const
{
    return m_occupancyLevel;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the occupancy level
 * @param const VehicleEngine::Stop::OccupancyLevel &occupancyLevel
 * @package VehicleEngine
 * @public
 * Sets the occupancy level for this stop to the given VehicleEngine::Stop::OccupancyLevel &occupancyLevel.
 * Emits the occupancyLevelChanged signal.
 */
void VehicleEngine::Stop::setOccupancyLevel(const VehicleEngine::Stop::OccupancyLevel &occupancyLevel)
{
    m_occupancyLevel = occupancyLevel;
    emit this->occupancyLevelChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the type
 * @return const VehicleEngine::Stop::Type type
 * @package VehicleEngine
 * @public
 * Gets the type and returns it.
 */
VehicleEngine::Stop::Type VehicleEngine::Stop::type() const
{
    return m_type;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the type
 * @param const VehicleEngine::Stop::Type &type
 * @package VehicleEngine
 * @public
 * Sets the type to the given VehicleEngine::Stop::Type &type
 * Emits the typeChanged signal.
 */
void VehicleEngine::Stop::setType(const VehicleEngine::Stop::Type &type)
{
    m_type = type;
    emit this->typeChanged();
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isExtraStop
 * @return const bool isExtraStop
 * @package VehicleEngine
 * @public
 * Gets the isExtraStop and returns it.
 */
bool VehicleEngine::Stop::isExtraStop() const
{
    return m_isExtraStop;
}

/**
 * @file vehiclestop.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isExtraStop
 * @param const bool &isExtraStop
 * @package VehicleEngine
 * @public
 * Sets the isExtraStop to the given bool &isExtraStop.
 * Emits the isExtraStopChanged signal.
 */
void VehicleEngine::Stop::setIsExtraStop(const bool &isExtraStop)
{
    m_isExtraStop = isExtraStop;
    emit this->isExtraStopChanged();
}
