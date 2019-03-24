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
#include "liveboardfactorytest.h"
using namespace QRail;

void LiveboardEngine::FactoryTest::initLiveboardFactoryTest()
{
    qDebug() << "Init LiveboardEngine::Factory test";
    factory = LiveboardEngine::Factory::getInstance();
    connect(factory,
            SIGNAL(finished(QRail::LiveboardEngine::Board *)),
            this,
            SLOT(liveboardReceived(QRail::LiveboardEngine::Board *)));
    connect(factory,
            SIGNAL(stream(QRail::VehicleEngine::Vehicle *)),
            this,
            SLOT(liveboardStreamReceived(QRail::VehicleEngine::Vehicle *)));
}

void QRail::LiveboardEngine::FactoryTest::runLiveboardFactoryTest()
{
    qDebug() << "Running LiveboardEngine::Factory test";
    QDateTime start;

    qDebug() << "---------------------------------------------- ABORT LIVEBOARD ----------------------------------------------";

    /*// Testing abort
    start = QDateTime::currentDateTimeUtc();
    qInfo() << "Testing abort";
    factory->getLiveboardByStationURI(
        QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
        LiveboardEngine::Board::Mode::DEPARTURES);

    // Cancel operation
    factory->abortCurrentOperation();

    QEventLoop loopAbort;
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loopAbort, SLOT(quit()));
    loopAbort.exec();*/

    qDebug() << "---------------------------------------------- CREATING LIVEBOARD ----------------------------------------------";

    // Creating liveboard
    start = QDateTime::currentDateTimeUtc();
    factory->getLiveboardByStationURI(
        QUrl("http://irail.be/stations/NMBS/008813003"), // Brussels-Central, the most events generating station of the SNCB
        QDateTime::currentDateTimeUtc(),
        QDateTime::currentDateTimeUtc().addSecs(3600 * 8),
        LiveboardEngine::Board::Mode::DEPARTURES);

    // Start an eventloop to wait for the finished signal to allow benchmarking of asynchronous events
    QEventLoop loop;
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loop, SLOT(quit()));
    loop.exec();
    qInfo() << "Liveboard Vilvoorde DEPARTURES took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";

    qDebug() << "---------------------------------------------- UPDATE RECEIVED LIVEBOARD ----------------------------------------------";
    factory->watch(liveboard);
    QEventLoop loopUpdateReceived;
    //connect(liveboard, SIGNAL(entriesChanged()), &loopUpdateReceived, SLOT(quit()));
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loopUpdateReceived, SLOT(quit()));
    loopUpdateReceived.exec();
    factory->unwatch(liveboard);

    qDebug() << "---------------------------------------------- CACHED LIVEBOARD ----------------------------------------------";

    start = QDateTime::currentDateTimeUtc();
    factory->getLiveboardByStationURI(
        QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
        LiveboardEngine::Board::Mode::DEPARTURES);

    // Start an eventloop to wait for the finished signal to allow benchmarking of asynchronous events
    QEventLoop loopCached;
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loopCached, SLOT(quit()));
    loopCached.exec();
    qInfo() << "Liveboard Vilvoorde DEPARTURES CACHED took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";

    qDebug() << "---------------------------------------------- NEXT RESULTS ----------------------------------------------";

    // Extending liveboard with next results
    start = QDateTime::currentDateTimeUtc();
    factory->getNextResultsForLiveboard(liveboard);

    // Start an eventloop to wait for the routesFound signal to allow benchmarking of asynchronous events
    QEventLoop loopNext;
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loopNext, SLOT(quit()));
    loopNext.exec();
    qInfo() << "Liveboard Vilvoorde DEPARTURES extending NEXT took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";

    qDebug() << "---------------------------------------------- PREVIOUS RESULTS ----------------------------------------------";

    // Extending liveboard with previous results
    start = QDateTime::currentDateTimeUtc();
    factory->getPreviousResultsForLiveboard(liveboard);

    // Start an eventloop to wait for the routesFound signal to allow benchmarking of asynchronous events
    QEventLoop loopPrevious;
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loopPrevious, SLOT(quit()));
    loopPrevious.exec();
    qInfo() << "Liveboard Vilvoorde DEPARTURES extending PREVIOUS took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";
}

void QRail::LiveboardEngine::FactoryTest::cleanLiveboardFactoryTest()
{
    qDebug() << "Cleaning up QRail::LiveboardEngine::Factory test";
}

void QRail::LiveboardEngine::FactoryTest::liveboardReceived(QRail::LiveboardEngine::Board *board)
{
    liveboard = board;
    qDebug() << "Received liveboard from QRail::LiveboardEngine::Factory for station" <<
             board->station()->name().value(QLocale::Language::Dutch);
    qDebug() << "\tFrom:" << board->from();
    qDebug() << "\tUntil:" << board->until();
    qDebug() << "\tMode:" << board->mode();
    qDebug() << "\tNumber of entries:" << board->entries().size();
    foreach(QRail::VehicleEngine::Vehicle *entry, board->entries()) {
        qDebug() << "\t\t"
                 << entry->headsign()
                 << entry->intermediaryStops().first()->arrivalTime().toString("hh:mm")
                 << "+"
                 << entry->intermediaryStops().first()->arrivalDelay() / 60
                 << "|"
                 << entry->intermediaryStops().first()->departureTime().toString("hh:mm")
                 << "+"
                 << entry->intermediaryStops().first()->departureDelay() / 60;
    }
    liveboard = board;
}

void LiveboardEngine::FactoryTest::liveboardStreamReceived(QRail::VehicleEngine::Vehicle *entry)
{
    qDebug() << "\t\t"
             << entry->uri()
             << entry->headsign()
             << entry->intermediaryStops().first()->arrivalTime().toString("hh:mm")
             << "+"
             << entry->intermediaryStops().first()->arrivalDelay() / 60
             << "|"
             << entry->intermediaryStops().first()->departureTime().toString("hh:mm")
             << "+"
             << entry->intermediaryStops().first()->departureDelay() / 60;
}
