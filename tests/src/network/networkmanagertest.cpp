/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of QRail.                                               *
 *                                                                            *
 *   QRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   QRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with QRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#include "networkmanagertest.h"

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager init
 * Init Manager
 */
void Network::ManagerTest::initNetworkManager()
{
    qDebug() << "Init Network::Manager test";

    http = Network::Manager::getInstance();
    //connect(http, SIGNAL(requestCompleted(QNetworkReply *)), this, SLOT(processReply(QNetworkReply *)));
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager run tests
 * Run NetworkManager tests:
 *  - GET request
 *  - POST request
 *  - DELETE request
 *  - HEAD request
 */
void Network::ManagerTest::runNetworkManager()
{
    qDebug() << "Running Network::Manager test";

    // Activate QSignalSpy
    //QSignalSpy spy(http, SIGNAL(requestCompleted(QNetworkReply *)));

    // HTTP GET
    http->getResource(QUrl("https://httpbin.org/get"), this);
    // Wait for signal
    //QVERIFY(spy.wait(NETWORK_WAIT_TIME));

    // HTTP POST
    http->postResource(QUrl("https://httpbin.org/post"), QByteArray("HTTP POST OK"), this);
    // Wait for signal
    //QVERIFY(spy.wait(NETWORK_WAIT_TIME));

    // HTTP DELETE
    http->deleteResource(QUrl("https://httpbin.org/delete"), this);
    // Wait for signal
    //QVERIFY(spy.wait(NETWORK_WAIT_TIME));

    // HTTP HEAD
    http->headResource(QUrl("https://httpbin.org/get"), this);
    // Wait for signal
    //QVERIFY(spy.wait(NETWORK_WAIT_TIME));
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager clean up
 * Init Manager
 */
void Network::ManagerTest::cleanNetworkManager()
{
    qDebug() << "Cleaning up Network::Manager test";

    //disconnect(http, SIGNAL(requestCompleted(QNetworkReply *)), this, SLOT(processReply(QNetworkReply *)));
}

void Network::ManagerTest::customEvent(QEvent *event)
{
    if(event->type() == http->dispatcher()->eventType())
    {
        qDebug() << "Received Network::Dispatcher event in VehicleEngine::Factory, casting now!";
        event->accept();
        Network::DispatcherEvent *networkEvent = reinterpret_cast<Network::DispatcherEvent *>(event);
        this->processHTTPReply(networkEvent->reply());
    }
    else {
        qDebug() << "Unwanted event in VehicleEngine::Factory, ignoring...";
        event->ignore();
    }
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager reply handler
 * Handles the HTTP replies from the NetworkManager by checking the HTTP status code and log it.
 */
void Network::ManagerTest::processHTTPReply(QNetworkReply *reply)
{
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 200); // HTTP 200 OK check
    qDebug() << reply->readAll();
}
