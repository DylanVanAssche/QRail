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
#ifndef LIVEBOARDNULLBOARD_H
#define LIVEBOARDNULLBOARD_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QList>

#include <engines/vehicle/vehiclevehicle.h>
#include "engines/station/stationstation.h"
#include "engines/station/stationnullstation.h"
#include "engines/liveboard/liveboardboard.h"
#include "qrail.h"

namespace QRail {
namespace LiveboardEngine {
/**
 * @class NullBoard
 * @brief An LiveboardEngine::NullBoard contains a empty liveboard.
 * A liveboard holds all the departing or arriving vehicles (depending on the mode) in a certain station.
 * \warning Receiving a NullBoard from the QRail library indicates that something must be terrible wrong! Check you debug output carefully.
 */
class QRAIL_SHARED_EXPORT NullBoard : public LiveboardEngine::Board
{
public:
    //! Gets a LiveboardEngine::NullBoard instance.
    /*!
        \return board a LiveboardEngine::NullBoard instance.
        \public
        Constructs a LiveboardEngine::NullBoard if none exists and returns the instance.
     */
    static NullBoard *getInstance();

private:
    NullBoard(const QList<QRail::VehicleEngine::Vehicle *> &entries,
              StationEngine::Station *station,
              const QDateTime &from,
              const QDateTime &until,
              QObject *parent = nullptr);
    static LiveboardEngine::NullBoard *m_instance;
};
}
}

#endif // LIVEBOARDNULLBOARD_H
