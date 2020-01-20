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

#ifndef EVENTSOURCE_H
#define EVENTSOURCE_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QTimer>
#include <QtCore/QSharedPointer>
#include "network/networkmanager.h"

#define MAX_RETRIES 3
#define POLL_INTERVAL 30 * 1000 // 30 000 ms = 30 s

namespace QRail {
namespace Network {
class EventSource : public QObject
{
    Q_OBJECT
public:
    //! The different states the EventSource can have
    enum class ReadyState {
        CONNECTING,
        OPEN,
        CLOSED
    };
    enum Subscription {
        SSE,
        POLLING
    };
    explicit EventSource(QUrl url, Subscription subcriptionType, QObject *parent = nullptr);
    QUrl url();
    void close();
    void open();

signals:
    void errorReceived(QString error);
    void messageReceived(QString message);
    void readyStateChanged(QRail::Network::EventSource::ReadyState state);

/*protected:
    //! Dispatcher protected method, only here as a reference.
    virtual void customEvent(QEvent *event);*/

private slots:
    void handleSSEStream();
    void handleSSEFinished();
    void handlePollingFinished();
    void pollPollingStream();

private:
    QTimer *m_timer;
    QUrl m_url;
    QString m_lastEventId;
    qint16 m_retries;
    qint64 m_retryTime;
    QRail::Network::Manager *m_manager;
    QSharedPointer<QNetworkReply> m_reply;
    ReadyState m_readyState;
    QString m_chunks;
    Subscription m_subscriptionType;
    void setReadyState(ReadyState state);
    void parseEvents(QStringList events);
};
}
}

#endif // EVENTSOURCE_H
