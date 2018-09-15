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
#ifndef NETWORKDISPATCHER_H
#define NETWORKDISPATCHER_H

#include <QtCore/QObject>
#include <QtCore/QEvent>
#include <QtCore/QCoreApplication>
#include <QtCore/QMap>
#include <QtNetwork/QNetworkReply>

namespace QRail {
    namespace Network {
        /*
         * Using Qt events, we can easily dispatch events to a certain QObject.
         * DOCS:
         *      - https://doc.qt.io/qt-5/qevent.html
         *      - https://doc.qt.io/qt-5/qobject.html#customEvent
         *      - https://doc.qt.io/qt-5/qcoreapplication.html#postEvent
         * INFO:
         *      - http://programmingexamples.wikidot.com/qt-events
         *      - https://ryanclouser.com/2015/07/16/Qt-Passing-Custom-QEvent-Data/
         */
        class DispatcherEvent : public QEvent
        {
        public:
            DispatcherEvent(const QEvent::Type &type) : QEvent(type)
            {
            }
            QNetworkReply *reply() const;
            void setReply(QNetworkReply *reply);

        private:
            QNetworkReply *m_reply;
        };

        class Dispatcher : public QObject
        {
            Q_OBJECT
        public:
            explicit Dispatcher(QObject *parent = nullptr);
            void dispatchReply(QNetworkReply *reply);
            void addTarget(QNetworkReply *reply, QObject *caller);
            void removeTarget(QNetworkReply *reply);
            QObject *findTarget(QNetworkReply *reply);
            QEvent::Type eventType() const;

        private:
            QMap<QNetworkReply *, QObject *> m_targets;
            QEvent::Type m_eventType;
            void setEventType(const QEvent::Type &eventType);
        };
    }
}

#endif // NETWORKDISPATCHER_H