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
#include "engines/liveboard/liveboardnullboard.h"
using namespace QRail;
LiveboardEngine::NullBoard *LiveboardEngine::NullBoard::m_instance = nullptr;

/**
 * @file liveboardnullboard.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief LiveboardEngine::NullBoard constructor
 * @param const QList<QRail::VehicleEngine::Vehicle *> &entries
 * @param StationEngine::Station *station
 * @param const QDateTime &from
 * @param const QDateTime &until
 * @param QObject *parent
 * @package LiveboardEngine
 * @private
 * Constructs a LiveboardEngine::NullBoard according to the Null design pattern.
 */
LiveboardEngine::NullBoard::NullBoard(
    const QList<VehicleEngine::Vehicle *> &entries,
    StationEngine::Station *station,
    const QDateTime &from,
    const QDateTime &until,
    QObject *parent) : LiveboardEngine::Board(entries, station, from, until, parent)
{

}

/**
 * @file liveboardnullboard.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Gets a LiveboardEngine::NullBoard instance
 * @return LiveboardEngine::NullBoard *board
 * @package LiveboardEngine
 * @public
 * Constructs a LiveboardEngine::NullBoard if none exists and returns the instance.
 */
LiveboardEngine::NullBoard *LiveboardEngine::NullBoard::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new NullBoard";
        m_instance = new NullBoard(
            QList<VehicleEngine::Vehicle *>(),
            StationEngine::NullStation::getInstance(),
            QDateTime(),
            QDateTime()
        );
    }
    return m_instance;
}

