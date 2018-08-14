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

#ifndef CSAROUTELEG_H
#define CSAROUTELEG_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QMetaType>
#include "csaroutelegend.h"
#include "csavehicle.h"
#include "csavehiclestop.h"
#include "csanullstation.h"

namespace CSA {
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
            const CSA::RouteLeg::Type &type,
            CSA::Vehicle *vehicleInformation,
            CSA::RouteLegEnd *departure,
            CSA::RouteLegEnd *arrival,
            QObject *parent = nullptr
            );
    CSA::RouteLeg::Type type() const;
    void setType(const CSA::RouteLeg::Type &type);
    CSA::Vehicle *vehicleInformation() const;
    void setVehicleInformation(CSA::Vehicle *vehicleInformation);
    QList<CSA::VehicleStop *> intermediaryStops() const;
    void setIntermediaryStops(const QList<CSA::VehicleStop *> &intermediaryStops);
    CSA::RouteLegEnd *departure() const;
    void setDeparture(CSA::RouteLegEnd *departure);
    CSA::RouteLegEnd *arrival() const;
    void setArrival(CSA::RouteLegEnd *arrival);
    CSA::Station *station() const;

signals:
    void typeChanged();
    void vehicleInformationChanged();
    void intermediaryStopsChanged();
    void stopsChanged();
    void departureChanged();
    void arrivalChanged();
    void stationChanged();

private:
    CSA::RouteLeg::Type m_type;
    CSA::Vehicle *m_vehicleInformation;
    QList<CSA::VehicleStop *> m_intermediaryStops;
    CSA::RouteLegEnd *m_departure;
    CSA::RouteLegEnd *m_arrival;

    Q_ENUM(Type)
};
}

#endif // CSAROUTELEG_H
