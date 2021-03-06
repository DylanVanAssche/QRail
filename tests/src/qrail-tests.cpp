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
#include <QtCore/QCoreApplication>
#include <QtTest/QtTest>

#include "database/databasemanagertest.h"
#include "engines/liveboard/liveboardfactorytest.h"
#include "engines/router/routerplannertest.h"
#include "engines/vehicle/vehiclefactorytest.h"
#include "engines/station/stationfactorytest.h"
#include "fragments/fragmentsfragmenttest.h"
#include "fragments/fragmentspagetest.h"
#include "network/networkmanagertest.h"
#include "network/networkeventsourcetest.h"
#include "qrail.h"

#define WAIT_TIME 5000

// Unittest main method
int main(int argc, char *argv[])
{
    // Allow QEventLoops for QSignalSpy
    QCoreApplication app(argc, argv);

    /*
    * This workaround will execute every test as soon as the event loop is
    * started. NOTE: Requires C++11 support INFO:
    * https://forum.qt.io/topic/36208/solved-console-application-does-not-exit/7
    */
    QTimer::singleShot(0, []() {
        // Init the QRail library
        initQRail();

        // Create test instances
        int networkManagerResult = -1;
        int networkEventSourceResult = 0; //-1; TODO: Add SSE endpoint on a server to test this on Travis CI
        int dbManagerResult = -1;
        int lcFragmentResult = -1;
        int lcPageResult = -1;
        int routerPlannerResult = 0; //-1 Needs reproducing tests (test datasets)
        int liveboardFactoryResult = 0; //-1 Needs reproducing tests (test datasets)
        int vehicleFactoryResult = -1;
        int stationFactoryResult = -1;
        QRail::Network::ManagerTest testSuiteNetworkManager;
        QRail::Network::EventSourceTest testSuitsNetworkEventSource;
        QRail::Database::ManagerTest testSuiteDBManager;
        QRail::Fragments::FragmentTest testSuiteLCFragment;
        QRail::Fragments::PageTest testSuiteLCPage;
        QRail::RouterEngine::PlannerTest testSuiteCSAPlanner;
        QRail::LiveboardEngine::FactoryTest testSuiteLiveboardFactory;
        QRail::VehicleEngine::FactoryTest testSuiteVehicleFactory;
        QRail::StationEngine::FactoryTest testSuiteStationFactory;

        // Run unit tests without passing arguments
        networkManagerResult = QTest::qExec(&testSuiteNetworkManager, 0, nullptr);
        //networkEventSourceResult = QTest::qExec(&testSuitsNetworkEventSource, 0, nullptr);
        dbManagerResult = QTest::qExec(&testSuiteDBManager, 0, nullptr);
        lcFragmentResult = QTest::qExec(&testSuiteLCFragment, 0, nullptr);
        lcPageResult = QTest::qExec(&testSuiteLCPage, 0, nullptr);

        // Run QRail::StationEngine::Factory integration test
        stationFactoryResult = QTest::qExec(&testSuiteStationFactory, 0, nullptr);

        // Run QRail::LiveboardEngine::Factory integration test
        liveboardFactoryResult = QTest::qExec(&testSuiteLiveboardFactory, 0, nullptr);

        // Run QRail::LiveboardEngine::Factory integration test
        vehicleFactoryResult = QTest::qExec(&testSuiteVehicleFactory, 0, nullptr);

        // Run QRail::RouterEngine::Planner integration test
        routerPlannerResult = QTest::qExec(&testSuiteCSAPlanner, 0, nullptr);

        // Return the status code of every test for CI/CD
        QCoreApplication::exit(networkManagerResult | networkEventSourceResult | dbManagerResult | lcFragmentResult | lcPageResult |
                               routerPlannerResult | liveboardFactoryResult | vehicleFactoryResult | stationFactoryResult);
    });
    return app.exec();
}
