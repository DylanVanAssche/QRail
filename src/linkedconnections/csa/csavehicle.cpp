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

#include "csavehicle.h"

/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Vehicle constructor
 * @param const QUrl &uri
 * @param const QUrl &id
 * @param const QString &headsign
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::Vehicle to store information about the vehicle itself.
 */
CSA::Vehicle::Vehicle(const QUrl &uri, const QUrl &id, const QString &headsign, QObject *parent) : QObject(parent)
{
    // Clean up when parent is destroyed
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_uri = uri;
    m_id = id;
    m_headsign = headsign;
}

// Getters & Setters
/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI of the vehicle
 * @return const QUrl uri
 * @package CSA
 * @public
 * Gets the URI of the vehicle and returns it.
 */
QUrl CSA::Vehicle::uri() const
{
    return m_uri;
}

/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the URI of the vehicle
 * @param const QUrl &uri
 * @package CSA
 * @public
 * Sets the URI of the vehicle to the given QUrl &uri
 * Emit the uriChanged signal.
 */
void CSA::Vehicle::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the ID of the vehicle
 * @return const QUrl id
 * @package CSA
 * @public
 * Gets the ID of the vehicle and returns it.
 */
QUrl CSA::Vehicle::id() const
{
    return m_id;
}

/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the ID of the vehicle
 * @param const QUrl &id
 * @package CSA
 * @public
 * Sets the ID of the vehicle to the given QUrl &id
 * Emit the idChanged signal.
 */
void CSA::Vehicle::setId(const QUrl &id)
{
    m_id = id;
    emit this->idChanged();
}

/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the headsign of the vehicle
 * @return const QString headsign
 * @package CSA
 * @public
 * Gets the headsign of the vehicle and returns it.
 */
QString CSA::Vehicle::headsign() const
{
    return m_headsign;
}

/**
 * @file csavehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the headsign of the vehicle
 * @param const QString &headsign
 * @package CSA
 * @public
 * Sets the headsign of the vehicle to the given QString &headsign
 * Emit the headsignChanged signal.
 */
void CSA::Vehicle::setHeadsign(const QString &headsign)
{
    m_headsign = headsign;
    emit this->headsignChanged();
}
