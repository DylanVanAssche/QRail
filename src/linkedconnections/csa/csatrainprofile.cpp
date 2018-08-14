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

#include "csatrainprofile.h"

/**
 * @file csastationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::TrainProfile constructor
 * @param const QDateTime &arrivalTime
 * @param Fragments::Fragment *arrivalConnection
 * @param const qint16 transfers
 * @param QObject *parent = nullptr
 * @package CSA
 * @public
 * Constructs a CSA::TrainProfile for the T array in the Connection Scan Algorithm.
 */
CSA::TrainProfile::TrainProfile(const QDateTime &arrivalTime, Fragments::Fragment *arrivalConnection, const qint16 transfers, QObject *parent) : QObject(parent)
{
    // Enable clean up when parent is destroyed
    this->setParent(parent);

    // Use private members to avoid signal trigging on construction
    this->setArrivalTime(arrivalTime);
    this->setArrivalConnection(arrivalConnection);
    this->setTransfers(transfers);
}

// Getters & Setters
/**
 * @file csatrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the arrival time
 * @return QDateTime arrivalTime
 * @public
 * Gets the arrival time at the final destination.
 */
QDateTime CSA::TrainProfile::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file csatrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the arrival time
 * @param const QDateTime &arrivalTime
 * @public
 * Sets the arrival time at the final destination to the given QDateTime &arrivalTime.
 */
void CSA::TrainProfile::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

/**
 * @file csatrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the arrival connection
 * @return Fragment *arrivalConnection
 * @public
 * Gets the arrival connection for the next transfer or arrival.
 */
Fragments::Fragment *CSA::TrainProfile::arrivalConnection() const
{
    return m_arrivalConnection;
}

/**
 * @file csatrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the arrival connection
 * @param Fragment *arrivalConnection
 * @public
 * Sets the arrival connection for the next transfer or arrival to the given Fragment *arrivalConnection.
 */
void CSA::TrainProfile::setArrivalConnection(Fragments::Fragment *arrivalConnection)
{
    m_arrivalConnection = arrivalConnection;
    emit this->arrivalConnectionChanged();
}

/**
 * @file csatrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the number of transfers
 * @return qint16 transfers
 * @public
 * Gets the number of transfers until the destination when hopping on this train
 */
qint16 CSA::TrainProfile::transfers() const
{
    return m_transfers;
}

/**
 * @file csatrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the number of transfers
 * @return qint16 transfers
 * @public
 * Sets the number of transfers until the destination when hopping on this train to the given qint16 &transfers.
 */
void CSA::TrainProfile::setTransfers(const qint16 &transfers)
{
    m_transfers = transfers;
    emit this->transfersChanged();
}
