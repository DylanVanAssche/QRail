/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of QRail.                                               *
 *                                                                            *
 *   QRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   QRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with QRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#ifndef CSAROUTELEG_H
#define CSAROUTELEG_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include "engines/router/routerroutelegend.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "engines/station/stationnullstation.h"

namespace RouterEngine {
class RouteLeg : public QObject
{
    Q_OBJECT
public:
    enum class Type
    {
        WALKING,
        BICYCLE,
        BUS,
        TRAM,
        METRO,
        BOAT,
        TAXI,
        TRAIN
    };
    explicit RouteLeg(
            const RouterEngine::RouteLeg::Type &type,
            VehicleEngine::Vehicle *vehicleInformation,
            RouterEngine::RouteLegEnd *departure,
            RouterEngine::RouteLegEnd *arrival,
            QObject *parent = nullptr
            );
    RouterEngine::RouteLeg::Type type() const;
    void setType(const RouterEngine::RouteLeg::Type &type);
    VehicleEngine::Vehicle *vehicleInformation() const;
    void setVehicleInformation(VehicleEngine::Vehicle *vehicleInformation);
    RouterEngine::RouteLegEnd *departure() const;
    void setDeparture(RouterEngine::RouteLegEnd *departure);
    RouterEngine::RouteLegEnd *arrival() const;
    void setArrival(RouterEngine::RouteLegEnd *arrival);
    StationEngine::Station *station() const;

signals:
    void typeChanged();
    void vehicleInformationChanged();
    void stopsChanged();
    void departureChanged();
    void arrivalChanged();
    void stationChanged();

private:
    RouterEngine::RouteLeg::Type m_type;
    VehicleEngine::Vehicle *m_vehicleInformation;
    RouterEngine::RouteLegEnd *m_departure;
    RouterEngine::RouteLegEnd *m_arrival;

    Q_ENUM(Type)
};
}

#endif // CSAROUTELEG_H
