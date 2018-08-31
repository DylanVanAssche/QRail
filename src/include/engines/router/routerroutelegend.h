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

#ifndef CSAROUTELEGEND_H
#define CSAROUTELEGEND_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QDateTime>
#include "engines/station/stationstation.h"
#include "engines/vehicle/vehiclevehicle.h"

namespace QRail {
    namespace RouterEngine {
        class RouteLegEnd : public QObject
        {
            Q_OBJECT
        public:
            explicit RouteLegEnd(
                    const QUrl &uri,
                    const QDateTime &time,
                    StationEngine::Station *station,
                    const QString &platform,
                    const bool &isNormalPlatform,
                    const qint16 &delay,
                    const bool &isCanceled,
                    const bool &isPassed,
                    const QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel,
                    QObject *parent = nullptr
                    );
            QUrl uri() const;
            void setUri(const QUrl &uri);
            QDateTime time() const;
            void setTime(const QDateTime &time);
            StationEngine::Station *station() const;
            void setStation(StationEngine::Station *station);
            QString platform() const;
            void setPlatform(const QString &platform);
            bool isNormalPlatform() const;
            void setIsNormalPlatform(bool isNormalPlatform);
            qint16 delay() const;
            void setDelay(const qint16 &delay);
            bool isCanceled() const;
            void setIsCanceled(bool isCanceled);
            bool isPassed() const;
            void setIsPassed(bool isPassed);
            QRail::VehicleEngine::Stop::OccupancyLevel occupancyLevel() const;
            void setOccupancyLevel(const QRail::VehicleEngine::Stop::OccupancyLevel &occupancyLevel);

        signals:
            void uriChanged();
            void timeChanged();
            void stationChanged();
            void platformChanged();
            void isNormalPlatformChanged();
            void delayChanged();
            void isCanceledChanged();
            void isPassedChanged();
            void occupancyLevelChanged();
            // connect to signals of the underlying things depending on the type!

        private:
            QUrl m_uri;
            QDateTime m_time;
            StationEngine::Station *m_station;
            QString m_platform;
            bool m_isNormalPlatform;
            qint16 m_delay;
            bool m_isCanceled;
            bool m_isPassed;
            QRail::VehicleEngine::Stop::OccupancyLevel m_occupancyLevel;
        };
    }
}

#endif // CSAROUTELEGEND_H
