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

    // Init the dispatcher
    this->setDispatcher(new QRail::Network::Dispatcher(this));

    // Init cache
    QNetworkConfigurationManager QNAMConfig;
    this->QNAM()->setConfiguration(QNAMConfig.defaultConfiguration());
    this->setCache(new QNetworkDiskCache(this));

    // Get the caching directory of the application
    QString path = QStandardPaths::writableLocation(QStandardPaths::CacheLocation) + "/network";

    // Create the 'network' folder to save our caching data
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
    connect(this->QNAM(), SIGNAL(finished(QNetworkReply *)),
            this, SLOT(requestCompleted(QNetworkReply *)));

    // Create HTTP client information
    this->setUserAgent(QString("%1/%2 (%3/%4)").arg("QRail", "0.1.0", "Sailfish OS", "3.0.1.11"));
    this->setAcceptHeader(QString("application/ld+json"));
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 9 Aug 2018
 * @brief Get a QRail::Network::Manager instance
 * @param QObject *parent = nullptr
 * @return QRail::Network::Manager *manager
 * @package Network
 * @public
 * Constructs a QRail::Network::Manager if none exists and returns the instance.
 */
QRail::Network::Manager *QRail::Network::Manager::getInstance()
{
    if (m_instance == nullptr) {
        qDebug() << "Creating new QRail::Network::Manager";
        m_instance = new Manager();
    }
    return m_instance;
}

// Invokers
/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Get a resource
 * @param const QUrl &url
 * @param QObject *caller
 * @package Network
 * @public
 * Retrieves a certain resource from the given QUrl &url using a GET request.
 * The result as a QNetworkReply *reply will be available through the Qt event system.
 * Implement the customEvent() method in your QObject to receive the data.
 */
void QRail::Network::Manager::getResource(const QUrl &url, QObject *caller)
{
    qDebug() << "GET resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    QNetworkReply *reply = this->QNAM()->get(request);
    this->dispatcher()->addTarget(reply, caller);
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Post to a resource
 * @param const QUrl &url
 * @param QObject *caller
 * @package Network
 * @public
 * Posts data to a certain resource from the given QUrl url using a POST request.
 * The result as a QNetworkReply *reply will be available through the Qt event system.
 * Implement the customEvent() method in your QObject to receive the data.
 */
void QRail::Network::Manager::postResource(const QUrl &url, const QByteArray &data, QObject *caller)
{
    qDebug() << "POST resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    QNetworkReply *reply = this->QNAM()->post(request, data);
    this->dispatcher()->addTarget(reply, caller);
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Delete a resource
 * @param const QUrl &url
 * @param QObject *caller
 * @package Network
 * @public
 * Deletes a certain resource from the given QUrl url using a DELETE request.
 * The result as a QNetworkReply *reply will be available through the Qt event system.
 * Implement the customEvent() method in your QObject to receive the data.
 */
void QRail::Network::Manager::deleteResource(const QUrl &url, QObject *caller)
{
    qDebug() << "DELETE resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    QNetworkReply *reply = this->QNAM()->deleteResource(request);
    this->dispatcher()->addTarget(reply, caller);
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Head a resource
 * @param const QUrl &url
 * @param QObject *caller
 * @package Network
 * @public
 * Retrieves the headers of the resource from the given QUrl url using a HEAD request.
 * The result as a QNetworkReply *reply will be available through the Qt event system.
 * Implement the customEvent() method in your QObject to receive the data.
 */
void QRail::Network::Manager::headResource(const QUrl &url, QObject *caller)
{
    qDebug() << "HEAD resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    QNetworkReply *reply = this->QNAM()->head(request);
    this->dispatcher()->addTarget(reply, caller);
}

void QRail::Network::Manager::requestCompleted(QNetworkReply *reply)
{
    this->dispatcher()->dispatchReply(reply);
}

// Helpers
/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Prepare the HTTP request
 * @param const QUrl &url
 * @return QNetworkRequest request
 * @package Network
 * @public
 * Everytime a HTTP request has been made by the user it needs several default headers to complete it's mission.
 * The prepareRequest method just does that, it adds the Accept, User-Agent header to the request and allows redirects.
 */
QNetworkRequest QRail::Network::Manager::prepareRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setRawHeader(QByteArray("Accept"), this->acceptHeader().toUtf8());
    request.setHeader(QNetworkRequest::UserAgentHeader, this->userAgent());
    request.setHeader(QNetworkRequest::ContentTypeHeader, CONTENT_TYPE);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    request.setAttribute(QNetworkRequest::CacheLoadControlAttribute, QNetworkRequest::PreferNetwork); // Load from network if cache has expired
    return request;
}

// Getter & Setters
/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Gets the current user agent
 * @return const QString userAgent
 * @package Network
 * @public
 * Retrieves the current user agent used to make requests in this HTTP instance.
 */
QString QRail::Network::Manager::userAgent() const
{
    return m_userAgent;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Sets the current user agent
 * @param const QString &userAgent
 * @package Network
 * @public
 * Changes the current user agent to the given QString.
 */
void QRail::Network::Manager::setUserAgent(const QString &userAgent)
{
    m_userAgent = userAgent;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Get the current accept header
 * @return const QString acceptHeader
 * @package Network
 * @public
 * Retrieves the current accept header used to make requests in this HTTP instance.
 */
QString QRail::Network::Manager::acceptHeader() const
{
    return m_acceptHeader;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Sets the current accept header
 * @param const QString &acceptHeader
 * @package Network
 * @public
 * Changes the current accept header to the given QString.
 */
void QRail::Network::Manager::setAcceptHeader(const QString &acceptHeader)
{
    m_acceptHeader = acceptHeader;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the QNAM instance
 * @return QNetworkAccessManager *QNAM
 * @package Network
 * @public
 * Gets the QNetworkAccessManager instance.
 */
QNetworkAccessManager *QRail::Network::Manager::QNAM() const
{
    return m_QNAM;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the QNAM instance
 * @param QNetworkAccessManager *value
 * @package Network
 * @public
 * Sets the QNetworkAccessManager instance.
 */
void QRail::Network::Manager::setQNAM(QNetworkAccessManager *value)
{
    m_QNAM = value;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets the QNAM cache instance
 * @return QAbstractNetworkCache *cache
 * @package Network
 * @public
 * Gets the QAbstractNetworkCache instance.
 */
QAbstractNetworkCache *QRail::Network::Manager::cache() const
{
    return m_cache;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Sets the QNAM cache instance
 * @param QAbstractNetworkCache *cache
 * @package Network
 * @public
 * Sets the QAbstractNetworkCache instance.
 */
void QRail::Network::Manager::setCache(QAbstractNetworkCache *cache)
{
    m_cache = cache;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 28 Aug 2018
 * @brief Gets the QRail::Network::Dispatcher instance
 * @return QRail::Network::Dispatcher *
 * @package Network
 * @public
 * Gets the QRail::Network::Dispatcher instance.
 */
QRail::Network::Dispatcher *QRail::Network::Manager::dispatcher() const
{
    return m_dispatcher;
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 28 Aug 2018
 * @brief Sets the QRail::Network::Dispatcher instance
 * @param QRail::Network::Dispatcher *dispatcher
 * @package Network
 * @public
 * Sets the QRail::Network::Dispatcher instance.
 */
void QRail::Network::Manager::setDispatcher(QRail::Network::Dispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
}
