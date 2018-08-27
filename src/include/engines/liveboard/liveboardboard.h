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

#ifndef LIVEBOARDBOARD_H
#define LIVEBOARDBOARD_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include "engines/station/stationstation.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "fragments/fragmentspage.h"

namespace LiveboardEngine {
class Board : public QObject
{
    Q_OBJECT
public:
    enum class Mode
    {
        ARRIVALS,
        DEPARTURES,
    };
    explicit Board(QObject *parent = nullptr);
    explicit Board(
            const QList<VehicleEngine::Vehicle *> &entries,
            StationEngine::Station *station,
            const QDateTime &from,
            const QDateTime &until,
            QObject *parent = nullptr
            );
    void addEntry(VehicleEngine::Vehicle *entry);
    QList<VehicleEngine::Vehicle *> entries() const;
    void setEntries(const QList<VehicleEngine::Vehicle *> &entries);
    StationEngine::Station *station() const;
    void setStation(StationEngine::Station *station);
    QDateTime from() const;
    void setFrom(const QDateTime &from);
    QDateTime until() const;
    void setUntil(const QDateTime &until);
    LiveboardEngine::Board::Mode mode() const;
    void setMode(const LiveboardEngine::Board::Mode &mode);

signals:
    void entriesChanged();
    void stationChanged();
    void fromChanged();
    void untilChanged();
    void modeChanged();

private:
    QList<VehicleEngine::Vehicle *> m_entries;
    StationEngine::Station *m_station;
    QDateTime m_from;
    QDateTime m_until;
    LiveboardEngine::Board::Mode m_mode;

    Q_ENUM(Mode)
};
}

#endif // LIVEBOARDBOARD_H
