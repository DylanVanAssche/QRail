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
#include "network/networkdispatcher.h"
using namespace QRail;

QRail::Network::Dispatcher::Dispatcher(QObject *parent) : QObject(parent)
{
    // Register a custom event type to the Qt event system
    this->setEventType(static_cast<QEvent::Type>(QEvent::registerEventType()));
}

void QRail::Network::Dispatcher::dispatchReply(QNetworkReply *reply)
{
    /*
     * WARNING:
     *      QEvent must be allocated on the heap since the event queue will
     *      take ownership of the QEvent object.
     *      Accessing it after calling 'postEvents()' isn't safe!
     * INFO: https://doc.qt.io/qt-5/qcoreapplication.html#postEvent
     */

    // Create custom event type
    QRail::Network::DispatcherEvent event = QRail::Network::DispatcherEvent(this->eventType());
    event.setReply(reply);

    // Retrieve the caller of the reply
    QObject *caller = this->findTarget(reply);

    // Post the event to the event queue and remove the reply from the targets list
    QCoreApplication::sendEvent(caller, &event);
    this->removeTarget(reply, caller);
}

void QRail::Network::Dispatcher::addTarget(QNetworkReply *reply, QObject *caller, bool isSubscription)
{
    QMutexLocker locker(&targetListLocker);
    m_targets.insert(reply, caller);
    if(isSubscription) {
        m_subscriptions.append(caller);
    }
}

void QRail::Network::Dispatcher::removeTarget(QNetworkReply *reply, QObject *caller)
{
    QMutexLocker locker(&targetListLocker);
    if(m_subscriptions.contains(caller)) {
        qDebug() << "Subscription:" << caller << "skip removal";
    }
    else {
        m_targets.remove(reply);
    }
}

void Network::Dispatcher::removeSubscriber(QObject *caller)
{
    m_subscriptions.removeAll(caller);
}

QObject *QRail::Network::Dispatcher::findTarget(QNetworkReply *reply)
{
    QMutexLocker locker(&targetListLocker);
    return m_targets.value(reply);
}

QEvent::Type QRail::Network::Dispatcher::eventType() const
{
    return m_eventType;
}

void QRail::Network::Dispatcher::setEventType(const QEvent::Type &eventType)
{
    m_eventType = eventType;
}

QNetworkReply *QRail::Network::DispatcherEvent::reply() const
{
    return m_reply;
}

void QRail::Network::DispatcherEvent::setReply(QNetworkReply *reply)
{
    m_reply = reply;
}
