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

#ifndef NetworkManagerTEST_H
#define NetworkManagerTEST_H

#include <QtCore/QObject>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include "../src/network/networkmanager.h"

#define SIGNAL_WAIT_TIME 3000

namespace Network {
class ManagerTest : public QObject
{
    Q_OBJECT
private slots:
    void initNetworkManager();
    void runNetworkManager();
    void cleanNetworkManager();

public slots:
    void processReply(QNetworkReply *reply);

private:
    Network::Manager *http;
};
}

#endif // NetworkManagerTEST_H
