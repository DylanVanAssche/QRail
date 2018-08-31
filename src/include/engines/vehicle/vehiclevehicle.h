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
#ifndef CSAVEHICLE_H
#define CSAVEHICLE_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QMetaType>

#include "qrail.h"
#include "engines/vehicle/vehiclestop.h"
#include "engines/station/stationstation.h"

namespace QRail {
    namespace VehicleEngine {
        class QRAIL_SHARED_EXPORT Vehicle : public QObject
        {
            Q_OBJECT
        public:
            explicit Vehicle(QObject *parent = nullptr);
            explicit Vehicle(
                    const QUrl &uri,
                    const QUrl &tripURI,
                    const QString &headsign,
                    QObject *parent = nullptr
                    );
            explicit Vehicle(
                    const QUrl &uri,
                    const QUrl &tripURI,
                    const QString &headsign,
                    const QList<QRail::VehicleEngine::Stop *> &intermediaryStops,
                    QObject *parent = nullptr
                    );
            QUrl uri() const;
            void setUri(const QUrl &uri);
            QString headsign() const;
            void setHeadsign(const QString &headsign);
            QList<QRail::VehicleEngine::Stop *> intermediaryStops() const;
            void setIntermediaryStops(const QList<QRail::VehicleEngine::Stop *> &intermediaryStops);
            QUrl tripURI() const;
            void setTripURI(const QUrl &tripURI);

        signals:
            void uriChanged();
            void tripURIChanged();
            void headsignChanged();
            void intermediaryStopsChanged();

        private:
            QUrl m_uri;
            QUrl m_tripURI;
            QString m_headsign;
            QList<QRail::VehicleEngine::Stop *> m_intermediaryStops;
        };
    }
}

#endif // CSAVEHICLE_H
