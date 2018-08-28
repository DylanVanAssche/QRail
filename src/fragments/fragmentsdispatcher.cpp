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

Fragments::Dispatcher::Dispatcher(QObject *parent) : QObject(parent)
{
    this->setEventType(static_cast<QEvent::Type>(QEvent::registerEventType()));
}

void Fragments::Dispatcher::dispatchPage(Fragments::Page *page)
{
    /*
     * WARNING:
     *  QEvent must be allocated on the heap since the event queue will
     *  take ownership of the QEvent object.
     *  Accessing it after calling 'postEvents()' isn't safe!
     * INFO: https://doc.qt.io/qt-5/qcoreapplication.html#postEvent
     */

    // Create custom event type
    Fragments::DispatcherEvent *event = new Fragments::DispatcherEvent(this->eventType());
    event->setPage(page);

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
    QList<QObject *> callerList = this->findAndRemoveTargets(from, until);

    // Post the event to the event queue
    foreach(QObject *caller, callerList) {
        QCoreApplication::postEvent(caller, event);
    }
}

Fragments::Page *Fragments::DispatcherEvent::page() const
{
    return m_page;
}

void Fragments::DispatcherEvent::setPage(Fragments::Page *page)
{
    m_page = page;
}

void Fragments::Dispatcher::addTarget(const QDateTime &departureTime, QObject *caller)
{
    m_targets.insert(departureTime, caller);
}

QList<QObject *> Fragments::Dispatcher::findAndRemoveTargets(const QDateTime &from, const QDateTime &until)
{
    QList<QObject *> callers = QList<QObject *>();
    foreach(QDateTime timestamp, m_targets.keys())
    {
        if((timestamp.toMSecsSinceEpoch() >= from.toMSecsSinceEpoch()) && (timestamp.toMSecsSinceEpoch() <= until.toMSecsSinceEpoch()))
        {
            callers.append(m_targets.value(timestamp));
            m_targets.remove(timestamp);
        }
    }
    return callers;
}

QEvent::Type Fragments::Dispatcher::eventType() const
{
    return m_eventType;
}

void Fragments::Dispatcher::setEventType(const QEvent::Type &eventType)
{
    m_eventType = eventType;
}
