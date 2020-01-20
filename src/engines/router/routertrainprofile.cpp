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
#include "engines/router/routertrainprofile.h"
using namespace QRail;

/**
 * @file routerstationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief QRail::RouterEngine::TrainProfile constructor
 * @param const QDateTime &arrivalTime
 * @param QRail::Fragments::Fragment *arrivalConnection
 * @param const qint16 transfers
 * @param QObject *parent = nullptr
 * @package RouterEngine
 * @public
 * Constructs a QRail::RouterEngine::TrainProfile for the T array in the Connection Scan Algorithm.
 */
QRail::RouterEngine::TrainProfile::TrainProfile(const QDateTime &arrivalTime,
                                                QSharedPointer<QRail::Fragments::Fragment> arrivalConnection,
                                                const qint16 transfers,
                                                QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal trigging on construction
    this->setArrivalTime(arrivalTime);
    this->setArrivalConnection(arrivalConnection);
    this->setTransfers(transfers);
}

// Getters & Setters
/**
 * @file routertrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the arrival time
 * @return QDateTime arrivalTime
 * @public
 * Gets the arrival time at the final destination.
 */
QDateTime QRail::RouterEngine::TrainProfile::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file routertrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the arrival time
 * @param const QDateTime &arrivalTime
 * @public
 * Sets the arrival time at the final destination to the given QDateTime &arrivalTime.
 */
void QRail::RouterEngine::TrainProfile::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

/**
 * @file routertrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the arrival connection
 * @return Fragment *arrivalConnection
 * @public
 * Gets the arrival connection for the next transfer or arrival.
 */
QSharedPointer<QRail::Fragments::Fragment> QRail::RouterEngine::TrainProfile::arrivalConnection() const
{
    return m_arrivalConnection;
}

/**
 * @file routertrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the arrival connection
 * @param Fragment *arrivalConnection
 * @public
 * Sets the arrival connection for the next transfer or arrival to the given Fragment *arrivalConnection.
 */
void QRail::RouterEngine::TrainProfile::setArrivalConnection(QSharedPointer<QRail::Fragments::Fragment> arrivalConnection)
{
    m_arrivalConnection = arrivalConnection;
    emit this->arrivalConnectionChanged();
}

/**
 * @file routertrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the number of transfers
 * @return qint16 transfers
 * @public
 * Gets the number of transfers until the destination when hopping on this train
 */
qint16 QRail::RouterEngine::TrainProfile::transfers() const
{
    return m_transfers;
}

/**
 * @file routertrainprofile.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Sets the number of transfers
 * @return qint16 transfers
 * @public
 * Sets the number of transfers until the destination when hopping on this train to the given qint16 &transfers.
 */
void QRail::RouterEngine::TrainProfile::setTransfers(const qint16 &transfers)
{
    m_transfers = transfers;
    emit this->transfersChanged();
}
