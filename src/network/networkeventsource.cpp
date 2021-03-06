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
#include "network/networkeventsource.h"
using namespace QRail;
using namespace Network;

EventSource::EventSource(QUrl url, Subscription subscriptionType, QObject *parent) : QObject(parent)
{
    m_url = url;
    m_subscriptionType = subscriptionType;

    // Create a QRail::Network::Manager and open the event source
    m_manager = QRail::Network::Manager::getInstance();
    this->open();
}

QUrl EventSource::url()
{
    return m_url;
}

void EventSource::close()
{
    m_reply->abort();
    this->setReadyState(EventSource::ReadyState::CLOSED);
}

void EventSource::open()
{
    qDebug() << "Opening EventSource";
    this->setReadyState(EventSource::ReadyState::CONNECTING);
    if(m_subscriptionType == Subscription::SSE) {
        qDebug() << "Opening SSE stream...";
        m_reply = QSharedPointer<QNetworkReply>(m_manager->subscribe(m_url));
        connect(m_reply.data(), SIGNAL(readyRead()), this, SLOT(handleSSEStream()));
        connect(m_reply.data(), SIGNAL(finished()), this, SLOT(handleSSEFinished()));
    }
    else if(m_subscriptionType == Subscription::POLLING) {
        qDebug() << "Opening HTTP polling stream...";
        m_timer = new QTimer(this);
        connect(m_timer, SIGNAL(timeout()), this, SLOT(pollPollingStream()));
        m_timer->start(POLL_INTERVAL);
    }
    else if(m_subscriptionType == Subscription::NONE) {
        qDebug() << "None subscription, doing nothing...";
    }
    else {
        qCritical() << "Unknown subscription type!";
    }
}

void EventSource::handleSSEStream()
{
    // Read reply, reset retries counter and update the ready state
    QString payload = QString(m_reply->readAll());
    m_retries = 0;
    this->setReadyState(EventSource::ReadyState::OPEN);

    // Add chunk to data stream
    m_chunks.append(payload);
    if(m_chunks.contains("\n\n")) {
        qDebug() << "Received complete event!";
        QStringList events = m_chunks.split("\n\n", QString::SkipEmptyParts);
        this->parseEvents(events);
    }
}

void EventSource::handleSSEFinished()
{
    qDebug() << "Stream finished:" << m_reply->url();
    this->setReadyState(EventSource::ReadyState::CLOSED);
    qDebug() << "Reconnecting...";
    if(m_retries < MAX_RETRIES) {
        m_retries++;
        this->close();
        this->open();
    }
    else {
        qCritical() << "Unable to reconnect, max retries reached";
        emit this->errorReceived(QString("Unable to reconnect, max retries reached"));
    }
}

void EventSource::handlePollingFinished()
{
    qDebug() << "Received poll reply";
    QString payload = QString(m_reply->readAll());
    emit this->messageReceived(payload);
}

void EventSource::pollPollingStream()
{
    // Only execute polling when the connection is open.
    if(m_readyState != EventSource::ReadyState::CLOSED) {
        qDebug() << "Polling resource...";
        m_reply = QSharedPointer<QNetworkReply>(m_manager->getResource(m_url));
        connect(m_reply.data(), SIGNAL(finished()), this, SLOT(handlePollingFinished()));
    }
    else {
        qDebug() << "EventSource is closed, unable to poll";
    }
}

void EventSource::parseEvents(QStringList events)
{
    qDebug() << "Parsing events from stream...";
    // Split each event entry
    foreach(QString event, events) {
        QStringList eventParts = event.split("\n");

        // Parse each entry
        foreach(QString part, eventParts) {
            // Split entry by TYPE:PAYLOAD
            QStringList payload = part.split(":");

            // Handle each entry type
            if(payload[0] == "id") {
                qDebug() << "SSE ID received" << part.replace("id: ", "").toInt();
                m_lastEventId = part.replace("id: ", "").toInt();
            }
            else if(payload[0] == "data") {
                qDebug() << "SSE DATA received:" << part.replace("data: ", "").length() << "chars";
                emit this->messageReceived(part.replace("data: ", ""));
            }
            else if(payload[0] == "retry") {
                qDebug() << "SSE RETRY received:" << part.replace("retry: ", "").toInt();
                m_retryTime = part.replace("retry: ", "").toInt();
            }
        }
    }

    // Clear chunks and update the parsing state
    m_chunks = "";
}

void EventSource::setReadyState(EventSource::ReadyState state)
{
    m_readyState = state;
    if(m_readyState == EventSource::ReadyState::CLOSED) {
        qDebug() << "EventSource CLOSED";
    }
    else if(m_readyState == EventSource::ReadyState::OPEN) {
        qDebug() << "EventSource OPEN";
    }
    else if(m_readyState == EventSource::ReadyState::CONNECTING) {
        qDebug() << "EventSource CONNECTING";
    }
    emit this->readyStateChanged(m_readyState);
}
