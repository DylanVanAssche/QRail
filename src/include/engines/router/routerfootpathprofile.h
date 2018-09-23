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
#ifndef ROUTERFOOTPATHPROFILE_H
#define ROUTERFOOTPATHPROFILE_H

#include <QtCore/QObject>

#include "engines/station/stationstation.h"

namespace QRail {
namespace RouterEngine {
class FootpathProfile : public QObject
{
    Q_OBJECT
public:
    explicit FootpathProfile(QRail::StationEngine::Station *arrivalStation,
                             QRail::StationEngine::Station *departureStation,
                             qreal distance,
                             QObject *parent = nullptr);
    QRail::StationEngine::Station *arrivalStation() const;
    void setArrivalStation(QRail::StationEngine::Station *arrivalStation);
    QRail::StationEngine::Station *departureStation() const;
    void setDepartureStation(QRail::StationEngine::Station *departureStation);
    qreal distance() const;
    void setDistance(const qreal &distance);

private:
    QRail::StationEngine::Station *m_arrivalStation;
    QRail::StationEngine::Station *m_departureStation;
    qreal m_distance;
};
}
}

#endif // ROUTERFOOTPATHPROFILE_H
