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
#ifndef NETWORKEVENTSOURCETEST_H
#define NETWORKEVENTSOURCETEST_H

#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtTest/QtTest>

#include "network/networkeventsource.h"

#define NETWORK_WAIT_TIME 3000

namespace QRail {
namespace Network {
class EventSourceTest : public QObject
{
    Q_OBJECT
private slots:
    void initEventSource();
    void runEventSource();
    void cleanEventSource();

public slots:
    void processMessage(QString msg);
    void processError(QString err);
    void processReadyState(EventSource::ReadyState state);

private:
    QRail::Network::EventSource *m_sse;
    QRail::Network::EventSource *m_polling;
};
}
}

#endif // NETWORKEVENTSOURCETEST_H
