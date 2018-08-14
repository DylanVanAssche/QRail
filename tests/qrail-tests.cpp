/*
*   This file is part of BeRail.
*
*   BeRail is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   BeRail is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with BeRail.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <QtCore/QCoreApplication>
#include <QtTest/QtTest>

#include "network/networkmanagertest.h"
#include "database/databasemanagertest.h"
#include "fragments/fragmentsfragmenttest.h"
#include "fragments/fragmentspagetest.h"
#include "fragments/fragmentsfactorytest.h"
#include "csa/csaplannertest.h"

#define WAIT_TIME 5000

// Unittest main method
int main(int argc, char *argv[])
{
    /*
     * QtCreator tests setup:
     *   - CONFIG+=tests in the harbour-berail.pro file
     *   - Clone the debug kit and name it 'tests'
     *   - Select the tests kit in QtCreator
     *
     * TUTORIAL: http://xilexio.org/?p=125
     */

    // Allow QEventLoops for QSignalSpy
    QCoreApplication app(argc, argv);

    // Create test instances
    int networkManagerResult = -1;
    int dbManagerResult = -1;
    int lcFragmentResult = -1;
    int lcPageResult = -1;
    int lcFactoryResult = -1;
    int csaPlannerResult = -1;
    Network::ManagerTest testSuiteNetworkManager;
    Database::ManagerTest testSuiteDBManager;
    Fragments::FragmentTest testSuiteLCFragment;
    Fragments::PageTest testSuiteLCPage;
    Fragments::FactoryTest testSuiteLCFactory;
    CSA::PlannerTest testSuiteCSAPlanner;

    // Run unit tests
    networkManagerResult = QTest::qExec(&testSuiteNetworkManager, argc, argv);
    dbManagerResult = QTest::qExec(&testSuiteDBManager, argc, argv);
    lcFragmentResult = QTest::qExec(&testSuiteLCFragment, argc, argv);
    lcPageResult = QTest::qExec(&testSuiteLCPage, argc, argv);
    lcFactoryResult = QTest::qExec(&testSuiteLCFactory, argc, argv);

    /*
     * Wait until all the unit tests are completed to avoid network replies
     * from httpbin.org in the CSA::Planner. If you remove this statement,
     * wrong HTTP replies will land in the CSA::Planner due the fact that the
     * Network::Manager is a singleton instance! The internal JSON validation
     * for the Linked Connections fragment will fail!
     *
     * REMARK: QTest::qWait() still allows processing for events, QTest::qSleep() does not!
     */
    QTest::qWait(WAIT_TIME);

    // Run integration tests
    csaPlannerResult = QTest::qExec(&testSuiteCSAPlanner, argc, argv);

    // Return the status code of every test for CI/CD
    return networkManagerResult | dbManagerResult | lcFragmentResult | lcPageResult | lcFactoryResult | csaPlannerResult | app.exec();
}
