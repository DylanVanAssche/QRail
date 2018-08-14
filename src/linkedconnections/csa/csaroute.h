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

#ifndef CSAROUTE_H
#define CSAROUTE_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include "csarouteleg.h"
#include "csatransfer.h"
#include "csamessage.h"

namespace CSA {
class Route : public QObject
{
    Q_OBJECT
public:
    explicit Route(
            const QList<CSA::RouteLeg *> &legs,
            const QList<CSA::Transfer *> &transfers,
            const QList<CSA::Message *> &tripAlerts,
            const QList<CSA::Message *> &vehicleAlerts,
            const QList<CSA::Message *> &remarks,
            QObject *parent = nullptr
            );
    explicit Route(
            const QList<RouteLeg *> &legs,
            QObject *parent = nullptr
            );
    QList<CSA::RouteLeg *> legs() const;
    void setLegs(const QList<RouteLeg *> &legs);
    QList<CSA::Transfer *> transfers() const;
    void setTransfers(const QList<CSA::Transfer *> &transfers);
    QList<CSA::Message *> tripAlerts() const;
    void setTripAlerts(const QList<CSA::Message *> &tripAlerts);
    QList<CSA::Message *> vehicleAlerts() const;
    void setVehicleAlerts(const QList<CSA::Message *> &vehicleAlerts);
    QList<CSA::Message *> remarks() const;
    void setRemarks(const QList<CSA::Message *> &remarks);
    qint64 duration() const;
    qint64 durationWithDelays() const;
    QDateTime departureTime() const;
    qint16 departureDelay() const;
    QDateTime departureDelayedTime() const;
    QDateTime arrivalTime() const;
    QDateTime arrivalDelayedTime()const;
    qint16 arrivalDelay() const;
    qint16 transferCount() const;
    qint16 stationCount() const;
    QString departurePlatform() const;
    bool isDeparturePlatformNormal() const;
    QString arrivalPlatform() const;
    bool isArrivalPlatformNormal() const;
    CSA::Transfer *departureStation() const;
    CSA::Transfer *arrivalStation() const;
    bool isPartiallyCanceled() const;

signals:
    void legsChanged();
    void transfersChanged();
    void tripAlertsChanged();
    void vehicleAlertsChanged();
    void remarksChanged();

private:
    QList<CSA::RouteLeg *> m_legs;
    QList<CSA::Transfer *> m_transfers;
    QList<CSA::Message *> m_tripAlerts;
    QList<CSA::Message *> m_vehicleAlerts;
    QList<CSA::Message *> m_remarks;
};
}

#endif // CSAROUTE_H
