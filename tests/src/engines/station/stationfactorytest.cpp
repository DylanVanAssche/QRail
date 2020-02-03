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
#include "stationfactorytest.h"
using namespace QRail;


void StationEngine::FactoryTest::initStationFactoryTest()
{
    qDebug() << "Init StationEngine::FactoryTest";
    factory = QRail::StationEngine::Factory::getInstance();
}

void StationEngine::FactoryTest::runStationFactoryTest()
{
    qDebug() << "Running StationEngine::FactoryTest";
    QSharedPointer<QRail::StationEngine::Station> station = factory->getStationByURI(QUrl("http://irail.be/stations/NMBS/008811189"));
    QVERIFY2(station->name().value(QLocale::Language::Dutch) == QString("Vilvoorde"), "Station factory returned wrong station for URI 008811189");
    qDebug() << "Station 008811189 is" << station->name().value(QLocale::Language::Dutch);

    QList<QSharedPointer<QRail::StationEngine::Station>> stations = factory->getStationsByName("Bruss");
    QVERIFY2(stations.count() > 0, "Fuzzy search must show at least 1 station");

    qDebug() << "Found" << stations.count() << " stations using fuzzy match:";
    foreach (QSharedPointer<QRail::StationEngine::Station> station, stations) {
        qDebug() << "\t" << station->name().value(QLocale::Language::Dutch);
    }
}

void StationEngine::FactoryTest::cleanStationFactoryTest()
{
    qDebug() << "Cleaning StationEngine::FactoryTest";
}
