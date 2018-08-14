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

#ifndef CSAVEHICLESTOP_H
#define CSAVEHICLESTOP_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QMetaType>
#include "csavehicle.h"
#include "csastation.h"

namespace CSA {
class VehicleStop : public QObject
{
    Q_OBJECT
public:
    enum class Type
    {
        ARRIVAL,
        DEPARTURE,
        STOP
    };
    explicit VehicleStop(QObject *parent = nullptr);
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
    QUrl departureURI() const;
    void setDepartureURI(const QUrl &departureURI);
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
    void departureURIChanged();
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
    QUrl m_departureURI;
    CSA::Vehicle::OccupancyLevel m_occupancyLevel;
    CSA::VehicleStop::Type m_type;

    Q_ENUM(Type)
};
}

#endif // CSAVEHICLESTOP_H
