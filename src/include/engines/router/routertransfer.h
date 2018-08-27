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

#ifndef CSATRANSFER_H
#define CSATRANSFER_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

#include "qrail.h"
#include "engines/router/routerrouteleg.h"
#include "engines/router/routerroutelegend.h"

namespace RouterEngine {
class QRAIL_SHARED_EXPORT Transfer : public QObject
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
            RouterEngine::RouteLeg *departureLeg = nullptr,
            RouterEngine::RouteLeg *arrivalLeg = nullptr,
            QObject *parent = nullptr
            );
    RouterEngine::RouteLeg *departureLeg() const;
    void setDepartureLeg(RouterEngine::RouteLeg *departureLeg);
    RouterEngine::RouteLeg *arrivalLeg() const;
    void setArrivalLeg(RouterEngine::RouteLeg *arrivalLeg);
    RouterEngine::RouteLegEnd *departure() const;
    void setDeparture(RouterEngine::RouteLegEnd *departure);
    RouterEngine::RouteLegEnd *arrival() const;
    void setArrival(RouterEngine::RouteLegEnd *arrival);
    RouterEngine::Transfer::Type type() const;
    void setType(const RouterEngine::Transfer::Type &type);
    QUrl uri() const;
    StationEngine::Station *station() const;
    QDateTime time() const;
    qint16 delay() const;
    QDateTime delayedTime() const;
    QString platform() const;
    bool isCanceled() const;
    bool isNormalPlatform() const;
    bool isPassed() const;
    VehicleEngine::Stop::OccupancyLevel occupancyLevel() const;

signals:
    void departureLegChanged();
    void arrivalLegChanged();
    void departureChanged();
    void arrivalChanged();
    void typeChanged();

private:
    RouterEngine::RouteLeg *m_departureLeg;
    RouterEngine::RouteLeg *m_arrivalLeg;
    RouterEngine::RouteLegEnd *m_departure;
    RouterEngine::RouteLegEnd *m_arrival;
    RouterEngine::Transfer::Type m_type;

    Q_ENUM(Type)
};
}

#endif // CSATRANSFER_H
