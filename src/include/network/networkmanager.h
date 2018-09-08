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
#include <QtNetwork/QAbstractNetworkCache>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include <QtNetwork/QNetworkDiskCache>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QSslError>

#include "network/networkdispatcher.h"

#define CONTENT_TYPE "application/ld+json"

// Singleton pattern
namespace QRail {
    namespace Network {
        class Manager : public QObject {
          Q_OBJECT
        public:
          static Manager *getInstance();
          QString userAgent() const;
          void setUserAgent(const QString &userAgent);
          QString acceptHeader() const;
          void setAcceptHeader(const QString &acceptHeader);
          QRail::Network::Dispatcher *dispatcher() const;

        signals:
          QList<QSslError> sslErrorsReceived(QNetworkReply *reply,
                                             QList<QSslError> sslError);
          QNetworkAccessManager::NetworkAccessibility
          networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility state);
          void userAgentChanged();
          void acceptHeaderChanged();

        public slots:
          void getResource(const QUrl &url, QObject *caller);
          void postResource(const QUrl &url, const QByteArray &data, QObject *caller);
          void deleteResource(const QUrl &url, QObject *caller);
          void headResource(const QUrl &url, QObject *caller);

        private slots:
          void requestCompleted(QNetworkReply *reply);

        private:
          QNetworkAccessManager *m_QNAM;
          QAbstractNetworkCache *m_cache;
          QRail::Network::Dispatcher *m_dispatcher;
          QString m_userAgent;
          QString m_acceptHeader;
          static Manager *m_instance;
          explicit Manager(QObject *parent = nullptr);
          QNetworkRequest prepareRequest(const QUrl &url);
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
