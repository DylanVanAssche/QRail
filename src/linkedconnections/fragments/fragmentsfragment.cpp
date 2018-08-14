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

#include "fragmentsfragment.h"

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Fragment constructor: empty
 * @param QObject *parent = nullptr
 * @package Fragments
 * @public
 * Constructs a Fragment to store the information about
 * a Linked Connection fragment for the Connection Scan Algorithm (CSA).
 */
Fragments::Fragment::Fragment(QObject *parent) : QObject(parent)
{
    // Set parent of this QObject. When parent is destroyed, this one is automatically cleaned up too.
    this->setParent(parent);
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Fragment constructor: arrival and departure
 * @param const QUrl &uri
 * @param const QUrl &departureStationURI
 * @param const QUrl &arrivalStationURI
 * @param const QDateTime &departureTime
 * @param const QDateTime &arrivalTime
 * @param const qint16 &departureDelay
 * @param const qint16 &arrivalDelay
 * @param const QUrl &tripURI
 * @param const QUrl &routeURI
 * @param const QString &direction
 * @param QObject *parent = nullptr
 * @package Fragments
 * @public
 * Constructs a Fragment to store the information about
 * a Linked Connection fragment for the Connection Scan Algorithm (CSA).
 */
Fragments::Fragment::Fragment(
        const QUrl &uri, const QUrl &departureStationURI,
        const QUrl &arrivalStationURI,
        const QDateTime &departureTime,
        const QDateTime &arrivalTime,
        const qint16 &departureDelay,
        const qint16 &arrivalDelay,
        const QUrl &tripURI,
        const QUrl &routeURI,
        const QString &direction,
        QObject *parent
        )
{
    // Set parent of this QObject. When parent is destroyed, this one is automatically cleaned up too.
    this->setParent(parent);

    // Avoid using setters in constructor to reduce signal emitting on instantiation.
    m_uri = uri;
    m_departureStationURI = departureStationURI;
    m_arrivalStationURI = arrivalStationURI;
    m_departureTime = departureTime;
    m_arrivalTime = arrivalTime;
    m_departureDelay = departureDelay;
    m_arrivalDelay = arrivalDelay;
    m_tripURI = tripURI;
    m_routeURI = routeURI;
    m_direction = direction;
}

// Getter & Setters
/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the URI of the linked connection fragment
 * @return const QUrl URI
 * @package Fragments
 * @public
 * Retrieves the URI of the linked connection fragment  and returns it.
 */
QUrl Fragments::Fragment::uri() const
{
    return m_uri;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the URI of the linked connection fragment
 * @param const QUrl &URI
 * @package Fragments
 * @public
 * Sets the URI of the linked connection fragment to the given QUrl &uri.
 * Emits the uriChanged signal.
 */
void Fragments::Fragment::setURI(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the departure station URI of the linked connection fragment
 * @return const QUrl departureStationURI
 * @package Fragments
 * @public
 * Retrieves the departure station URI of the linked connection fragment and returns it.
 */
QUrl Fragments::Fragment::departureStationURI() const
{
    return m_departureStationURI;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the departure station URI of the linked connection fragment
 * @param const QUrl &departureStationURI
 * @package Fragments
 * @public
 * Sets the departure station URI of the linked connection fragment to the given QUrl &departureStationURI.
 * Emits the departureStationURIChanged signal.
 */
void Fragments::Fragment::setDepartureStationURI(const QUrl &departureStationURI)
{
    m_departureStationURI = departureStationURI;
    emit this->departureStationURIChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the arrival station URI of the linked connection fragment
 * @return const QUrl arrivalStationURI
 * @package Fragments
 * @public
 * Retrieves the arrival station URI of the linked connection fragment and returns it.
 */
QUrl Fragments::Fragment::arrivalStationURI() const
{
    return m_arrivalStationURI;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the arrival station URI of the linked connection fragment
 * @param const QUrl &arrivalStationURI
 * @package Fragments
 * @public
 * Sets the arrival station URI of the linked connection fragment to the given QUrl &arrivalStationURI.
 * Emits the arrivalStationURIChanged signal.
 */
void Fragments::Fragment::setArrivalStationURI(const QUrl &arrivalStationURI)
{
    m_arrivalStationURI = arrivalStationURI;
    emit this->arrivalStationURIChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the departure time of the linked connection fragment
 * @return const QDateTime departureTime
 * @package Fragments
 * @public
 * Retrieves the departure time of the linked connection fragment and returns it.
 */
QDateTime Fragments::Fragment::departureTime() const
{
    return m_departureTime;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the departure time of the linked connection fragment
 * @param const QDateTime &departureTime
 * @package Fragments
 * @public
 * Sets the departure time of the linked connection fragment to the given QDateTime &departureTime.
 * Emits the departureTimeChanged signal.
 */
void Fragments::Fragment::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
    emit this->departureTimeChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the arrival time of the linked connection fragment
 * @return const QDateTime arrivalTime
 * @package Fragments
 * @public
 * Retrieves the arrival time of the linked connection fragment and returns it.
 */
QDateTime Fragments::Fragment::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the arrival time of the linked connection fragment
 * @param const QDateTime &arrivalTime
 * @package Fragments
 * @public
 * Sets the arrival time of the linked connection fragment to the given QDateTime &arrivalTime.
 * Emits the arrivalTimeChanged signal.
 */
void Fragments::Fragment::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the departure delay of the linked connection fragment
 * @return const qint16 departureDelay
 * @package Fragments
 * @public
 * Retrieves the departure delay (in seconds) of the linked connection fragment and returns it.
 */
qint16 Fragments::Fragment::departureDelay() const
{
    return m_departureDelay;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the departure delay of the linked connection fragment
 * @param const QDateTime &departureDelay
 * @package Fragments
 * @public
 * Sets the departure delay (in seconds) of the linked connection fragment to the given qint16 &departureDelay.
 * Emits the departureDelayChanged signal.
 */
void Fragments::Fragment::setDepartureDelay(const qint16 &departureDelay)
{
    m_departureDelay = departureDelay;
    emit this->departureDelayChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the arrival delay of the linked connection fragment
 * @return const qint16 arrivalDelay
 * @package Fragments
 * @public
 * Retrieves the arrival delay (in seconds) of the linked connection fragment and returns it.
 */
qint16 Fragments::Fragment::arrivalDelay() const
{
    return m_arrivalDelay;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the arrival delay of the linked connection fragment
 * @param const QDateTime &arrival
 * @package Fragments
 * @public
 * Sets the arrival delay (in seconds) of the linked connection fragment to the given qint16 &arrivalDelay.
 * Emits the arrivalDelayChanged signal.
 */
void Fragments::Fragment::setArrivalDelay(const qint16 &arrivalDelay)
{
    m_arrivalDelay = arrivalDelay;
    emit this->arrivalDelayChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 30 Jul 2018
 * @brief Gets the delayed departure time of the linked connection fragment
 * @return const QDateTime departureDelayedTime
 * @package Fragments
 * @public
 * Retrieves the delayed departure time of the linked connection fragment and returns it.
 */
QDateTime Fragments::Fragment::departureDelayedTime() const
{
    return (this->departureTime().addSecs(this->departureDelay()));
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 30 Jul 2018
 * @brief Gets the delayed arrival time of the linked connection fragment
 * @return const QDateTime arrivalDelayedTime
 * @package Fragments
 * @public
 * Retrieves the delayed arrival time of the linked connection fragment and returns it.
 */
QDateTime Fragments::Fragment::arrivalDelayedTime() const
{
    return (this->arrivalTime().addSecs(this->arrivalDelay()));
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the trip URI of the linked connection fragment
 * @return const QUrl tripURI
 * @package Fragments
 * @public
 * Retrieves the trip URI of the linked connection fragment and returns it.
 */
QUrl Fragments::Fragment::tripURI() const
{
    return m_tripURI;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the trip URI of the linked connection fragment
 * @param const QUrl &tripURI
 * @package Fragments
 * @public
 * Sets the trip URI of the linked connection fragment to the given QUrl &tripURI.
 * Emits the tripURIChanged signal.
 */
void Fragments::Fragment::setTripURI(const QUrl &tripURI)
{
    m_tripURI = tripURI;
    emit this->tripURIChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the route URI of the linked connection fragment
 * @return const QUrl routeURI
 * @package Fragments
 * @public
 * Retrieves the route URI of the linked connection fragment and returns it.
 */
QUrl Fragments::Fragment::routeURI() const
{
    return m_routeURI;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the route URI of the linked connection fragment
 * @param const QUrl &routeURI
 * @package Fragments
 * @public
 * Sets the route URI of the linked connection fragment to the given QUrl &routeURI.
 * Emits the routeURIChanged signal.
 */
void Fragments::Fragment::setRouteURI(const QUrl &routeURI)
{
    m_routeURI = routeURI;
    emit this->routeURIChanged();
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the direction of the linked connection fragment
 * @return const QString direction
 * @package Fragments
 * @public
 * Retrieves the direction of the vehicle in this linked connection fragment and returns it.
 */
QString Fragments::Fragment::direction() const
{
    return m_direction;
}

/**
 * @file fragmentsfragment.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the direction of the linked connection fragment
 * @param const QString &direction
 * @package Fragments
 * @public
 * Sets the direction of the vehicle in this linked connection fragment to the given QString &direction.
 * Emits the directionChanged signal.
 */
void Fragments::Fragment::setDirection(const QString &direction)
{
    m_direction = direction;
    emit this->directionChanged();
}
