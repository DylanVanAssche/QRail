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
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), this,
            SLOT(liveboardReceived(QRail::LiveboardEngine::Board *)));
    connect(factory, SIGNAL(stream(QRail::VehicleEngine::Vehicle *)), this,
            SLOT(liveboardStreamReceived(QRail::VehicleEngine::Vehicle *)));
}

void QRail::LiveboardEngine::FactoryTest::runLiveboardFactoryTest()
{
    qDebug() << "Running LiveboardEngine::Factory test";
    QDateTime start = QDateTime::currentDateTime();
    factory->getLiveboardByStationURI(
        QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
        LiveboardEngine::Board::Mode::DEPARTURES);

    // Start an eventloop to wait for the routesFound signal to allow benchmarking of asynchronous events
    QEventLoop loop;
    connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loop, SLOT(quit()));
    loop.exec();

    qInfo() << "Liveboard Vilvoorde DEPARTURES took"
            << start.msecsTo(QDateTime::currentDateTime())
            << "msecs";
}

void QRail::LiveboardEngine::FactoryTest::cleanLiveboardFactoryTest()
{
    qDebug() << "Cleaning up QRail::LiveboardEngine::Factory test";
}

void QRail::LiveboardEngine::FactoryTest::liveboardReceived(QRail::LiveboardEngine::Board *board)
{
    qDebug() << "Received liveboard from QRail::LiveboardEngine::Factory for station" <<
             board->station()->name().value(QLocale::Language::Dutch);
    qDebug() << "\tFrom:" << board->from();
    qDebug() << "\tUntil:" << board->until();
    qDebug() << "\tMode:" << board->mode();
    qDebug() << "\tNumber of entries:" << board->entries().size();
}

void LiveboardEngine::FactoryTest::liveboardStreamReceived(QRail::VehicleEngine::Vehicle *entry)
{
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
