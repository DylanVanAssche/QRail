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
#include "engines/router/routerroutelegend.h"
using namespace QRail;

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief RouteLegEnd constructor
 * @param const QUrl &uri
 * @param const QDateTime &time
 * @param StationEngine::Station *station
 * @param const QString &platform
 * @param const bool &isNormalPlatform
 * @param const qint16 &delay
 * @param const bool &isCanceled
 * @param const bool &isPassed
 * @param const QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel
 * @param QObject *parent
 * @package RouterEngine
 * @public
 * Constructs a RouteLeg with a certain leg type, vehicle information and it's departure and arrival legs.
 */
QRail::RouterEngine::RouteLegEnd::RouteLegEnd(const QUrl &uri,
                                              const QDateTime &time,
                                              StationEngine::Station *station,
                                              const QString &platform,
                                              const bool &isNormalPlatform,
                                              const qint16 &delay,
                                              const bool &isCanceled,
                                              const bool &isPassed,
                                              const QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel,
                                              QObject *parent) : QObject(parent)
{
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
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @return const QUrl &uri
 * @package RouterEngine
 * @public
 * Gets the URI of the leg end and returns it.
 */
QUrl QRail::RouterEngine::RouteLegEnd::uri() const
{
    return m_uri;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the URI
 * @param const QUrl &uri
 * @package RouterEngine
 * @public
 * Sets the URI of the leg end to the given QUrl &uri.
 * Emits the uriChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the time
 * @return const QDateTime &time
 * @package RouterEngine
 * @public
 * Gets the time of the leg end and returns it.
 */
QDateTime QRail::RouterEngine::RouteLegEnd::time() const
{
    return m_time;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the time
 * @param const QDateTime &uri
 * @package RouterEngine
 * @public
 * Sets the time of the leg end to the given QDateTime &time.
 * Emits the timeChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setTime(const QDateTime &time)
{
    m_time = time;
    emit this->timeChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station
 * @return const StationEngine::Station *station
 * @package RouterEngine
 * @public
 * Gets the station of the leg end and returns it.
 */
StationEngine::Station *QRail::RouterEngine::RouteLegEnd::station() const
{
    return m_station;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the station
 * @param StationEngine::Station *station
 * @package RouterEngine
 * @public
 * Sets the station of the leg end to the given StationEngine::Station &station.
 * Emits the stationChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setStation(StationEngine::Station *station)
{
    m_station = station;
    emit this->stationChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the station
 * @return const QString platform
 * @package RouterEngine
 * @public
 * Gets the platform and returns it.
 */
QString QRail::RouterEngine::RouteLegEnd::platform() const
{
    return m_platform;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the platform
 * @param const QString &platform
 * @package RouterEngine
 * @public
 * Sets the platform to the given QString &platform.
 * Emits the platformChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setPlatform(const QString &platform)
{
    m_platform = platform;
    emit this->platformChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isNormalPlatform
 * @return const bool isNormalPlatform
 * @package RouterEngine
 * @public
 * Gets the isNormalPlatform and returns it.
 */
bool QRail::RouterEngine::RouteLegEnd::isNormalPlatform() const
{
    return m_isNormalPlatform;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isNormalPlatform
 * @param const bool &isNormalPlatform
 * @package RouterEngine
 * @public
 * Sets the isNormalPlatform to the given bool &isNormalPlatform.
 * Emits the isNormalPlatformChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setIsNormalPlatform(bool isNormalPlatform)
{
    m_isNormalPlatform = isNormalPlatform;
    emit this->isNormalPlatformChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the delay
 * @return const qint16 delay
 * @package RouterEngine
 * @public
 * Gets the delay and returns it.
 */
qint16 QRail::RouterEngine::RouteLegEnd::delay() const
{
    return m_delay;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the delay
 * @param const qint16 &delay
 * @package RouterEngine
 * @public
 * Sets the delay to the given qint16 &delay.
 * Emits the delayChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setDelay(const qint16 &delay)
{
    m_delay = delay;
    emit this->delayChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isCanceled
 * @return const bool isCanceled
 * @package RouterEngine
 * @public
 * Gets the isCanceled and returns it.
 */
bool QRail::RouterEngine::RouteLegEnd::isCanceled() const
{
    return m_isCanceled;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isCanceled
 * @param const bool &isCanceled
 * @package RouterEngine
 * @public
 * Sets the isCanceled to the given bool &isCanceled.
 * Emits the isCanceledChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setIsCanceled(bool isCanceled)
{
    m_isCanceled = isCanceled;
    emit this->isCanceledChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the isPassed
 * @return const bool isPassed
 * @package RouterEngine
 * @public
 * Gets the isPassed and returns it.
 */
bool QRail::RouterEngine::RouteLegEnd::isPassed() const
{
    return m_isPassed;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the isPassed
 * @param const bool &isPassed
 * @package RouterEngine
 * @public
 * Sets the isPassed to the given bool &isPassed.
 * Emits the isPassedChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setIsPassed(bool isPassed)
{
    m_isPassed = isPassed;
    emit this->isPassedChanged();
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the occupancy level
 * @return const QRail::VehicleEngine::Stop::OccupancyLevel occupancyLevel
 * @package RouterEngine
 * @public
 * Gets the occupancy level of the leg and returns it.
 */
QRail::VehicleEngine::Stop::OccupancyLevel QRail::RouterEngine::RouteLegEnd::occupancyLevel() const
{
    return m_occupancyLevel;
}

/**
 * @file routerroutelegend.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the occupancy level
 * @param const QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel
 * @package RouterEngine
 * @public
 * Sets the occunpancy level to the given QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel.
 * Emits the occupancyLevelChanged signal.
 */
void QRail::RouterEngine::RouteLegEnd::setOccupancyLevel(const
                                                         QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel)
{
    m_occupancyLevel = occupancyLevel;
    emit this->occupancyLevelChanged();
}
