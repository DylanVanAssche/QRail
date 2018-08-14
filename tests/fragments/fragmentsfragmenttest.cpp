/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of BeRail.                                               *
 *                                                                            *
 *   BeRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   BeRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with BeRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#include "fragmentsfragmenttest.h"
#define SIGNAL_WAIT_TIME 1000

/*
 * Based on iRail graph from https://graph.irail.be/sncb/connections?departureTime=2018-07-21T07:29:00.000Z
 * If no delays are encountered for a connection, those fields are not available.
 * When there's a delay, the gtfs:pickupType and gtfs:dropOffType fields are not available.
 *
 * No delay:
 * @graph[0]
 * {
 *  "@id": "http://irail.be/connections/8822004/20180721/IC3108",
 *  "@type": "Connection",
 *  "departureStop": "http://irail.be/stations/NMBS/008822004",
 *  "arrivalStop": "http://irail.be/stations/NMBS/008822343",
 *  "departureTime": "2018-07-21T07:29:00.000Z",
 *  "arrivalTime": "2018-07-21T07:32:00.000Z",
 *  "direction": "Anvers-Central",
 *  "gtfs:trip": "http://irail.be/vehicle/IC3108/20180721",
 *  "gtfs:route": "http://irail.be/vehicle/IC3108",
 *  "gtfs:pickupType": "gtfs:Regular",
 *  "gtfs:dropOffType": "gtfs:Regular"
 * }
 *
 * With delay:
 * @graph[1]
 * {
 *  "@id": "http://irail.be/connections/8814001/20180721/IC3309",
 *  "@type": "Connection",
 *  "departureStop": "http://irail.be/stations/NMBS/008814001",
 *  "arrivalStop": "http://irail.be/stations/NMBS/008813037",
 *  "departureTime": "2018-07-21T07:29:00.000Z",
 *  "arrivalTime": "2018-07-21T07:31:00.000Z",
 *  "direction": "Anvers-Central",
 *  "departureDelay": 0,
 *  "arrivalDelay": 60,
 *  "gtfs:trip": "http://irail.be/vehicle/IC3309/20180721",
 *  "gtfs:route": "http://irail.be/vehicle/IC3309"
 * }
 */

void Fragments::FragmentTest::initLinkedConnectionFragment()
{
    qDebug() << "Init Fragments::Fragment test";

    lcWithoutDelay = new Fragments::Fragment(this);
    lcWithDelay = new Fragments::Fragment(
                QUrl("http://irail.be/connections/8814001/20180721/IC3309"),
                QUrl("http://irail.be/stations/NMBS/008814001"),
                QUrl("http://irail.be/stations/NMBS/008813037"),
                QDateTime::fromString("2018-07-21T07:29:00.000Z", Qt::ISODate),
                QDateTime::fromString("2018-07-21T07:31:00.000Z", Qt::ISODate),
                0,
                60,
                QUrl("http://irail.be/vehicle/IC3309/20180721"),
                QUrl("http://irail.be/vehicle/IC3309"),
                QString("Anvers-Central"),
                this
                );
}

void Fragments::FragmentTest::runLinkedConnectionFragment()
{
    qDebug() << "Running Fragments::Fragment test";

    // Set each property and check if the signal of the property fires
    QSignalSpy spyUriChanged(lcWithoutDelay, SIGNAL(uriChanged()));
    lcWithoutDelay->setURI(QUrl("http://irail.be/connections/8822004/20180721/IC3108"));
    QCOMPARE(spyUriChanged.count(), 1);

    QSignalSpy spyDepartureStationURIChanged(lcWithoutDelay, SIGNAL(departureStationURIChanged()));
    lcWithoutDelay->setDepartureStationURI(QUrl("http://irail.be/stations/NMBS/008822004"));
    QCOMPARE(spyDepartureStationURIChanged.count(), 1);

    QSignalSpy spyArrivalStationURIChanged(lcWithoutDelay, SIGNAL(arrivalStationURIChanged()));
    lcWithoutDelay->setArrivalStationURI(QUrl("http://irail.be/stations/NMBS/008822343"));
    QCOMPARE(spyArrivalStationURIChanged.count(), 1);

    QSignalSpy spyDepartureTimeChanged(lcWithoutDelay, SIGNAL(departureTimeChanged()));
    lcWithoutDelay->setDepartureTime(QDateTime::fromString("2018-07-21T07:29:00.000Z", Qt::ISODate));
    QCOMPARE(spyDepartureTimeChanged.count(), 1);

    QSignalSpy spyArrivalTimeChanged(lcWithoutDelay, SIGNAL(arrivalTimeChanged()));
    lcWithoutDelay->setArrivalTime(QDateTime::fromString("2018-07-21T07:32:00.000Z", Qt::ISODate));
    QCOMPARE(spyArrivalTimeChanged.count(), 1);

    QSignalSpy spyDepartureDelayChanged(lcWithoutDelay, SIGNAL(departureDelayChanged()));
    lcWithoutDelay->setDepartureDelay(0);
    QCOMPARE(spyDepartureDelayChanged.count(), 1);

    QSignalSpy spyArrivalDelayChanged(lcWithoutDelay, SIGNAL(arrivalDelayChanged()));
    lcWithoutDelay->setArrivalDelay(0);
    QCOMPARE(spyArrivalDelayChanged.count(), 1);

    QSignalSpy spyTripURIChanged(lcWithoutDelay, SIGNAL(tripURIChanged()));
    lcWithoutDelay->setTripURI(QUrl("http://irail.be/vehicle/IC3108/20180721"));
    QCOMPARE(spyTripURIChanged.count(), 1);

    QSignalSpy spyRouteURIChanged(lcWithoutDelay, SIGNAL(routeURIChanged()));
    lcWithoutDelay->setRouteURI(QUrl("http://irail.be/vehicle/IC3108"));
    QCOMPARE(spyRouteURIChanged.count(), 1);

    QSignalSpy spyDirectionChanged(lcWithoutDelay, SIGNAL(directionChanged()));
    lcWithoutDelay->setDirection(QString("Anvers-Central"));
    QCOMPARE(spyDirectionChanged.count(), 1);
}

void Fragments::FragmentTest::cleanLinkedConnectionFragment()
{
    qDebug() << "Cleaning up Fragments::Fragment test";

    lcWithoutDelay->deleteLater();
    lcWithDelay->deleteLater();
}
