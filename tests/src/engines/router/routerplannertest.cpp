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
    qRegisterMetaType<QList<QRail::RouterEngine::Route *>>("QList<QRail::RouterEngine::Route*>");

    // Connect the signals
    connect(planner, SIGNAL(finished(QList<QRail::RouterEngine::Route *>)), this,
            SLOT(processRoutesFinished(QList<QRail::RouterEngine::Route *>)));
    connect(planner, SIGNAL(stream(QRail::RouterEngine::Route *)), this,
            SLOT(processRoutesStream(QRail::RouterEngine::Route *)));
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
    QVERIFY2(arrivalTime > QDateTime::currentDateTime(),
             "Arrival time can't be before the departure time!");

    /*
    * CSA routing: Find all the routes between 2 stations with a given departure
    * time. Trip: Vilvoorde -> Brugge Number of transfers: 1 or 2 Possible
    * transfer stations: Brussel-Noord, Brussel-Zuid, Gent-Sint-Pieters LC2IRail
    * equivalent:
    * https://lc2irail.thesis.bertmarcelis.be/connections/008811189/008891009/departing/2018-08-02T13:00:00+00:00
    */

    // Test abort
    planner->getConnections(
        QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
        QUrl("http://irail.be/stations/NMBS/008891009"), // To: Brugge
        QDateTime::currentDateTimeUtc(), // Departure time (UTC)
        4 // Max transfers
    );

    // Cancel operation
    planner->abortCurrentOperation();

    QTest::qSleep(3000); // 3 seconds should be sufficient to process the abort command

    QDateTime start = QDateTime::currentDateTime();
    planner->getConnections(
        QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
        QUrl("http://irail.be/stations/NMBS/008891009"), // To: Brugge
        QDateTime::currentDateTimeUtc(), // Departure time (UTC)
        4 // Max transfers
    );

    // Start an eventloop to wait for the finished signal to allow benchmarking of asynchronous events
    QEventLoop loop;
    connect(planner, SIGNAL(finished(QList<QRail::RouterEngine::Route *>)), &loop, SLOT(quit()));
    loop.exec();
    qInfo() << "Routing Vilvoorde -> Brugge took"
            << start.msecsTo(QDateTime::currentDateTime())
            << "msecs";
}

void QRail::RouterEngine::PlannerTest::cleanCSAPlannerTest()
{
    disconnect(planner, SIGNAL(finished(QList<QRail::RouterEngine::Route *>)),
               this, SLOT(processRoutesFinished(QList<QRail::RouterEngine::Route *>)));
    disconnect(planner, SIGNAL(processing(QUrl)),
               this, SLOT(processing(QUrl)));
    disconnect(planner, SIGNAL(requested(QUrl)),
               this, SLOT(requested(QUrl)));
    disconnect(planner, SIGNAL(stream(QRail::RouterEngine::Route *)),
               this, SLOT(processRoutesStream(QRail::RouterEngine::Route *)));
}

void QRail::RouterEngine::PlannerTest::processing(const QUrl &pageURI)
{
    qDebug() << "Page received:" << pageURI.toString();
}

void QRail::RouterEngine::PlannerTest::requested(const QUrl &pageURI)
{
    qDebug() << "Page requested:" << pageURI.toString();
}

void QRail::RouterEngine::PlannerTest::processRoutesFinished(const
                                                             QList<QRail::RouterEngine::Route *>
                                                             &routes)
{
    qDebug() << "CSA found" << routes.size() << "possible routes";
    foreach (QRail::RouterEngine::Route *route, routes) {
        // Verify the complete trip
        qDebug() << "Trip:" << route->departureStation()->station()->name().value(
                     QLocale::Language::Dutch) << "->" << route->arrivalStation()->station()->name().value(
                     QLocale::Language::Dutch) << " Route:";
        QVERIFY2(route->departureStation()->station()->name().value(QLocale::Language::Dutch) ==
                 QString("Vilvoorde"), "Expected departure station: Vilvoorde");
        QVERIFY2(route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) ==
                 QString("Brugge"), "Expected arrival station: Brugge");

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
                qDebug() << "TRANSFER:" << "Changing vehicle at" << transfer->time().time().toString("hh:mm") <<
                         transfer->station()->name().value(QLocale::Language::Dutch);
                retrievedTransferStations << transfer->station()->name().value(QLocale::Language::Dutch);
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
                qDebug() << "DEPARTURE:" << transfer->time().time().toString("hh:mm") <<
                         transfer->station()->name().value(QLocale::Language::Dutch);
                QVERIFY2(transfer->station()->name().value(QLocale::Language::Dutch) == QString("Vilvoorde"),
                         "Expected departure station: Vilvoorde");
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
                qDebug() << "ARRIVAL:" << transfer->time().time().toString("hh:mm") <<
                         transfer->station()->name().value(QLocale::Language::Dutch);
                QVERIFY2(transfer->station()->name().value(QLocale::Language::Dutch) == QString("Brugge"),
                         "Expected arrival station: Brugge");
            } else {
                QFAIL("Transfer object is INVALID");
            }
        }
    }
    qDebug() << "All routes processed";
}

void RouterEngine::PlannerTest::processRoutesStream(QRail::RouterEngine::Route *route)
{
    // Log the complete trip to the console and verify it
    foreach (QRail::RouterEngine::Transfer *transfer, route->transfers()) {
        if (transfer->type() == QRail::RouterEngine::Transfer::Type::TRANSFER) {
            qDebug() << "TRANSFER:" << "Changing vehicle at" << transfer->time().time().toString("hh:mm") <<
                     transfer->station()->name().value(QLocale::Language::Dutch);
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
            qDebug() << "DEPARTURE:" << transfer->time().time().toString("hh:mm") <<
                     transfer->station()->name().value(QLocale::Language::Dutch);
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
            qDebug() << "ARRIVAL:" << transfer->time().time().toString("hh:mm") <<
                     transfer->station()->name().value(QLocale::Language::Dutch);
        }
    }
}
