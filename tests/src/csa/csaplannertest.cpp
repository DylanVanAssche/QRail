#include "csaplannertest.h"

void CSA::PlannerTest::initCSAPlannerTest()
{
    // Get a Planner instance
    planner = CSA::Planner::getInstance();

    // Let the Qt meta object system know how it should handle our custom QObjects
    qRegisterMetaType< QList<CSA::Route*> >("QList<Route*>");

    // Connect the signals
    connect(planner, SIGNAL(routesFound(QList<CSA::Route*>)), this, SLOT(processRouteFound(QList<CSA::Route*>)));
    connect(planner, SIGNAL(pageProgress(QUrl, qint16)), this, SLOT(pageProcess(QUrl, qint16)));
    connect(planner, SIGNAL(pageReceived(QUrl)), this, SLOT(pageReceived(QUrl)));
    connect(planner, SIGNAL(pageRequested(QUrl)), this, SLOT(pageRequested(QUrl)));
}

void CSA::PlannerTest::runCSAPlannerTest()
{
    /*
     * Guess the arrival time given a departure time.
     * The arrival time must come after the departure time to be valid.
     */
    QDateTime arrivalTime = planner->calculateArrivalTime(QDateTime::currentDateTimeUtc());
    QVERIFY2(arrivalTime > QDateTime::currentDateTime(), "Arrival time can't be before the departure time!");

    /*
     * CSA routing: Find all the routes between 2 stations with a given departure time.
     * Trip: Vilvoorde -> Brugge
     * Number of transfers: 1 or 2
     * Possible transfer stations: Brussel-Noord, Brussel-Zuid, Gent-Sint-Pieters
     * LC2IRail equivalent: https://lc2irail.thesis.bertmarcelis.be/connections/008811189/008891009/departing/2018-08-02T13:00:00+00:00
     */

    QBENCHMARK_ONCE {
        planner->getConnections(
                    QUrl("http://irail.be/stations/NMBS/008811189"), // From: Vilvoorde
                    QUrl("http://irail.be/stations/NMBS/008891009"), // To: Brugge
                    QDateTime::fromString("2018-08-02T13:00:00.000Z", Qt::ISODate), // Departure time (UTC)
                    4 // Max transfers
                    );

        QEventLoop loop;
        connect(planner, SIGNAL(routesFound(QList<CSA::Route*>)), &loop, SLOT(quit()));
        loop.exec();
    }
}

void CSA::PlannerTest::cleanCSAPlannerTest()
{
    planner->deleteLater();
}

void CSA::PlannerTest::pageProcess(const QUrl &pageURI, const qint16 &progress)
{
    // Limit the logging of the page progress
    if(progress == 33 || progress == 67 || progress == 100) {
        qDebug() << "Page (" << pageURI.toString() << ") progress:" << progress << "%";
    }
}

void CSA::PlannerTest::pageReceived(const QUrl &pageURI)
{
    qDebug() << "Page received:" << pageURI.toString();
}

void CSA::PlannerTest::pageRequested(const QUrl &pageURI)
{
    qDebug() << "Page requested:" << pageURI.toString();
}

void CSA::PlannerTest::processRouteFound(const QList<CSA::Route *> &routes)
{
    qDebug() << "CSA found" << routes.size() << "possible routes";
    foreach(CSA::Route *route, routes) {
        // Verify the complete trip
        qDebug() << "Trip:" << route->departureStation()->station()->name().value(QLocale::Language::Dutch) << "->" << route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) << " Route:";
        QVERIFY2(route->departureStation()->station()->name().value(QLocale::Language::Dutch) == QString("Vilvoorde"), "Expected departure station: Vilvoorde");
        QVERIFY2(route->arrivalStation()->station()->name().value(QLocale::Language::Dutch) == QString("Brugge"), "Expected arrival station: Brugge");

        // Keep track of the transfer stations
        QStringList retrievedTransferStations;
        QStringList possibleTransferStations;
        possibleTransferStations << "Brussel-Noord" << "Brussel-Zuid" << "Gent-Sint-Pieters" << "Schaarbeek" << "Brussel-Centraal";

        // Log the complete trip to the console and verify it
        foreach(CSA::Transfer *transfer, route->transfers()) {
            if(transfer->type() == CSA::Transfer::Type::TRANSFER) {
                qDebug() << "TRANSFER:" << "Changing vehicle at" << transfer->time().time().toString("hh:mm") << transfer->station()->name().value(QLocale::Language::Dutch);
                retrievedTransferStations << transfer->station()->name().value(QLocale::Language::Dutch);
            }
            else if(transfer->type() == CSA::Transfer::Type::DEPARTURE) {
                qDebug() << "DEPARTURE:" << transfer->time().time().toString("hh:mm") << transfer->station()->name().value(QLocale::Language::Dutch);
                QVERIFY2(transfer->station()->name().value(QLocale::Language::Dutch) == QString("Vilvoorde"), "Expected departure station: Vilvoorde");
            }
            else if(transfer->type() == CSA::Transfer::Type::ARRIVAL) {
                qDebug() << "ARRIVAL:" << transfer->time().time().toString("hh:mm") << transfer->station()->name().value(QLocale::Language::Dutch);
                QVERIFY2(transfer->station()->name().value(QLocale::Language::Dutch) == QString("Brugge"), "Expected arrival station: Brugge");
            }
            else {
                QFAIL("Transfer object is INVALID");
            }
        }
    }
}
