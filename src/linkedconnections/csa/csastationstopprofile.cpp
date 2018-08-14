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

#include "csastationstopprofile.h"

/**
 * @file csastationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::StationStopProfile constructor
 * @param const QDateTime &departureTime
 * @param const QDateTime &arrivalTime
 * @param Fragments::Fragment *departureConnection
 * @param Fragments::Fragment *arrivalConnection
 * @param const qint16 transfers
 * @param QObject *parent = nullptr
 * @package CSA
 * @public
 * Constructs a CSA::StationStopProfile for the S array in the Connection Scan Algorithm.
 */
CSA::StationStopProfile::StationStopProfile(const QDateTime &departureTime, const QDateTime &arrivalTime, Fragments::Fragment *departureConnection, Fragments::Fragment *arrivalConnection, const qint16 transfers, QObject *parent) : QObject(parent)
{
    // Enable auto clean up when parent is destroyed
    this->setParent(parent);

    // Use private members to avoid signal triggering on construction
    m_departureTime = departureTime;
    m_arrivalTime = arrivalTime;
    m_departureConnection = departureConnection;
    m_arrivalConnection = arrivalConnection;
    m_transfers = transfers;
}

// Getters & Setters
/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the departure time
 * @return QDateTime departureTime
 * @public
 * Gets the departure time in this stop.
 */
QDateTime CSA::StationStopProfile::departureTime() const
{
    return m_departureTime;
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the departure time
 * @param const QDateTime &departureTime
 * @public
 * Sets the current departure time to the given QDateTime &departureTime.
 * Emits the departureTimeChanged signal when changed.
 */
void CSA::StationStopProfile::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
    emit this->departureTimeChanged();
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the arrival time
 * @return QDateTime arrivalTime
 * @public
 * Gets the arrival time in this stop.
 */
QDateTime CSA::StationStopProfile::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the arrivelt ime
 * @param const QDateTime &arrivalTime
 * @public
 * Sets the current arrival time to the given QDateTime &arrivalTime.
 * Emits the arrivalTimeChanged signal when changed.
 */
void CSA::StationStopProfile::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the departure connection
 * @return Fragment *departureConnection
 * @public
 * Gets the departure connection in this stop.
 */
Fragments::Fragment *CSA::StationStopProfile::departureConnection() const
{
    return m_departureConnection;
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the departure connection
 * @param const Fragment *departureConnection
 * @public
 * Sets the current departure connection to the given Fragment *departureConnection.
 * Emits the departureConnectionChanged signal when changed.
 */
void CSA::StationStopProfile::setDepartureConnection(Fragments::Fragment *departureConnection)
{
    m_departureConnection = departureConnection;
    emit this->departureConnectionChanged();
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the arrival connection
 * @return Fragment *arrivalConnection
 * @public
 * Gets the arrival connection in this stop.
 */
Fragments::Fragment *CSA::StationStopProfile::arrivalConnection() const
{
    return m_arrivalConnection;
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the arrival connection
 * @param const Fragment *arrivalConnection
 * @public
 * Sets the current arrival connection to the given Fragment *arrivalConnection.
 * Emits the arrivalConnectionChanged signal when changed.
 */
void CSA::StationStopProfile::setArrivalConnection(Fragments::Fragment *arrivalConnection)
{
    m_arrivalConnection = arrivalConnection;
    emit this->arrivalConnectionChanged();
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the number of transfers
 * @return qint16 transfers
 * @public
 * Gets the number of transfers between this stop and the destination.
 */
qint16 CSA::StationStopProfile::transfers() const
{
    return m_transfers;
}

/**
 * @file csastationstopprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the number of transfers
 * @param const qint16 &transfers
 * @public
 * Sets the number of transfers between this stop and the destination to the given qint16 &transfers.
 * Emits the transfersChanged signal when changed.
 */
void CSA::StationStopProfile::setTransfers(const qint16 &transfers)
{
    m_transfers = transfers;
    emit this->transfersChanged();
}
