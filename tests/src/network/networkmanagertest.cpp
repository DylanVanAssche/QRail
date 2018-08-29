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
void QRail::Network::ManagerTest::initNetworkManager() {
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
void QRail::Network::ManagerTest::runNetworkManager() {
  qDebug() << "Running QRail::Network::Manager test";

  // Activate QSignalSpy
  // QSignalSpy spy(http, SIGNAL(requestCompleted(QNetworkReply *)));

  // HTTP GET
  http->getResource(QUrl("https://httpbin.org/get"), this);

  // HTTP POST
  http->postResource(QUrl("https://httpbin.org/post"),
                     QByteArray("HTTP POST OK"), this);

  // HTTP DELETE
  http->deleteResource(QUrl("https://httpbin.org/delete"), this);

  // HTTP HEAD
  http->headResource(QUrl("https://httpbin.org/get"), this);
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager clean up
 * Init Manager
 */
void QRail::Network::ManagerTest::cleanNetworkManager() {
  qDebug() << "Cleaning up QRail::Network::Manager test";
}

void QRail::Network::ManagerTest::customEvent(QEvent *event) {
  if (event->type() == http->dispatcher()->eventType()) {
    qDebug() << "Received QRail::Network::Dispatcher event in "
                "QRail::VehicleEngine::Factory, "
                "casting now!";
    event->accept();
    QRail::Network::DispatcherEvent *networkEvent =
        reinterpret_cast<QRail::Network::DispatcherEvent *>(event);
    this->processHTTPReply(networkEvent->reply());
  } else {
    qDebug() << "Unwanted event in QRail::VehicleEngine::Factory, ignoring...";
    event->ignore();
  }
}

/**
 * @file NetworkManagertest.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Manager reply handler
 * Handles the HTTP replies from the NetworkManager by checking the HTTP status
 * code and log it.
 */
void QRail::Network::ManagerTest::processHTTPReply(QNetworkReply *reply) {
  QCOMPARE(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt(),
           200); // HTTP 200 OK check
  qDebug() << reply->readAll();
}
