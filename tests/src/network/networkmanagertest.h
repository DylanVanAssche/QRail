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
#ifndef NetworkManagerTEST_H
#define NetworkManagerTEST_H

#include "network/networkmanager.h"
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtTest/QtTest>

#define NETWORK_WAIT_TIME 3000

namespace QRail {
namespace Network {
class ManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void initNetworkManager();
    void runNetworkManager();
    void cleanNetworkManager();

protected:
    virtual void customEvent(QEvent *event);

private:
    QRail::Network::Manager *http;
    void processHTTPReply(QNetworkReply *reply);
};
} // namespace Network
} // namespace QRail

#endif // NetworkManagerTEST_H
