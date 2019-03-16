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

#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QStandardPaths>
#include <QtCore/QDir>
#include <QtNetwork/QAbstractNetworkCache>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>

#include "network/networkdispatcher.h"

#define CONTENT_TYPE "application/ld+json"
#define ACCEPT_HEADER_SSE "text/event-stream"
#define ACCEPT_HEADER_HTTP "application/ld+json"

// Singleton pattern
namespace QRail {
namespace Network {
//! A Network::Manager provides you easily access to the WWW.
/*!
    \class Manager
    The singleton design pattern allows you to create Network::Manager objects in an easy way. Several modes are available to perform your WWW operations.<br>
    Thanks to the facade design pattern, the internals of the network library behind this facade are hidden for the user.
 */
class Manager : public QObject
{
    Q_OBJECT
public:
    //! Gets a Network::Manager instance.
    /*!
        \return An instance of Network::Manager.
        \public
        Constructs a Network::Manager if none exists and returns the instance.
     */
    static Manager *getInstance();
    QString userAgent() const;
    void setUserAgent(const QString &userAgent);
    QRail::Network::Dispatcher *dispatcher() const;

signals:
    //! SSL errors are emitted through this signal.
    QList<QSslError> sslErrorsReceived(QNetworkReply *reply, QList<QSslError> sslError);
    //! Emitted when the network accessible state has been changed. For example: WiFi is out of range.
    QNetworkAccessManager::NetworkAccessibility networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility state);
    //! Emitted when the user agent has been changed.
    void userAgentChanged();

public slots:
    //! HTTP GET request.
    /*!
        \param url The URL you want to access.
        \param caller The caller of this method.
        \note The caller is needed since the dispatcher will send you a special event using the Qt event system.
     */
    void getResource(const QUrl &url, QObject *caller);
    //! HTTP POST request.
    /*!
        \param url The URL you want to access.
        \param caller The caller of this method.
        \param data The data that you want to send to the server.
        \note The caller is needed since the dispatcher will send you a special event using the Qt event system.
     */
    void postResource(const QUrl &url, const QByteArray &data, QObject *caller);
    //! HTTP DELETE request.
    /*!
        \param url The URL you want to access.
        \param caller The caller of this method.
        \note The caller is needed since the dispatcher will send you a special event using the Qt event system.
     */
    void deleteResource(const QUrl &url, QObject *caller);
    //! HTTP HEAD request.
    /*!
        \param url The URL you want to access.
        \param caller The caller of this method.
        \note The caller is needed since the dispatcher will send you a special event using the Qt event system.
     */
    void headResource(const QUrl &url, QObject *caller);
    //! Subscribe to a HTTP SSE resource.
    /*!
        \param url The URL of the SSE resource.
        \param caller The caller of this method.
     */
    QNetworkReply *subscribe(const QUrl &url, QObject *caller);
    //! Unsubscribe to a HTTP SSE resource.
    /*!
        \param caller The caller of this method.
    */
    void unsubscribe(QObject *caller);

private slots:
    void requestCompleted(QNetworkReply *reply);
    QNetworkReply *poll(const QUrl &url);

private:
    QNetworkAccessManager *m_QNAM;
    QAbstractNetworkCache *m_cache;
    QRail::Network::Dispatcher *m_dispatcher;
    QString m_userAgent;
    static Manager *m_instance;
    explicit Manager(QObject *parent = nullptr);
    QNetworkRequest prepareHTTPRequest(const QUrl &url);
    QNetworkAccessManager *QNAM() const;
    void setQNAM(QNetworkAccessManager *value);
    QAbstractNetworkCache *cache() const;
    void setCache(QAbstractNetworkCache *cache);
    void setDispatcher(QRail::Network::Dispatcher *dispatcher);
    static Manager *manager();
    static void setManager(const Manager *manager);
};
} // namespace Network
} // namespace QRail

#endif // NETWORKMANAGER_H
