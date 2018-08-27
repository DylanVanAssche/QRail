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

#include "vehiclefactorytest.h"

void VehicleEngine::FactoryTest::initVehicleFactoryTest()
{
    qDebug() << "Init VehicleEngine::Factory test";
    factory = VehicleEngine::Factory::getInstance();
    connect(factory, SIGNAL(vehicleReady(VehicleEngine::Vehicle*)), this, SLOT(vehicleReady(VehicleEngine::Vehicle*)));
}

void VehicleEngine::FactoryTest::runVehicleFactoryTest()
{
    qDebug() << "Running VehicleEngine::Factory test";

    // Activate QSignalSpy
    qRegisterMetaType< VehicleEngine::Vehicle* >("VehicleEngine::Vehicle"); // register custom class
    QSignalSpy spyVehicle(factory, SIGNAL(vehicleReady(VehicleEngine::Vehicle*)));

    qDebug() << "Vehicle L562 route";
    factory->getVehicleByURI(
                QUrl("http://irail.be/vehicle/L562"),
                QLocale::Language::Dutch
                );
    QVERIFY(spyVehicle.wait(VEHICLE_WAIT_TIME));

    qDebug() << "Vehicle L562 route should NOT be retrieved from cache (cache-control: no-cache header)";
    factory->getVehicleByURI(
                QUrl("http://irail.be/vehicle/L562"),
                QLocale::Language::Dutch
                );
    QVERIFY(spyVehicle.wait(VEHICLE_WAIT_TIME));

    qDebug() << "Vehicle IC540 route";
    factory->getVehicleByURI(
                QUrl("http://irail.be/vehicle/IC540"),
                QLocale::Language::Dutch
                );
    QVERIFY(spyVehicle.wait(VEHICLE_WAIT_TIME));
}

void VehicleEngine::FactoryTest::cleanVehicleFactoryTest()
{
    qDebug() << "Clean up VehicleEngine::Factory test";
}

void VehicleEngine::FactoryTest::vehicleReady(VehicleEngine::Vehicle *vehicle)
{
    qDebug() << "Received vehicle:" << vehicle->uri();
    qDebug() << "\tTrip URI:" << vehicle->tripURI();
    qDebug() << "\tHeadsign:" << vehicle->headsign();
    qDebug() << "\tStops:";
    foreach(VehicleEngine::Stop *stop, vehicle->intermediaryStops()) {
        qDebug() << "\t\t" << stop->arrivalTime().toString("hh:mm") << stop->station()->name().value(QLocale::Language::Dutch);
    }
}
