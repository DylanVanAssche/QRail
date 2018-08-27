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

#ifndef CSAROUTE_H
#define CSAROUTE_H

#include <QtCore/QObject>
#include <QtCore/QList>

#include "qrail.h"
#include "engines/router/routerrouteleg.h"
#include "engines/router/routertransfer.h"
#include "engines/alerts/alertsmessage.h"

namespace RouterEngine {
class QRAIL_SHARED_EXPORT Route : public QObject
{
    Q_OBJECT
public:
    explicit Route(
            const QList<RouterEngine::RouteLeg *> &legs,
            const QList<RouterEngine::Transfer *> &transfers,
            const QList<AlertsEngine::Message *> &tripAlerts,
            const QList<AlertsEngine::Message *> &vehicleAlerts,
            const QList<AlertsEngine::Message *> &remarks,
            QObject *parent = nullptr
            );
    explicit Route(
            const QList<RouteLeg *> &legs,
            QObject *parent = nullptr
            );
    QList<RouterEngine::RouteLeg *> legs() const;
    void setLegs(const QList<RouteLeg *> &legs);
    QList<RouterEngine::Transfer *> transfers() const;
    void setTransfers(const QList<RouterEngine::Transfer *> &transfers);
    QList<AlertsEngine::Message *> tripAlerts() const;
    void setTripAlerts(const QList<AlertsEngine::Message *> &tripAlerts);
    QList<AlertsEngine::Message *> vehicleAlerts() const;
    void setVehicleAlerts(const QList<AlertsEngine::Message *> &vehicleAlerts);
    QList<AlertsEngine::Message *> remarks() const;
    void setRemarks(const QList<AlertsEngine::Message *> &remarks);
    qint64 duration() const;
    qint64 durationWithDelays() const;
    QDateTime departureTime() const;
    qint16 departureDelay() const;
    QDateTime arrivalTime() const;
    qint16 arrivalDelay() const;
    qint16 transferCount() const;
    qint16 stationCount() const;
    QString departurePlatform() const;
    bool isDeparturePlatformNormal() const;
    QString arrivalPlatform() const;
    bool isArrivalPlatformNormal() const;
    RouterEngine::Transfer *departureStation() const;
    RouterEngine::Transfer *arrivalStation() const;
    bool isPartiallyCanceled() const;

signals:
    void legsChanged();
    void transfersChanged();
    void tripAlertsChanged();
    void vehicleAlertsChanged();
    void remarksChanged();

private:
    QList<RouterEngine::RouteLeg *> m_legs;
    QList<RouterEngine::Transfer *> m_transfers;
    QList<AlertsEngine::Message *> m_tripAlerts;
    QList<AlertsEngine::Message *> m_vehicleAlerts;
    QList<AlertsEngine::Message *> m_remarks;
};
}

#endif // CSAROUTE_H
