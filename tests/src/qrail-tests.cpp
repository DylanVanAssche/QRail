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

#include "qrail.h"
#include "network/networkmanagertest.h"
#include "database/databasemanagertest.h"
#include "fragments/fragmentsfragmenttest.h"
#include "fragments/fragmentspagetest.h"
#include "fragments/fragmentsfactorytest.h"
#include "engines/router/routerplannertest.h"
#include "engines/liveboard/liveboardfactorytest.h"
#include "engines/vehicle/vehiclefactorytest.h"

#define WAIT_TIME 5000

// Unittest main method
int main(int argc, char *argv[])
{
    // Allow QEventLoops for QSignalSpy
    QCoreApplication app(argc, argv);

    /*
     * This workaround will execute every test as soon as the event loop is started.
     * NOTE: Requires C++11 support
     * INFO: https://forum.qt.io/topic/36208/solved-console-application-does-not-exit/7
     */
    QTimer::singleShot(0, [](){
        // Init the QRail library
        initQRail();

        // Create test instances
        int networkManagerResult = 0;
        int dbManagerResult = 0;
        int lcFragmentResult = 0;
        int lcPageResult = 0;
        int lcFactoryResult = 0;
        int routerPlannerResult = 0;
        int liveboardFactoryResult = 0;
        int vehicleFactoryResult = -1;
        Network::ManagerTest testSuiteNetworkManager;
        Database::ManagerTest testSuiteDBManager;
        Fragments::FragmentTest testSuiteLCFragment;
        Fragments::PageTest testSuiteLCPage;
        Fragments::FactoryTest testSuiteLCFactory;
        RouterEngine::PlannerTest testSuiteCSAPlanner;
        LiveboardEngine::FactoryTest testSuiteLiveboardFactory;
        VehicleEngine::FactoryTest testSuiteVehicleFactory;

        // Run unit tests without passing arguments
        /*networkManagerResult = QTest::qExec(&testSuiteNetworkManager, 0, nullptr);
        dbManagerResult = QTest::qExec(&testSuiteDBManager, 0, nullptr);
        lcFragmentResult = QTest::qExec(&testSuiteLCFragment, 0, nullptr);
        lcPageResult = QTest::qExec(&testSuiteLCPage, 0, nullptr);
        lcFactoryResult = QTest::qExec(&testSuiteLCFactory, 0, nullptr);*/

        /*
         * Wait until all the unit tests are completed to avoid network replies
         * from httpbin.org in the RouterEngine::Planner. If you remove this statement,
         * wrong HTTP replies will land in the RouterEngine::Planner due the fact that the
         * Network::Manager is a singleton instance! The internal JSON validation
         * for the Linked Connections fragment will fail!
         *
         * REMARK: QTest::qWait() still allows processing for events, QTest::qSleep() does not!
         */
        //QTest::qWait(WAIT_TIME);

        // Run RouterEngine::Planner integration test
        //routerPlannerResult = QTest::qExec(&testSuiteCSAPlanner, 0, nullptr);

        /*
         * Wait until all the unit tests are completed to avoid network replies
         * from httpbin.org in the LiveboardEngine::Factory. If you remove this statement,
         * wrong HTTP replies will land in the LiveboardEngine::Factory due the fact that the
         * Network::Manager is a singleton instance! The internal JSON validation
         * for the Linked Connections fragment will fail!
         *
         * REMARK: QTest::qWait() still allows processing for events, QTest::qSleep() does not!
         */
        //QTest::qWait(WAIT_TIME);

        // Run LiveboardEngine::Factory integration test
        //liveboardFactoryResult = QTest::qExec(&testSuiteLiveboardFactory, 0, nullptr);

        /*
         * Wait until all the unit tests are completed to avoid network replies
         * from httpbin.org in the VehicleEngine::Factory. If you remove this statement,
         * wrong HTTP replies will land in the VehicleEngine::Factory due the fact that the
         * Network::Manager is a singleton instance! The internal JSON validation
         * for the Linked Connections fragment will fail!
         *
         * REMARK: QTest::qWait() still allows processing for events, QTest::qSleep() does not!
         */
        //QTest::qWait(WAIT_TIME);

        // Run LiveboardEngine::Factory integration test
        vehicleFactoryResult = QTest::qExec(&testSuiteVehicleFactory, 0, nullptr);

        // Return the status code of every test for CI/CD
        QCoreApplication::exit(networkManagerResult | dbManagerResult | lcFragmentResult | lcPageResult | lcFactoryResult | routerPlannerResult | liveboardFactoryResult | vehicleFactoryResult);
    });
    return app.exec();
}
