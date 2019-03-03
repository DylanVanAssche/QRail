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
#include "networkeventsourcetest.h"
using namespace QRail;

void QRail::Network::EventSourceTest::initEventSource()
{
    m_eventSource = new QRail::Network::EventSource(QUrl("http://localhost:8080/sncb/events/sse?lastSyncTime=2019-03-03T08:30:00.000Z"));
    connect(m_eventSource, SIGNAL(messageReceived(QString)), this, SLOT(processMessage(QString)));
    connect(m_eventSource, SIGNAL(errorReceived(QString)), this, SLOT(processError(QString)));
    //connect(m_eventSource, SIGNAL(readyStateChanged(QRail::Network::EventSource::ReadyState)), this, SLOT(processReadyState(EventSource::ReadyState)));
}

void Network::EventSourceTest::runEventSource()
{
    qDebug() << "Waiting for SSE response...";
    QEventLoop loop;
    connect(m_eventSource, SIGNAL(messageReceived(QString)), &loop, SLOT(quit()));
    connect(m_eventSource, SIGNAL(errorReceived(QString)), &loop, SLOT(quit()));
    loop.exec(); // Initial update
    loop.exec(); // Delta update 1
    loop.exec(); // Delta update 2
    qDebug() << "SSE Response OK";
}


void Network::EventSourceTest::cleanEventSource()
{
    m_eventSource->close();
    delete m_eventSource;
}

void Network::EventSourceTest::processMessage(const QString msg)
{
    qInfo() << "Message:" << msg;
}

void Network::EventSourceTest::processError(QString err)
{
    qCritical() << "Error:" << err;
}

void Network::EventSourceTest::processReadyState(Network::EventSource::ReadyState state)
{
    switch(state) {
    case QRail::Network::EventSource::ReadyState::CLOSED:
        qInfo() << "ReadyState: CLOSED";
        break;
    case QRail::Network::EventSource::ReadyState::CONNECTING:
        qInfo() << "ReadyState: CONNECTING";
        break;
    case QRail::Network::EventSource::ReadyState::OPEN:
        qInfo() << "ReadyState: OPEN";
        break;
    default:
        qCritical() << "ReadyState: UNKNOWN";
        break;
    }
}


