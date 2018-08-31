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
#ifndef VEHICLEFACTORYTEST_H
#define VEHICLEFACTORYTEST_H

#include <QtCore/QLocale>
#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include "engines/vehicle/vehiclefactory.h"
#include "engines/vehicle/vehiclestop.h"
#include "engines/vehicle/vehiclevehicle.h"

#define VEHICLE_WAIT_TIME 7500

namespace QRail {
    namespace VehicleEngine {
        class FactoryTest : public QObject {
          Q_OBJECT
        public slots:
          void vehicleReady(QRail::VehicleEngine::Vehicle *vehicle);

        private slots:
          void initVehicleFactoryTest();
          void runVehicleFactoryTest();
          void cleanVehicleFactoryTest();

        private:
          QRail::VehicleEngine::Factory *factory;
        };
    } // namespace VehicleEngine
} // namespace QRail

#endif // VEHICLEFACTORYTEST_H
