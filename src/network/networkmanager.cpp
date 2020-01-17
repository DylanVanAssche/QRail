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
#include "network/networkmanager.h"
using namespace QRail;
QRail::Network::Manager *QRail::Network::Manager::m_instance = nullptr;

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief QRail::Network::Manager facade constructor
 * @package Network
 * @private
 * Constructs a QRail::Network::Manager facade to access the network using the HTTP protocol.
 * The QRail::Network::Manager facade makes network access in Qt abstract from the underlying library (QNetworkAccessManager, libCurl, ...).
 */
QRail::Network::Manager::Manager(QObject *parent): QObject(parent)
{
    // Initiate a new QNetworkAccessManager with cache
    this->setQNAM(new QNetworkAccessManager(this));

    // Init cache
    QNetworkConfigurationManager QNAMConfig;
    this->QNAM()->setConfiguration(QNAMConfig.defaultConfiguration());
    this->setCache(new QNetworkDiskCache(this));

    // Get the caching directory of the application
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/network";

    // Create the 'network' folder to save our caching network data
    QDir cacheDirectory;
    cacheDirectory.mkpath(path);

    // Setup the QNetworkDiskCache
    ((QNetworkDiskCache *)this->cache())->setCacheDirectory(path);
    this->QNAM()->setCache(this->cache());

    // Connect QNetworkAccessManager signals
    connect(this->QNAM(), SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
            this, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    connect(this->QNAM(), SIGNAL(sslErrors(QNetworkReply *, QList<QSslError>)),
            this, SIGNAL(sslErrorsReceived(QNetworkReply *, QList<QSslError>)));
    connect(this->QNAM(), SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply *)));

    // Create HTTP client information
    this->setUserAgent(QString("%1/%2 (%3/%4)").arg("QRail", "0.2.0", "Linux", "cli"));
}

QRail::Network::Manager *QRail::Network::Manager::getInstance()
{
    if (m_instance == nullptr) {
        qDebug() << "Creating new QRail::Network::Manager";
        m_instance = new Manager();
    }
    return m_instance;
}

// Invokers
QNetworkReply *QRail::Network::Manager::getResource(const QUrl &url)
{
    qDebug() << "GET resource:" << url;
    QNetworkRequest request = this->prepareHTTPRequest(url);
    QNetworkReply *reply = this->QNAM()->get(request);
    return reply;
}

QNetworkReply *QRail::Network::Manager::postResource(const QUrl &url, const QByteArray &data)
{
    qDebug() << "POST resource:" << url;
    QNetworkRequest request = this->prepareHTTPRequest(url);
    QNetworkReply *reply = this->QNAM()->post(request, data);
    return reply;
}

QNetworkReply *QRail::Network::Manager::deleteResource(const QUrl &url)
{
    qDebug() << "DELETE resource:" << url;
    QNetworkRequest request = this->prepareHTTPRequest(url);
    QNetworkReply *reply = this->QNAM()->deleteResource(request);
    return reply;
}

QNetworkReply *QRail::Network::Manager::headResource(const QUrl &url)
{
    qDebug() << "HEAD resource:" << url;
    QNetworkRequest request = this->prepareHTTPRequest(url);
    QNetworkReply *reply = this->QNAM()->head(request);
    return reply;
}

QNetworkReply *QRail::Network::Manager::subscribe(const QUrl &url)
{
    // SSE has special request headers and attributes
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Accept"), QByteArray(ACCEPT_HEADER_SSE));
    request.setHeader(QNetworkRequest::UserAgentHeader, this->userAgent());
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::AlwaysNetwork); // SSE events may not be cached
    QNetworkReply *reply = this->QNAM()->get(request);
    return reply;
}

void QRail::Network::Manager::unsubscribe()
{
    qDebug() << "Closed stream";
}

void Network::Manager::finished(QNetworkReply *reply)
{
    qDebug() << "QNAM REPLY:" << reply->url();
}

// Helpers
QNetworkRequest QRail::Network::Manager::prepareHTTPRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Accept"), ACCEPT_HEADER_HTTP);
    request.setHeader(QNetworkRequest::UserAgentHeader, this->userAgent());
    request.setHeader(QNetworkRequest::ContentTypeHeader, CONTENT_TYPE);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork); // Load from network if cache has expired
    return request;
}

// Getter & Setters
QString QRail::Network::Manager::userAgent() const
{
    return m_userAgent;
}

void QRail::Network::Manager::setUserAgent(const QString &userAgent)
{
    m_userAgent = userAgent;
}

QNetworkAccessManager *QRail::Network::Manager::QNAM() const
{
    return m_QNAM;
}

void QRail::Network::Manager::setQNAM(QNetworkAccessManager *value)
{
    m_QNAM = value;
}

QAbstractNetworkCache *QRail::Network::Manager::cache() const
{
    return m_cache;
}

void QRail::Network::Manager::setCache(QAbstractNetworkCache *cache)
{
    m_cache = cache;
}
