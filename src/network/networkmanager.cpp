/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of BeRail.                                               *
 *                                                                            *
 *   BeRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   BeRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with BeRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#include "networkmanager.h"
Network::Manager* Network::Manager::m_instance = nullptr;

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Network::Manager facade constructor
 * @package Network
 * @private
 * Constructs a Network::Manager facade to access the network using the HTTP protocol.
 * The Network::Manager facade makes network access in Qt abstract from the underlying library (QNetworkAccessManager, libCurl, ...).
 */
Network::Manager::Manager(QObject* parent): QObject(parent)
{
    // Set parent of this QObject. When parent is destroyed, this one is automatically cleaned up too.
    this->setParent(parent);

    // Initiate a new QNetworkAccessManager with cache
    this->setQNAM(new QNetworkAccessManager(this));
    QNetworkConfigurationManager QNAMConfig;
    this->QNAM()->setConfiguration(QNAMConfig.defaultConfiguration());
    this->setCache(new QNetworkDiskCache(this));
    ((QNetworkDiskCache *)this->cache())->setCacheDirectory("~/.local/share/harbour-berail/network");
    this->QNAM()->setCache(this->cache());

    // Connect QNetworkAccessManager signals
    connect(this->QNAM(), SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)), this, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    connect(this->QNAM(), SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SIGNAL(sslErrorsReceived(QNetworkReply*,QList<QSslError>)));
    connect(this->QNAM(), SIGNAL(finished(QNetworkReply*)), this, SIGNAL(requestCompleted(QNetworkReply*)));

    // Create HTTP client information
    this->setUserAgent(QString("%1/%2 (%3)").arg("BeRail-LC", "V0.0.1", "2.2.0.29"));
    this->setAcceptHeader(QString("application/ld+json"));
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 9 Aug 2018
 * @brief Get a Network::Manager instance
 * @param QObject *parent = nullptr
 * @return Network::Manager *manager
 * @package Network
 * @public
 * Constructs a Network::Manager if none exists and returns the instance.
 */
Network::Manager *Network::Manager::getInstance(QObject *parent)
{
    if(m_instance == nullptr) {
        qDebug() << "Creating new Network::Manager";
        m_instance = new Manager(parent);
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
 * @package Network
 * @public
 * Retrieves a certain resource from the given QUrl &url using a GET request.
 * The result as a QNetworkReply *reply will be available as soon as the requestCompleted signal is fired.
 */
void Network::Manager::getResource(const QUrl &url)
{
    qDebug() << "GET resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    this->QNAM()->get(request);
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Post to a resource
 * @param const QUrl &url
 * @package Network
 * @public
 * Posts data to a certain resource from the given QUrl url using a POST request.
 * The result as a QNetworkReply *reply will be available as soon as the requestCompleted signal is fired.
 */
void Network::Manager::postResource(const QUrl &url, const QByteArray &data)
{
    qDebug() << "POST resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    this->QNAM()->post(request, data);
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 17 Jul 2018
 * @brief Delete a resource
 * @param const QUrl &url
 * @package Network
 * @public
 * Deletes a certain resource from the given QUrl url using a DELETE request.
 * The result as a QNetworkReply *reply will be available as soon as the requestCompleted signal is fired.
 */
void Network::Manager::deleteResource(const QUrl &url)
{
    qDebug() << "DELETE resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    this->QNAM()->deleteResource(request);
}

/**
 * @file networkmanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Head a resource
 * @param const QUrl &url
 * @package Network
 * @public
 * Retrieves the headers of the resource from the given QUrl url using a HEAD request.
 * The result as a QNetworkReply *reply will be available as soon as the requestCompleted signal is fired.
 */
void Network::Manager::headResource(const QUrl &url)
{
    qDebug() << "HEAD resource:" << url;
    QNetworkRequest request = this->prepareRequest(url);
    this->QNAM()->head(request);
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
QNetworkRequest Network::Manager::prepareRequest(const QUrl &url)
{
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, this->acceptHeader());
    request.setHeader(QNetworkRequest::UserAgentHeader, this->userAgent());
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
QString Network::Manager::userAgent() const
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
void Network::Manager::setUserAgent(const QString &userAgent)
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
QString Network::Manager::acceptHeader() const
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
void Network::Manager::setAcceptHeader(const QString &acceptHeader)
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
QNetworkAccessManager *Network::Manager::QNAM() const
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
void Network::Manager::setQNAM(QNetworkAccessManager *value)
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
QAbstractNetworkCache *Network::Manager::cache() const
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
void Network::Manager::setCache(QAbstractNetworkCache *cache)
{
    m_cache = cache;
}
