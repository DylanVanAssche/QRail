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
#ifndef CSATRANSFER_H
#define CSATRANSFER_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtCore/QDebug>

#include "qrail.h"
#include "engines/router/routerrouteleg.h"
#include "engines/router/routerroutelegend.h"

namespace QRail {
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
            QRail::RouterEngine::RouteLeg *departureLeg = nullptr,
            QRail::RouterEngine::RouteLeg *arrivalLeg = nullptr,
            QObject *parent = nullptr
            );
    QRail::RouterEngine::RouteLeg *departureLeg() const;
    void setDepartureLeg(QRail::RouterEngine::RouteLeg *departureLeg);
    QRail::RouterEngine::RouteLeg *arrivalLeg() const;
    void setArrivalLeg(QRail::RouterEngine::RouteLeg *arrivalLeg);
    QRail::RouterEngine::RouteLegEnd *departure() const;
    void setDeparture(QRail::RouterEngine::RouteLegEnd *departure);
    QRail::RouterEngine::RouteLegEnd *arrival() const;
    void setArrival(QRail::RouterEngine::RouteLegEnd *arrival);
    QRail::RouterEngine::Transfer::Type type() const;
    void setType(const QRail::RouterEngine::Transfer::Type &type);
    QUrl uri() const;
    StationEngine::Station *station() const;
    QDateTime time() const;
    qint16 delay() const;
    QDateTime delayedTime() const;
    QString platform() const;
    bool isCanceled() const;
    bool isNormalPlatform() const;
    bool isPassed() const;
    QRail::VehicleEngine::Stop::OccupancyLevel occupancyLevel() const;

signals:
    void departureLegChanged();
    void arrivalLegChanged();
    void departureChanged();
    void arrivalChanged();
    void typeChanged();

private:
    QRail::RouterEngine::RouteLeg *m_departureLeg;
    QRail::RouterEngine::RouteLeg *m_arrivalLeg;
    QRail::RouterEngine::RouteLegEnd *m_departure;
    QRail::RouterEngine::RouteLegEnd *m_arrival;
    QRail::RouterEngine::Transfer::Type m_type;

    Q_ENUM(Type)
};
}
}

#endif // CSATRANSFER_H
