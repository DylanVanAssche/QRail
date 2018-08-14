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

#include "fragmentsfactory.h"
Fragments::Factory* Fragments::Factory::m_instance = nullptr;

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Fragments::Factory constructor: empty
 * @param QObject *parent = nullptr
 * @package Fragments
 * @private
 * Constructs a Fragments::Factory to generate Linked Connections
 * fragments on the fly.
 */
Fragments::Factory::Factory(QObject *parent): QObject(parent)
{
    // Set parent of this QObject. When parent is destroyed, this one is automatically cleaned up too.
    this->setParent(parent);

    // Setup the Network::Manager
    this->setHttp(Network::Manager::getInstance(parent));
    connect(this->http(), SIGNAL(requestCompleted(QNetworkReply *)), this, SLOT(processHTTPReply(QNetworkReply *)));
    /*
     * QNAM and callers are living in different threads!
     * INFO: https://stackoverflow.com/questions/3268073/qobject-cannot-create-children-for-a-parent-that-is-in-a-different-thread
     */
    connect(this, SIGNAL(getResource(QUrl)), this->http(), SLOT(getResource(QUrl)));
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a Fragments::Factory instance
 * @param QObject *parent = nullptr
 * @return Fragment::Factory *factory
 * @package Fragments
 * @public
 * Constructs a Fragments::Factory if none exists and returns the instance.
 */
Fragments::Factory *Fragments::Factory::getInstance(QObject *parent)
{
    // Singleton pattern
    if(m_instance == nullptr) {
        qDebug() << "Generating new Factory";
        m_instance = new Fragments::Factory(parent);
    }
    return m_instance;
}

// Invokers
/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Requests a page by URI
 * @param const QUrl &uri
 * @package Fragments
 * @public
 * Starts the generation process of a Fragments::Page object by requesting a page by URI.
 * When the page is ready, the pageReady signal will be emitted.
 */
void Fragments::Factory::getPage(const QUrl &uri)
{
    // Use processing methods to allow other extensions in the future if needed
    this->getPageByURIFromNetworkManager(uri);
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Requests a page by departure time
 * @param const QDateTime &departureTime
 * @package Fragments
 * @public
 * Starts the generation process of a Fragments::Page object by requesting a page by departure time.
 * When the page is ready, the pageReady signal will be emitted.
 */
void Fragments::Factory::getPage(const QDateTime &departureTime)
{
    // Construct the URI of the page
    QUrl uri = QUrl(BASE_URL);
    QUrlQuery parameters;
    // Qt:ISODate returns 2018-07-27T14:18:40Z while we need 2018-07-27T14:18:40.000Z
    parameters.addQueryItem("departureTime", departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z"));
    uri.setQuery(parameters);

    // Use processing methods to allow other extensions in the future if needed
    this->getPageByURIFromNetworkManager(uri);
}

// Processors
/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Requests the page by URI from the network
 * @param const QUrl &uri
 * @package Fragments
 * @private
 * Sends a request to the Network::Manager to retrieve a Linked Connections page by URI.
 */
void Fragments::Factory::getPageByURIFromNetworkManager(const QUrl &uri)
{
    // Call the getResource slot due different threads
    emit this->getResource(uri);
}

// Helpers
/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Parses the JSON-LD fragment
 * @param const QJsonObject &connection
 * @return Fragments::Fragment *frag;
 * @package Fragments
 * @private
 * Converts the JSON-LD into a Fragments::Fragments object and returns it.
 */
Fragments::Fragment *Fragments::Factory::generateFragmentFromJSON(const QJsonObject &connection)
{   
    // Check if the data have all the required keys for a fragment
    QStringList fragProps;
    fragProps << "@id" << "departureStop" << "arrivalStop" << "departureTime" << "arrivalTime" << "gtfs:trip" << "gtfs:route";
    this->validateData(connection, fragProps);

    // Parse JSON
    QUrl uri = QUrl(connection["@id"].toString());
    QUrl departureStationURI = QUrl(connection["departureStop"].toString());
    QUrl arrivalStationURI = QUrl(connection["arrivalStop"].toString());
    QDateTime departureTime = QDateTime::fromString(connection["departureTime"].toString(), Qt::ISODate);
    QDateTime arrivalTime = QDateTime::fromString(connection["arrivalTime"].toString(), Qt::ISODate);
    QUrl tripURI = QUrl(connection["gtfs:trip"].toString());
    QUrl routeURI = QUrl(connection["gtfs:route"].toString());
    qint16 departureDelay;
    qint16 arrivalDelay;
    if(connection.contains("departureDelay")) {
        departureDelay = connection["departureDelay"].toInt();
    }
    if(connection.contains("arrivalDelay")) {
        arrivalDelay = connection["arrivalDelay"].toInt();
    }
    QString direction = connection["direction"].toString();

    // Create Linked Connection Fragment and return it
    Fragments::Fragment *frag = new Fragments::Fragment(
                uri,
                departureStationURI,
                arrivalStationURI,
                departureTime,
                arrivalTime,
                departureDelay,
                arrivalDelay,
                tripURI,
                routeURI,
                direction,
                m_instance
                );
    return frag;
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Validates JSON-LD data
 * @param const QJsonObject &data
 * @param const QStringList &properties
 * @return bool &valid
 * @package Fragments
 * @private
 * Checks the JSON-LD data by comparing the expected
 * properties with the received ones.
 */
bool Fragments::Factory::validateData(const QJsonObject &data, const QStringList &properties)
{
    foreach(QString prop, properties) {
        if(data[prop].isUndefined()) {
            qCritical() << "Parsing JSON-LD failed! Missing key:" << prop;
            return false;
        }
    }
    return true;
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Processes the network reply
 * @param QNetworkReply *reply
 * @package Fragments
 * @private
 * Reads the network reply, parses it using the helper methods.
 * When the reply is completed parsed and no errors were encountered,
 * the pageReady signal is emitted.
 * In case we faced an error, the error signal is emitted with an error message.
 */
void Fragments::Factory::processHTTPReply(QNetworkReply *reply)
{
    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
        qDebug() << "Content-Header:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "Content-Length:" << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong() << "bytes";
        qDebug() << "HTTP status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Cache:" << reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool();

        // Read HTTP reply
        QString replyData = (QString)reply->readAll();

        // HTTP 200 OK, create LCPage and LCFragment list
        QList<Fragments::Fragment *> fragments = QList<Fragments::Fragment *>();

        // Try to parse the data as JSON-LD
        QJsonParseError parseError;
        QJsonDocument jsonData = QJsonDocument::fromJson(replyData.toUtf8(), &parseError);
        if(parseError.error == QJsonParseError::NoError) {
            QJsonObject jsonObject = jsonData.object();

            // Check if the data have all the required keys for a page
            QStringList pageProps;
            pageProps << "@id" << "hydra:next" << "hydra:previous" << "@graph";
            this->validateData(jsonObject, pageProps);

            // Linked Connections fragments
            QJsonArray graph = jsonObject["@graph"].toArray();
            foreach(QJsonValue item, graph) {
                QJsonObject connection = item.toObject();
                Fragments::Fragment *frag = this->generateFragmentFromJSON(connection);
                fragments.append(frag);
            }

            // Linked Connections page
            QString pageURI = jsonObject["@id"].toString();
            QDateTime pageTimestamp = QDateTime::fromString(pageURI.right(24), Qt::ISODate); // TO DO REGEX
            QString hydraNext = jsonObject["hydra:next"].toString();
            QString hydraPrevious = jsonObject["hydra:previous"].toString();
            Fragments::Page *page = new Fragments::Page(
                        pageURI,
                        pageTimestamp,
                        hydraNext,
                        hydraPrevious,
                        fragments
                        );

            emit this->pageReady(page);
        }
        else {
            qCritical() << "Parsing JSON-LD data failed:" << parseError.errorString();
            emit this->error(QString("Parsing JSON-LD data failed: ").append(parseError.errorString()));
        }

    }
    else {
        qCritical() << "Network request failed! HTTP status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
        emit this->error(QString("Network request failed! HTTP status:").append(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()).append(reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
    }

    // Clean up the reply to avoid memory leaks
    reply->deleteLater();
}

// Getters & Setters
/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the Network::Manager instance
 * @return const Network::Manager *manager
 * @package Fragments
 * @private
 * Gets the Network::Manager instance and returns it.
 */
Network::Manager *Fragments::Factory::http() const
{
    return m_http;
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the Network::Manager instance
 * @param Network::Manager *http
 * @package Fragments
 * @private
 * Sets the Network::Manager instance to the given Network::Manager *http.
 */
void Fragments::Factory::setHttp(Network::Manager *http)
{
    m_http = http;
}

