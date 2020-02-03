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
using namespace QRail;

void QRail::VehicleEngine::FactoryTest::initVehicleFactoryTest()
{
    qDebug() << "Init QRail::VehicleEngine::Factory test";
    factory = QRail::VehicleEngine::Factory::getInstance();
    connect(factory, SIGNAL(finished(QRail::VehicleEngine::Vehicle *)), this,
            SLOT(vehicleReady(QRail::VehicleEngine::Vehicle *)));
}

void QRail::VehicleEngine::FactoryTest::runVehicleFactoryTest()
{
    qDebug() << "Running QRail::VehicleEngine::Factory test";

    // Activate QSignalSpy
    qRegisterMetaType<QRail::VehicleEngine::Vehicle *>("QRail::VehicleEngine::Vehicle"); // register custom class
    QSignalSpy spyVehicle(factory, SIGNAL(finished(QRail::VehicleEngine::Vehicle *)));

    qDebug() << "Vehicle L562 route";
    factory->getVehicleByURI(QUrl("http://irail.be/vehicle/L562"), QLocale::Language::Dutch);

    qDebug() <<
             "Vehicle L562 route should NOT be retrieved from the network cache (cache-control: no-cache header)";
    factory->getVehicleByURI(QUrl("http://irail.be/vehicle/L562"), QLocale::Language::Dutch);

    qDebug() << "Vehicle IC540 route";
    factory->getVehicleByURI(QUrl("http://irail.be/vehicle/IC540"), QLocale::Language::Dutch);

    // Wait until we received some signals firing from the factory
    QVERIFY(spyVehicle.wait(VEHICLE_WAIT_TIME));
}

void QRail::VehicleEngine::FactoryTest::cleanVehicleFactoryTest()
{
    qDebug() << "Clean up QRail::VehicleEngine::Factory test";
}

void QRail::VehicleEngine::FactoryTest::vehicleReady(QRail::VehicleEngine::Vehicle *vehicle)
{
    qDebug() << "Received vehicle:" << vehicle->uri();
    qDebug() << "\tTrip URI:" << vehicle->tripURI();
    qDebug() << "\tHeadsign:" << vehicle->headsign();
    qDebug() << "\tStops:";
    foreach (QSharedPointer<QRail::VehicleEngine::Stop> stop, vehicle->intermediaryStops()) {
        qDebug() << "\t\t" << stop->arrivalTime().toString("hh:mm") << stop->station()->name().value(
                     QLocale::Language::Dutch);
    }
}
