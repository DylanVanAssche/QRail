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

#ifndef CSAVEHICLESTOP_H
#define CSAVEHICLESTOP_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QMetaType>

#include "qrail.h"
#include "engines/station/stationstation.h"

namespace VehicleEngine {
class QRAIL_SHARED_EXPORT Stop : public QObject
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
    enum class OccupancyLevel
    {
        UNSUPPORTED,
        UNKNOWN,
        LOW,
        MEDIUM,
        HIGH
    };
    explicit Stop(QObject *parent = nullptr);
    explicit Stop(
            StationEngine::Station *station,
            const QString &platform,
            const bool &isPlatformNormal,
            const bool &hasLeft,
            const QDateTime &departureTime,
            const qint16 &departureDelay,
            const bool &isDepartureCanceled,
            const QDateTime &arrivalTime,
            const qint16 &arrivalDelay,
            const bool &isArrivalCanceled,
            const bool &isExtraStop,
            const VehicleEngine::Stop::OccupancyLevel &occupancyLevel,
            const VehicleEngine::Stop::Type &type,
            QObject *parent = nullptr
            );
    StationEngine::Station *station() const;
    void setStation(StationEngine::Station *station);
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
    VehicleEngine::Stop::OccupancyLevel occupancyLevel() const;
    void setOccupancyLevel(const VehicleEngine::Stop::OccupancyLevel &occupancyLevel);
    VehicleEngine::Stop::Type type() const;
    void setType(const VehicleEngine::Stop::Type &type);
    bool isExtraStop() const;
    void setIsExtraStop(const bool &isExtraStop);

signals:
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
    void isExtraStopChanged();
    void occupancyLevelChanged();
    void typeChanged();

private:
    StationEngine::Station *m_station;
    QString m_platform;
    bool m_isPlatformNormal;
    bool m_hasLeft;
    QDateTime m_departureTime;
    qint16 m_departureDelay;
    bool m_isDepartureCanceled;
    QDateTime m_arrivalTime;
    qint16 m_arrivalDelay;
    bool m_isArrivalCanceled;
    bool m_isExtraStop;
    VehicleEngine::Stop::OccupancyLevel m_occupancyLevel;
    VehicleEngine::Stop::Type m_type;

    Q_ENUM(Type)
    Q_ENUM(OccupancyLevel)
};
}

#endif // CSAVEHICLESTOP_H
