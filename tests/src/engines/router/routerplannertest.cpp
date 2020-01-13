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
#include "routerplannertest.h"
using namespace QRail;

void QRail::RouterEngine::PlannerTest::initCSAPlannerTest()
{
    // Get a Planner instance
    planner = QRail::RouterEngine::Planner::getInstance();

    // Let the Qt meta object system know how it should handle our custom QObjects
    qRegisterMetaType<QList<QSharedPointer<QRail::RouterEngine::Route> > >("QList<QRail::RouterEngine::Route*>");

    // Connect the signals
    connect(planner, SIGNAL(stream(QSharedPointer<QRail::RouterEngine::Route>)), this, SLOT(processRoutesStream(QSharedPointer<QRail::RouterEngine::Route>)));
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), this, SLOT(processRoutesFinished(QRail::RouterEngine::Journey*)));
    connect(planner, SIGNAL(processing(QUrl)), this, SLOT(processing(QUrl)));
    connect(planner, SIGNAL(requested(QUrl)), this, SLOT(requested(QUrl)));
}

void QRail::RouterEngine::PlannerTest::runCSAPlannerTest()
{
    /*
    * Guess the arrival time given a departure time.
    * The arrival time must come after the departure time to be valid.
    */
    QDateTime arrivalTime = planner->calculateArrivalTime(QDateTime::currentDateTimeUtc());
    QVERIFY2(arrivalTime > QDateTime::currentDateTimeUtc(), "Arrival time can't be before the departure time!");
    QDateTime start;

    /*
    * CSA routing: Find all the routes between 2 stations with a given departure
    * time. Trip: Vilvoorde -> Brugge Number of transfers: 1 or 2 Possible
    * transfer stations: Brussel-Noord, Brussel-Zuid, Gent-Sint-Pieters LC2IRail
    * equivalent:
    * https://lc2irail.thesis.bertmarcelis.be/connections/008811189/008891009/departing/2018-08-02T13:00:00+00:00
    */

   /* qDebug() << "---------------------------------------------- ROUTING PREFETCH ----------------------------------------------";

    start = QDateTime::currentDateTimeUtc();
    QEventLoop loopPrefetch;
    connect(planner->fragmentsFactory(), SIGNAL(prefetchFinished()), &loopPrefetch, SLOT(quit()));
    loopPrefetch.exec();

    qInfo() << "Prefetching took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";
*/
    qDebug() << "---------------------------------------------- ROUTING ABORT ----------------------------------------------";
    // Test abort
    /*planner->getConnections(
        QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
        QUrl("http://irail.be/stations/NMBS/008891009"), // To: Brugge
        QDateTime::currentDateTimeUtc(), // Departure time (UTC)
        4 // Max transfers
    );

    // Cancel operation
    planner->abortCurrentOperation();
    QEventLoop loopAbort;
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loopAbort, SLOT(quit()));
    loopAbort.exec();

    // Now we should get valid data, connect the processor with verification checks
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey *)), this, SLOT(processRoutesFinished(QRail::RouterEngine::Journey *)));*/

    qDebug() << "---------------------------------------------- ROUTING NETWORK ----------------------------------------------";

    start = QDateTime::currentDateTimeUtc();
    planner->getConnections(
        QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
        QUrl("http://irail.be/stations/NMBS/008891009"), // To: Brugge
        QDateTime::fromString("2020-01-15T14:00:00.000Z", Qt::ISODate), // Departure time (UTC)
        4 // Max transfers
    );
    /*planner->getConnections(
        QUrl("http://irail.be/stations/NMBS/008813003"), // From: Brussel-Central
        QUrl("http://irail.be/stations/NMBS/008844628"), // To: Eupen
        QDateTime::currentDateTimeUtc(), // Departure time (UTC)
        4 // Max transfers
    );*/


    QEventLoop loop1;
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loop1, SLOT(quit()));
    loop1.exec();
    qInfo() << "Network routing Vilvoorde -> Brugge took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";

    qDebug() << "---------------------------------------------- REROUTING ----------------------------------------------";

    /*// Start an eventloop to wait for the finished signal to allow benchmarking of asynchronous events
    QEventLoop loop3;
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loop3, SLOT(quit()));
    loop3.exec();
    qInfo() << "Rerouting Brussels-Central -> Antwerp-Central took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";

    // Unwatch again
    planner->unwatch(this->journey);*/

    qDebug() << "---------------------------------------------- CACHED ROUTING ----------------------------------------------";

    start = QDateTime::currentDateTimeUtc();
    planner->getConnections(
        QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
        QUrl("http://irail.be/stations/NMBS/008891009"), // To: Brugge
        QDateTime::currentDateTimeUtc(), // Departure time (UTC)
        4 // Max transfers
    );

    // Start an eventloop to wait for the finished signal to allow benchmarking of asynchronous events
    QEventLoop loop2;
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loop2, SLOT(quit()));
    loop2.exec();
    qInfo() << "Cached routing Vilvoorde -> Brugge took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";

    /*for(int i=0; i < REPEAT_COUNT; i++) {
        QDateTime start = QDateTime::currentDateTimeUtc();
        planner->getConnections(
            QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
            QUrl("http://irail.be/stations/NMBS/008892007"), // To: Gent-Sint-Pieters
            QDateTime::currentDateTimeUtc(), // Departure time (UTC)
            4 // Max transfers
        );

        // Start an eventloop to wait for the finished signal to allow benchmarking of asynchronous events
        QEventLoop loop3;
        connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loop3, SLOT(quit()));
        loop3.exec();
        qInfo() << "Iteration:" << i
                << "Routing Vilvoorde -> Gent-Sint-Pieters took"
                << start.msecsTo(QDateTime::currentDateTimeUtc())
                << "msecs";
    }*/
}

