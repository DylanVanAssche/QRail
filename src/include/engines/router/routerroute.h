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
#ifndef CSAROUTE_H
#define CSAROUTE_H

#include <QtCore/QList>
#include <QtCore/QObject>

#include "engines/alerts/alertsmessage.h"
#include "engines/router/routerrouteleg.h"
#include "engines/router/routertransfer.h"
#include "qrail.h"

namespace QRail {
namespace RouterEngine {
class QRAIL_SHARED_EXPORT Route : public QObject
{
    Q_OBJECT
public:
    explicit Route(const QList<QRail::RouterEngine::RouteLeg *> &legs,
                   const QList<QRail::RouterEngine::Transfer *> &transfers,
                   const QList<QRail::AlertsEngine::Message *> &tripAlerts,
                   const QList<QRail::AlertsEngine::Message *> &vehicleAlerts,
                   const QList<QRail::AlertsEngine::Message *> &remarks,
                   QObject *parent = nullptr);
    explicit Route(const QList<RouteLeg *> &legs, QObject *parent = nullptr);
    QList<QRail::RouterEngine::RouteLeg *> legs() const;
    void setLegs(const QList<RouteLeg *> &legs);
    QList<QRail::RouterEngine::Transfer *> transfers() const;
    void setTransfers(const QList<QRail::RouterEngine::Transfer *> &transfers);
    QList<QRail::AlertsEngine::Message *> tripAlerts() const;
    void setTripAlerts(const QList<QRail::AlertsEngine::Message *> &tripAlerts);
    QList<QRail::AlertsEngine::Message *> vehicleAlerts() const;
    void setVehicleAlerts(const QList<QRail::AlertsEngine::Message *> &vehicleAlerts);
    QList<QRail::AlertsEngine::Message *> remarks() const;
    void setRemarks(const QList<QRail::AlertsEngine::Message *> &remarks);
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
    QRail::RouterEngine::Transfer *departureStation() const;
    QRail::RouterEngine::Transfer *arrivalStation() const;
    bool isPartiallyCanceled() const;

signals:
    void legsChanged();
    void transfersChanged();
    void tripAlertsChanged();
    void vehicleAlertsChanged();
    void remarksChanged();

private:
    QList<QRail::RouterEngine::RouteLeg *> m_legs;
    QList<QRail::RouterEngine::Transfer *> m_transfers;
    QList<QRail::AlertsEngine::Message *> m_tripAlerts;
    QList<QRail::AlertsEngine::Message *> m_vehicleAlerts;
    QList<QRail::AlertsEngine::Message *> m_remarks;
};
} // namespace RouterEngine
} // namespace QRail

#endif // CSAROUTE_H
