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
#include "engines/vehicle/vehiclenullvehicle.h"
using namespace QRail;
QSharedPointer<VehicleEngine::NullVehicle> VehicleEngine::NullVehicle::m_instance = nullptr;

/**
 * @file vehiclenullvehicle.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief QRail::VehicleEngine::NullVehicle constructor
 * @param const QUrl &uri
 * @param const QUrl &tripURI
 * @param const QString &headsign
 * @param const QList<QRail::VehicleEngine::Stop *> &intermediaryStops
 * @package VehicleEngine
 * @public
 * Constructs a QRail::VehicleEngine::NullVehicle according to the Null design pattern.
 */
QRail::VehicleEngine::NullVehicle::NullVehicle(const QUrl &uri,
                                               const QUrl &tripURI,
                                               const QString &headsign,
                                               const QList<QSharedPointer<QRail::VehicleEngine::Stop>> intermediaryStops,
                                               QObject *parent) : Vehicle(uri, tripURI, headsign, intermediaryStops, parent)
{

}

/**
 * @file vehiclenullvehicle.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Gets a VehicleEngine::NullVehicle instance
 * @return VehicleEngine::NullVehicle *vehicle
 * @package VehicleEngine
 * @public
 * Constructs a VehicleEngine::NullVehicle if none exists and returns the instance.
 */
QSharedPointer<QRail::VehicleEngine::NullVehicle> QRail::VehicleEngine::NullVehicle::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new NullVehicle";
        m_instance = QSharedPointer<VehicleEngine::NullVehicle>(new VehicleEngine::NullVehicle(
            QUrl(),
            QUrl(),
            QString(),
            QList<QSharedPointer<QRail::VehicleEngine::Stop>>()
        ));
    }
    return m_instance;
}
