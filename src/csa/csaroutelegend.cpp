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

#include "csaroutelegend.h"

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief RouteLegEnd constructor
 * @param const QUrl &uri
 * @param const QDateTime &time
 * @param CSA::Station *station
 * @param const QString &platform
 * @param const bool &isNormalPlatform
 * @param const qint16 &delay
 * @param const bool &isCanceled
 * @param const bool &isPassed
 * @param const CSA::Vehicle::OccupancyLevel &occupancyLevel
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a RouteLeg with a certain leg type, vehicle information and it's departure and arrival legs.
 */
CSA::RouteLegEnd::RouteLegEnd(const QUrl &uri, const QDateTime &time, CSA::Station *station, const QString &platform, const bool &isNormalPlatform, const qint16 &delay, const bool &isCanceled, const bool &isPassed, const CSA::Vehicle::OccupancyLevel &occupancyLevel, QObject *parent) : QObject(parent)
{
    // Clean up when parent is destroyed
    this->setParent(parent);

    // Use private members to avoid signal fire on construction
    m_uri = uri;
    m_time = time;
    m_station = station;
    m_platform = platform;
    m_isNormalPlatform = isNormalPlatform;
    m_delay = delay;
    m_isCanceled = isCanceled;
    m_isPassed = isPassed;
    m_occupancyLevel = occupancyLevel;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @return const QUrl &uri
 * @package CSA
 * @public
 * Gets the URI of the leg end and returns it.
 */
QUrl CSA::RouteLegEnd::uri() const
{
    return m_uri;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the URI
 * @param const QUrl &uri
 * @package CSA
 * @public
 * Sets the URI of the leg end to the given QUrl &uri.
 * Emits the uriChanged signal.
 */
void CSA::RouteLegEnd::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the time
 * @return const QDateTime &time
 * @package CSA
 * @public
 * Gets the time of the leg end and returns it.
 */
QDateTime CSA::RouteLegEnd::time() const
{
    return m_time;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the time
 * @param const QDateTime &uri
 * @package CSA
 * @public
 * Sets the time of the leg end to the given QDateTime &time.
 * Emits the timeChanged signal.
 */
void CSA::RouteLegEnd::setTime(const QDateTime &time)
{
    m_time = time;
    emit this->timeChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station
 * @return const CSA::Station *station
 * @package CSA
 * @public
 * Gets the station of the leg end and returns it.
 */
CSA::Station *CSA::RouteLegEnd::station() const
{
    return m_station;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the station
 * @param CSA::Station *station
 * @package CSA
 * @public
 * Sets the station of the leg end to the given CSA::Station &station.
 * Emits the stationChanged signal.
 */
void CSA::RouteLegEnd::setStation(CSA::Station *station)
{
    m_station = station;
    emit this->stationChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station
 * @return const QString platform
 * @package CSA
 * @public
 * Gets the platform and returns it.
 */
QString CSA::RouteLegEnd::platform() const
{
    return m_platform;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the platform
 * @param const QString &platform
 * @package CSA
 * @public
 * Sets the platform to the given QString &platform.
 * Emits the platformChanged signal.
 */
void CSA::RouteLegEnd::setPlatform(const QString &platform)
{
    m_platform = platform;
    emit this->platformChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isNormalPlatform
 * @return const bool isNormalPlatform
 * @package CSA
 * @public
 * Gets the isNormalPlatform and returns it.
 */
bool CSA::RouteLegEnd::isNormalPlatform() const
{
    return m_isNormalPlatform;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isNormalPlatform
 * @param const bool &isNormalPlatform
 * @package CSA
 * @public
 * Sets the isNormalPlatform to the given bool &isNormalPlatform.
 * Emits the isNormalPlatformChanged signal.
 */
void CSA::RouteLegEnd::setIsNormalPlatform(bool isNormalPlatform)
{
    m_isNormalPlatform = isNormalPlatform;
    emit this->isNormalPlatformChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the delay
 * @return const qint16 delay
 * @package CSA
 * @public
 * Gets the delay and returns it.
 */
qint16 CSA::RouteLegEnd::delay() const
{
    return m_delay;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the delay
 * @param const qint16 &delay
 * @package CSA
 * @public
 * Sets the delay to the given qint16 &delay.
 * Emits the delayChanged signal.
 */
void CSA::RouteLegEnd::setDelay(const qint16 &delay)
{
    m_delay = delay;
    emit this->delayChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isCanceled
 * @return const bool isCanceled
 * @package CSA
 * @public
 * Gets the isCanceled and returns it.
 */
bool CSA::RouteLegEnd::isCanceled() const
{
    return m_isCanceled;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isCanceled
 * @param const bool &isCanceled
 * @package CSA
 * @public
 * Sets the isCanceled to the given bool &isCanceled.
 * Emits the isCanceledChanged signal.
 */
void CSA::RouteLegEnd::setIsCanceled(bool isCanceled)
{
    m_isCanceled = isCanceled;
    emit this->isCanceledChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPassed
 * @return const bool isPassed
 * @package CSA
 * @public
 * Gets the isPassed and returns it.
 */
bool CSA::RouteLegEnd::isPassed() const
{
    return m_isPassed;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isPassed
 * @param const bool &isPassed
 * @package CSA
 * @public
 * Sets the isPassed to the given bool &isPassed.
 * Emits the isPassedChanged signal.
 */
void CSA::RouteLegEnd::setIsPassed(bool isPassed)
{
    m_isPassed = isPassed;
    emit this->isPassedChanged();
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the occupancy level
 * @return const CSA::Vehicle::OccupancyLevel occupancyLevel
 * @package CSA
 * @public
 * Gets the occupancy level of the leg and returns it.
 */
CSA::Vehicle::OccupancyLevel CSA::RouteLegEnd::occupancyLevel() const
{
    return m_occupancyLevel;
}

/**
 * @file csaroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the occupancy level
 * @param const CSA::Vehicle::OccupancyLevel &occupancyLevel
 * @package CSA
 * @public
 * Sets the occunpancy level to the given CSA::Vehicle::OccupancyLevel &occupancyLevel.
 * Emits the occupancyLevelChanged signal.
 */
void CSA::RouteLegEnd::setOccupancyLevel(const CSA::Vehicle::OccupancyLevel &occupancyLevel)
{
    m_occupancyLevel = occupancyLevel;
    emit this->occupancyLevelChanged();
}
