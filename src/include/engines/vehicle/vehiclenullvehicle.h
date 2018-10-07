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
#ifndef VEHICLENULLVEHICLE_H
#define VEHICLENULLVEHICLE_H

#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QUrl>

#include "engines/vehicle/vehiclevehicle.h"
#include "engines/vehicle/vehiclestop.h"
#include "qrail.h"

namespace QRail {
namespace VehicleEngine {
class QRAIL_SHARED_EXPORT NullVehicle : public QRail::VehicleEngine::Vehicle
{
public:
    static NullVehicle *getInstance();

private:
    explicit NullVehicle(
        const QUrl &uri,
        const QUrl &tripURI,
        const QString &headsign,
        const QList<QRail::VehicleEngine::Stop *> &intermediaryStops,
        QObject *parent = nullptr
    );
    static VehicleEngine::NullVehicle *m_instance;
};
}
}

#endif // VEHICLENULLVEHICLE_H
