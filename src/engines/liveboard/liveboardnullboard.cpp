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

LiveboardEngine::NullBoard::NullBoard(const QList<VehicleEngine::Vehicle *> &entries,
                                      StationEngine::Station *station,
                                      const QDateTime &from,
                                      const QDateTime &until,
                                      QObject *parent) : LiveboardEngine::Board(entries, station, from, until, parent)
{

}

LiveboardEngine::NullBoard *LiveboardEngine::NullBoard::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new NullBoard";
        m_instance = new NullBoard(QList<VehicleEngine::Vehicle *>(),
                                   StationEngine::NullStation::getInstance(),
                                   QDateTime(),
                                   QDateTime());
    }
    return m_instance;
}

