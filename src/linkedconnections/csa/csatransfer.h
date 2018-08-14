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

#ifndef CSATRANSFER_H
#define CSATRANSFER_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>
#include "csarouteleg.h"
#include "csaroutelegend.h"

namespace CSA {
class Transfer : public QObject
{
    Q_OBJECT
public:
    enum class Type
    {
        ARRIVAL,
        DEPARTURE,
        TRANSFER,
        INVALID
    };
    explicit Transfer(
            CSA::RouteLeg *departureLeg = nullptr,
            CSA::RouteLeg *arrivalLeg = nullptr,
            QObject *parent = nullptr
            );
    CSA::RouteLeg *departureLeg() const;
    void setDepartureLeg(CSA::RouteLeg *departureLeg);
    CSA::RouteLeg *arrivalLeg() const;
    void setArrivalLeg(CSA::RouteLeg *arrivalLeg);
    CSA::RouteLegEnd *departure() const;
    void setDeparture(CSA::RouteLegEnd *departure);
    CSA::RouteLegEnd *arrival() const;
    void setArrival(CSA::RouteLegEnd *arrival);
    CSA::Transfer::Type type() const;
    void setType(const CSA::Transfer::Type &type);
    QUrl uri() const;
    CSA::Station *station() const;
    QDateTime time() const;
    qint16 delay() const;
    QDateTime delayedTime() const;
    QString platform() const;
    bool isCanceled() const;
    bool isNormalPlatform() const;
    bool isPassed() const;
    CSA::Vehicle::OccupancyLevel occupancyLevel() const;

signals:
    void departureLegChanged();
    void arrivalLegChanged();
    void departureChanged();
    void arrivalChanged();
    void typeChanged();

private:
    CSA::RouteLeg *m_departureLeg;
    CSA::RouteLeg *m_arrivalLeg;
    CSA::RouteLegEnd *m_departure;
    CSA::RouteLegEnd *m_arrival;
    CSA::Transfer::Type m_type;

    Q_ENUM(Type)
};
}

#endif // CSATRANSFER_H
