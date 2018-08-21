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

#ifndef CSAVEHICLESTOP_H
#define CSAVEHICLESTOP_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMetaType>

#include "../qrail.h"
#include "csavehicle.h"
#include "csastation.h"

namespace CSA {
class QRAIL_SHARED_EXPORT VehicleStop : public QObject
{
    Q_OBJECT
public:
    enum class Type
    {
        ARRIVAL,
        DEPARTURE,
        STOP,
        UNKNOWN
    };
    explicit VehicleStop(QObject *parent = nullptr);
    explicit VehicleStop(
            CSA::Vehicle *vehicleInformation,
            CSA::Station *station,
            const QString &platform,
            const bool &isPlatformNormal,
            const bool &hasLeft,
            const QDateTime &departureTime,
            const qint16 &departureDelay,
            const bool &isDepartureCanceled,
            const QDateTime &arrivalTime,
            const qint16 &arrivalDelay,
            const bool &isArrivalCanceled,
            const CSA::Vehicle::OccupancyLevel &occupancyLevel,
            const CSA::VehicleStop::Type &type,
            QObject *parent = nullptr);
    CSA::Vehicle *vehicleInformation() const;
    void setVehicleInformation(CSA::Vehicle *vehicleInformation);
    CSA::Station *station() const;
    void setStation(CSA::Station *station);
    QString platform() const;
    void setPlatform(const QString &platform);
    bool isPlatformNormal() const;
    void setIsPlatformNormal(const bool &isPlatformNormal);
    bool hasLeft() const;
    void setHasLeft(const bool &hasLeft);
    QDateTime departureTime() const;
    void setDepartureTime(const QDateTime &departureTime);
    qint16 departureDelay() const;
    void setDepartureDelay(const qint16 &departureDelay);
    bool isDepartureCanceled() const;
    void setIsDepartureCanceled(const bool &isDepartureCanceled);
    QDateTime arrivalTime() const;
    void setArrivalTime(const QDateTime &arrivalTime);
    qint16 arrivalDelay() const;
    void setArrivalDelay(const qint16 &arrivalDelay);
    bool isArrivalCanceled() const;
    void setIsArrivalCanceled(const bool &isArrivalCanceled);
    CSA::Vehicle::OccupancyLevel occupancyLevel() const;
    void setOccupancyLevel(const CSA::Vehicle::OccupancyLevel &occupancyLevel);
    CSA::VehicleStop::Type type() const;
    void setType(const CSA::VehicleStop::Type &type);

signals:
    void vehicleInformationChanged();
    void stationChanged();
    void platformChanged();
    void isPlatformNormalChanged();
    void hasLeftChanged();
    void departureTimeChanged();
    void departureDelayChanged();
    void isDepartureCanceledChanged();
    void arrivalTimeChanged();
    void arrivalDelayChanged();
    void isArrivalCanceledChanged();
    void occupancyLevelChanged();
    void typeChanged();

private:
    CSA::Vehicle *m_vehicleInformation;
    CSA::Station *m_station;
    QString m_platform;
    bool m_isPlatformNormal;
    bool m_hasLeft;
    QDateTime m_departureTime;
    qint16 m_departureDelay;
    bool m_isDepartureCanceled;
    QDateTime m_arrivalTime;
    qint16 m_arrivalDelay;
    bool m_isArrivalCanceled;
    CSA::Vehicle::OccupancyLevel m_occupancyLevel;
    CSA::VehicleStop::Type m_type;

    Q_ENUM(Type)
};
}

#endif // CSAVEHICLESTOP_H
