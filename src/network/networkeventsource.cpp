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

EventSource::EventSource(QUrl url, QObject *parent) : QObject(parent)
{
    m_url = url;
    m_manager = QRail::Network::Manager::getInstance();
    this->open();
}

QUrl EventSource::url()
{
    return m_url;
}

void EventSource::close()
{
    m_manager->unsubscribe(m_reply, this);
    this->setReadyState(EventSource::ReadyState::CLOSED);
}

void EventSource::open()
{
    m_reply = m_manager->subscribe(m_url, this);
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(handleStream()));
    connect(m_reply, SIGNAL(finished()), this, SLOT(handleStream()));
    this->setReadyState(EventSource::ReadyState::CONNECTING);
}

void EventSource::handleStream()
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

void EventSource::handleFinished()
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

void EventSource::parseEvents(QStringList events)
{
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
