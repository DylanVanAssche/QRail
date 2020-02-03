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
#include "networkmanagertest.h"
using namespace QRail;

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager init
 * Init Manager
 */
void QRail::Network::ManagerTest::initNetworkManager()
{
    qDebug() << "Init QRail::Network::Manager test";
    http = QRail::Network::Manager::getInstance();
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
void QRail::Network::ManagerTest::runNetworkManager()
{
    qDebug() << "Running QRail::Network::Manager test";

    // HTTP GET
    QNetworkReply *reply = http->getResource(QUrl("https://httpbin.org/get"));
    QEventLoop loop1;
    connect(reply, SIGNAL(finished()), &loop1, SLOT(quit()));
    loop1.exec();
    this->processHTTPReply(reply);

    // HTTP POST
    reply = http->postResource(QUrl("https://httpbin.org/post"), QByteArray("HTTP POST OK"));
    QEventLoop loop2;
    connect(reply, SIGNAL(finished()), &loop2, SLOT(quit()));
    loop2.exec();
    this->processHTTPReply(reply);

    // HTTP DELETE
    reply = http->deleteResource(QUrl("https://httpbin.org/delete"));
    QEventLoop loop3;
    connect(reply, SIGNAL(finished()), &loop3, SLOT(quit()));
    loop3.exec();
    this->processHTTPReply(reply);

    // HTTP HEAD
    reply = http->headResource(QUrl("https://httpbin.org/get"));
    QEventLoop loop4;
    connect(reply, SIGNAL(finished()), &loop4, SLOT(quit()));
    loop4.exec();
    this->processHTTPReply(reply);
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager clean up
 * Init Manager
 */
void QRail::Network::ManagerTest::cleanNetworkManager()
{
    qDebug() << "Cleaning up QRail::Network::Manager test";
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager reply handler
 * Handles the HTTP replies from the NetworkManager by checking the HTTP status
 * code and log it.
 */
void QRail::Network::ManagerTest::processHTTPReply(QNetworkReply *reply)
{
    QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(), 200); // HTTP 200 OK check
    qDebug() << reply->readAll();
}
