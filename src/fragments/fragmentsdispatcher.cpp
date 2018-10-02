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
#include "fragments/fragmentsdispatcher.h"
using namespace QRail;

QRail::Fragments::Dispatcher::Dispatcher(QObject *parent) : QObject(parent)
{
    // Register a custom event type to the Qt event system
    this->setEventType(static_cast<QEvent::Type>(QEvent::registerEventType()));
}

void QRail::Fragments::Dispatcher::dispatchPage(QRail::Fragments::Page *page)
{
    /*
     * WARNING:
     *  QEvent must be allocated on the heap since the event queue will
     *  take ownership of the QEvent object.
     *  Accessing it after calling 'postEvents()' isn't safe!
     * INFO: https://doc.qt.io/qt-5/qcoreapplication.html#postEvent
     */

    // Create custom event type
    QRail::Fragments::DispatcherEvent event = QRail::Fragments::DispatcherEvent(this->eventType());
    event.setPage(page);

    /*
     * Retrieve the callers of the page.
     *
     * Multiple callers can call the same
     * page since the timestamp can
     * be different for the same page.
     * Each page covers a range of time,
     * all timestamps in this range are
     * valid.
     */
    QDateTime from = page->fragments().first()->departureTime();
    QDateTime until = page->fragments().last()->departureTime();
    QList<QObject *> callerList = this->findTargets(from, until);

    // We should have retrieved some callers to dispatch the page to
    if (callerList.isEmpty()) {
        qCritical() << "No callers found for dispatching page:" << page->uri();
    }

    // Post the event to the event queue
    foreach (QObject *caller, callerList) {
        QCoreApplication::sendEvent(caller, &event);
    }
    this->removeTargets(from, until);
}

QRail::Fragments::Page *QRail::Fragments::DispatcherEvent::page() const
{
    return m_page;
}

void QRail::Fragments::DispatcherEvent::setPage(QRail::Fragments::Page *page)
{
    m_page = page;
}

void QRail::Fragments::Dispatcher::addTarget(const QDateTime &departureTime, QObject *caller)
{
    QMutexLocker locker(&targetListLocker);
    m_targets.insert(departureTime, caller);
}

QList<QObject *> QRail::Fragments::Dispatcher::findTargets(const QDateTime &from,
                                                           const QDateTime &until)
{
    QMutexLocker locker(&targetListLocker);
    QList<QObject *> callers = QList<QObject *>();
    foreach (QDateTime timestamp, m_targets.keys()) {
        /*
         * If the timestamp is the same or higher or equal than the first fragment departure time
         * and it's lower than the last fragment departure time + 1 minute.
         *
         * WARNING: We need to add 1 minute to this check to avoid a reace condition:
         *          If the page ends at 19:28:00.000 (departure time of the last fragment)
         *          and we request the page at 19:28:35.841 then our check needs to valid.
         *          The reason for this lies in the way the Linked Connection server fragments
         *          the data and how it's redirecting clients.
         */
        if ((timestamp.toMSecsSinceEpoch() >= from.toMSecsSinceEpoch())
                && (timestamp.toMSecsSinceEpoch() < until.toMSecsSinceEpoch() + MINUTES_TO_MSECONDS_MULTIPLIER)) {
            callers.append(m_targets.value(timestamp));
        }
    }
    return callers;
}

void QRail::Fragments::Dispatcher::removeTargets(const QDateTime &from,
                                                 const QDateTime &until)
{
    QMutexLocker locker(&targetListLocker);
    foreach (QDateTime timestamp, m_targets.keys()) {
        if ((timestamp.toMSecsSinceEpoch() >= from.toMSecsSinceEpoch())
                && (timestamp.toMSecsSinceEpoch() <= until.toMSecsSinceEpoch())) {
            m_targets.remove(timestamp);
        }
    }
}

QEvent::Type QRail::Fragments::Dispatcher::eventType() const
{
    return m_eventType;
}

void QRail::Fragments::Dispatcher::setEventType(const QEvent::Type &eventType)
{
    m_eventType = eventType;
}
