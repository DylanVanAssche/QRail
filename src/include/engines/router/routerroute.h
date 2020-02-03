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
//! A RouterEngine::Route contains all the information about a single route of Journey.
/*!
    \class Route
    Using a Route you can describe a complete route between 2 stops with all the transfers, alerts, ...
 */
class QRAIL_SHARED_EXPORT Route : public QObject
{
    Q_OBJECT
public:
    //! QRail::RouterEngine::Route constructor (empty).
    /*!
        \param parent QObject parent-child memory management.
        \public
        Constructs a QRail::RouterEngine::Route with the given parent.
     */
    explicit Route(const QList<QSharedPointer<QRail::RouterEngine::RouteLeg>> legs, QObject *parent = nullptr);
    //! QRail::RouterEngine::Route constructor.
    /*!
        \param legs A list of RouteLeg.
        \param transfers A list of Transfer.
        \param tripAlerts A list of Message about the trip.
        \param vehicleAlerts A list of Message about the vehicles.
        \param remarks A list of Message about the remarks.
        \param parent QObject parent-child memory management.
        \public
        Constructs a QRail::RouterEngine::Route with the given parent.
     */
    explicit Route(const QList<QSharedPointer<QRail::RouterEngine::RouteLeg>> legs,
                   const QList<QSharedPointer<QRail::RouterEngine::Transfer>> transfers,
                   const QList<QSharedPointer<QRail::AlertsEngine::Message>> tripAlerts,
                   const QList<QSharedPointer<QRail::AlertsEngine::Message>> vehicleAlerts,
                   const QList<QSharedPointer<QRail::AlertsEngine::Message>> remarks,
                   QObject *parent = nullptr);
    QList<QSharedPointer<QRail::RouterEngine::RouteLeg>> legs() const;
    void setLegs(const QList<QSharedPointer<QRail::RouterEngine::RouteLeg>> legs);
    QList<QSharedPointer<QRail::RouterEngine::Transfer>> transfers() const;
    void setTransfers(const QList<QSharedPointer<QRail::RouterEngine::Transfer>> transfers);
    QList<QSharedPointer<QRail::AlertsEngine::Message>> tripAlerts() const;
    void setTripAlerts(const QList<QSharedPointer<QRail::AlertsEngine::Message>> tripAlerts);
    QList<QSharedPointer<QRail::AlertsEngine::Message>> vehicleAlerts() const;
    void setVehicleAlerts(const QList<QSharedPointer<QRail::AlertsEngine::Message>> vehicleAlerts);
    QList<QSharedPointer<QRail::AlertsEngine::Message>> remarks() const;
    void setRemarks(const QList<QSharedPointer<QRail::AlertsEngine::Message>> remarks);
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
    QSharedPointer<QRail::RouterEngine::Transfer> departureStation() const;
    QSharedPointer<QRail::RouterEngine::Transfer> arrivalStation() const;
    bool isPartiallyCanceled() const;

signals:
    //! Emitted when one of the legs are changed.
    void legsChanged();
    //! Emitted when the transfers are changed.
    void transfersChanged();
    //! Emitted with the alerts for the complete trips are changed.
    void tripAlertsChanged();
    //! Emitted when the alerts for the vehicles in the Route are changed.
    void vehicleAlertsChanged();
    //! Emitted when any remarks are changed.
    void remarksChanged();

private:
    QList<QSharedPointer<QRail::RouterEngine::RouteLeg>> m_legs;
    QList<QSharedPointer<QRail::RouterEngine::Transfer>> m_transfers;
    QList<QSharedPointer<QRail::AlertsEngine::Message>> m_tripAlerts;
    QList<QSharedPointer<QRail::AlertsEngine::Message>> m_vehicleAlerts;
    QList<QSharedPointer<QRail::AlertsEngine::Message>> m_remarks;
};
} // namespace RouterEngine
} // namespace QRail

#endif // CSAROUTE_H