void QRail::RouterEngine::PlannerTest::cleanCSAPlannerTest()
{
    disconnect(planner,
               SIGNAL(finished(QRail::RouterEngine::Journey*)),
               this,
               SLOT(processRoutesFinished(QRail::RouterEngine::Journey *)));
    disconnect(planner,
               SIGNAL(processing(QUrl)),
               this,
               SLOT(processing(QUrl)));
    disconnect(planner,
               SIGNAL(requested(QUrl)),
               this,
               SLOT(requested(QUrl)));
    disconnect(planner,
               SIGNAL(stream(QSharedPointer<QRail::RouterEngine::Route>)),
               this,
               SLOT(processRoutesStream(QSharedPointer<QRail::RouterEngine::Route>)));
}

void QRail::RouterEngine::PlannerTest::processing(const QUrl &pageURI)
{
    qDebug() << "Page received:" << pageURI.toString();
}

void QRail::RouterEngine::PlannerTest::requested(const QUrl &pageURI)
{
    qDebug() << "Page requested:" << pageURI.toString();
}

void QRail::RouterEngine::PlannerTest::processRoutesFinished(QRail::RouterEngine::Journey *journey)
{
    //planner->watch(journey);
    qDebug() << "JOURNEY RECEIVED:" << journey;
    qDebug() << "CSA found" << journey->routes().size() << "possible routes";
    QVERIFY2(journey->routes().size() > 0, "CSA couldn't find any routes, this is impossible in our integration test!");
    foreach (QSharedPointer<QRail::RouterEngine::Route> route, journey->routes()) {
        // Verify the complete trip
        qDebug() << "Trip:"
                 << route->departureStation()->station()->name().value(QLocale::Language::Dutch)
                 << "->"
                 << route->arrivalStation()->station()->name().value(QLocale::Language::Dutch)
                 << " Route:";

        QVERIFY2(route->departureStation()->station()->name().value(QLocale::Language::Dutch) == QString("Vilvoorde")
                 || route->departureStation()->station()->name().value(QLocale::Language::Dutch) == QString("Diegem"),
                 "Expected departure station: Vilvoorde or Diegem");
        QVERIFY2(route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) == QString("Brugge")
                 || route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) == QString("Gent-Sint-Pieters"),
                 "Expected arrival station: Brugge or Gent-Sint-Pieters");

        // Keep track of the transfer stations
        QStringList retrievedTransferStations;
        QStringList possibleTransferStations;
        possibleTransferStations << "Brussel-Noord"
                                 << "Brussel-Zuid"
                                 << "Gent-Sint-Pieters"
                                 << "Schaarbeek"
                                 << "Brussel-Centraal";

        // Log the complete trip to the console and verify it
        foreach (QRail::RouterEngine::Transfer *transfer, route->transfers()) {
            if (transfer->type() == QRail::RouterEngine::Transfer::Type::TRANSFER) {
                qDebug() << "TRANSFER:"
                         << "Changing vehicle at"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch);
                retrievedTransferStations << transfer->station()->name().value(QLocale::Language::Dutch);
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
                qDebug() << "DEPARTURE:"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch);
                QVERIFY2(route->departureStation()->station()->name().value(QLocale::Language::Dutch) == QString("Vilvoorde")
                         || route->departureStation()->station()->name().value(QLocale::Language::Dutch) == QString("Diegem"),
                         "Expected departure station: Vilvoorde or Diegem");
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
                qDebug() << "ARRIVAL:"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch);
                QVERIFY2(route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) == QString("Brugge")
                         || route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) == QString("Gent-Sint-Pieters"),
                         "Expected arrival station: Brugge or Gent-Sint-Pieters");
            } else {
                QFAIL("Transfer object is INVALID");
            }
        }
    }
    qDebug() << "All routes processed";
}

void RouterEngine::PlannerTest::processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route)
{
    // Log the complete trip to the console and verify it
    foreach (QRail::RouterEngine::Transfer *transfer, route->transfers()) {
        if (transfer->type() == QRail::RouterEngine::Transfer::Type::TRANSFER) {
            qDebug() << "TRANSFER:"
                     << "Changing vehicle at"
                     << transfer->time().time().toString("hh:mm")
                     << transfer->station()->name().value(QLocale::Language::Dutch);
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
            qDebug() << "DEPARTURE:"
                     << transfer->time().time().toString("hh:mm")
                     << transfer->station()->name().value(QLocale::Language::Dutch);
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
            qDebug() << "ARRIVAL:"
                     << transfer->time().time().toString("hh:mm")
                     << transfer->station()->name().value(QLocale::Language::Dutch);
        }
    }
}
