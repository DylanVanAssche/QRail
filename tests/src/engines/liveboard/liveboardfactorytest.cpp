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
}

void QRail::LiveboardEngine::FactoryTest::runLiveboardFactoryTest()
{
    qDebug() << "Running LiveboardEngine::Factory test";
    QBENCHMARK {
        factory->getLiveboardByStationURI(
            QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
            LiveboardEngine::Board::Mode::DEPARTURES);

        // Start an eventloop to wait for the routesFound signal to allow benchmarking of asynchronous events
        QEventLoop loop1;
        connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loop1, SLOT(quit()));
        loop1.exec();
    }

    /*QBENCHMARK {
        factory->getLiveboardByStationURI(
            QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
            LiveboardEngine::Board::Mode::ARRIVALS);

        // Start an eventloop to wait for the routesFound signal to allow benchmarking of asynchronous events
        QEventLoop loop2;
        connect(factory, SIGNAL(finished(QRail::LiveboardEngine::Board *)), &loop2, SLOT(quit()));
        loop2.exec();
    }*/
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
    qDebug() << "\tEntries:";

    foreach (QRail::VehicleEngine::Vehicle *entry, board->entries()) {
        if (board->mode() == QRail::LiveboardEngine::Board::Mode::ARRIVALS) {
            qDebug() << "\t\t" << entry->headsign() <<
                     entry->intermediaryStops().first()->arrivalTime().toString("hh:mm");
        } else if (board->mode() == QRail::LiveboardEngine::Board::Mode::DEPARTURES) {
            qDebug() << "\t\t" << entry->headsign() <<
                     entry->intermediaryStops().first()->departureTime().toString("hh:mm");
        } else {
            qCritical() << "Unknown QRail::LiveboardEngine::Board mode!";
        }
    }
}
