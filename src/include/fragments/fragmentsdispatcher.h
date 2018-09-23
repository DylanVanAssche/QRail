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
#ifndef FRAGMENTSDISPATCHER_H
#define FRAGMENTSDISPATCHER_H

#include <QtCore/QDebug>
#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QEvent>
#include <QtCore/QList>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QCoreApplication>

#include "fragments/fragmentspage.h"

namespace QRail {
namespace Fragments {
class DispatcherEvent : public QEvent
{
public:
    DispatcherEvent(const QEvent::Type &type) : QEvent(type)
    {
    }
    QRail::Fragments::Page *page() const;
    void setPage(QRail::Fragments::Page *value);

private:
    QRail::Fragments::Page *m_page;
};

class Dispatcher : public QObject
{
    Q_OBJECT
public:
    explicit Dispatcher(QObject *parent = nullptr);
    void dispatchPage(QRail::Fragments::Page *page);
    void addTarget(const QDateTime &departureTime, QObject *caller);
    void removeTargets(const QDateTime &from, const QDateTime &until);
    QList<QObject *> findTargets(const QDateTime &from, const QDateTime &until);
    QEvent::Type eventType() const;

private:
    mutable QMutex targetListLocker;
    QMap<QDateTime, QObject *> m_targets;
    QEvent::Type m_eventType;
    void setEventType(const QEvent::Type &eventType);
};
}
}

#endif // FRAGMENTSDISPATCHER_H
