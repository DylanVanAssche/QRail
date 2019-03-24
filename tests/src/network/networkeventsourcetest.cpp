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
    m_sse = new QRail::Network::EventSource(QUrl("https://lc.dylanvanassche.be/sncb/events"), QRail::Network::EventSource::Subscription::SSE);
    connect(m_sse, SIGNAL(messageReceived(QString)), this, SLOT(processMessage(QString)));
    connect(m_sse, SIGNAL(errorReceived(QString)), this, SLOT(processError(QString)));
    m_polling = new QRail::Network::EventSource(QUrl("https://lc.dylanvanassche.be/sncb/events"), QRail::Network::EventSource::Subscription::POLLING);
    connect(m_polling, SIGNAL(messageReceived(QString)), this, SLOT(processMessage(QString)));
    connect(m_polling, SIGNAL(errorReceived(QString)), this, SLOT(processError(QString)));
}

void Network::EventSourceTest::runEventSource()
{
    qDebug() << "Waiting for SSE response...";
    QEventLoop loop1;
    connect(m_sse, SIGNAL(messageReceived(QString)), &loop1, SLOT(quit()));
    connect(m_sse, SIGNAL(errorReceived(QString)), &loop1, SLOT(quit()));
    loop1.exec(); // Initial update
    loop1.exec(); // Delta update 1
    loop1.exec(); // Delta update 2
    m_sse->close();
    qDebug() << "SSE response OK";

    qDebug() << "Waiting for polling response...";
    QEventLoop loop2;
    connect(m_polling, SIGNAL(messageReceived(QString)), &loop2, SLOT(quit()));
    connect(m_polling, SIGNAL(errorReceived(QString)), &loop2, SLOT(quit()));
    loop2.exec(); // Initial update
    loop2.exec(); // Delta update 1
    loop2.exec(); // Delta update 2
    m_polling->close();
    qDebug() << "Polling response OK";
}

void Network::EventSourceTest::cleanEventSource()
{
    delete m_sse;
    delete m_polling;
}

void Network::EventSourceTest::processMessage(const QString msg)
{
    //qInfo() << "Message:" << msg;
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


