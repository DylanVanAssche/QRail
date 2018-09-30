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
#include "fragments/fragmentsfactory.h"
using namespace QRail;
QRail::Fragments::Factory *QRail::Fragments::Factory::m_instance = nullptr;

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief QRail::Fragments::Factory constructor: empty
 * @param QObject *parent = nullptr
 * @package Fragments
 * @private
 * Constructs a QRail::Fragments::Factory to generate Linked Connections
 * fragments on the fly.
 */
QRail::Fragments::Factory::Factory(QObject *parent) : QObject(parent)
{
    // Setup the QRail::Network::Manager
    this->setHttp(QRail::Network::Manager::getInstance());

    // Setup dispatcher
    this->setDispatcher(new QRail::Fragments::Dispatcher());
    /*
     * QNAM and callers are living in different threads!
     * INFO:
     * https://stackoverflow.com/questions/3268073/qobject-cannot-create-children-for-a-parent-that-is-in-a-different-thread
     */
    connect(this, SIGNAL(getResource(QUrl, QObject *)),
            this->http(), SLOT(getResource(QUrl, QObject *)));
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a QRail::Fragments::Factory instance
 * @param QObject *parent = nullptr
 * @return Fragment::Factory *factory
 * @package Fragments
 * @public
 * Constructs a QRail::Fragments::Factory if none exists and returns the
 * instance.
 */
QRail::Fragments::Factory *QRail::Fragments::Factory::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new Factory";
        m_instance = new QRail::Fragments::Factory();
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
 * @param QObject *caller
 * @package Fragments
 * @public
 * Starts the generation process of a QRail::Fragments::Page object by
 * requesting a page by URI. When the page is ready, the pageReady signal will
 * be emitted.
 */
void QRail::Fragments::Factory::getPage(const QUrl &uri, QObject *caller)
{
    // Use processing methods to allow other extensions in the future if needed
    this->getPageByURIFromNetworkManager(uri);
    QUrlQuery query = QUrlQuery(uri);
    QDateTime departureTime = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
    this->dispatcher()->addTarget(departureTime, caller);
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Requests a page by departure time
 * @param const QDateTime &departureTime
 * @param QObject *caller
 * @package Fragments
 * @public
 * Starts the generation process of a QRail::Fragments::Page object by
 * requesting a page by departure time. When the page is ready, the pageReady
 * signal will be emitted.
 */
void QRail::Fragments::Factory::getPage(const QDateTime &departureTime, QObject *caller)
{
    // Construct the URI of the page
    QUrl uri = QUrl(BASE_URL);
    QUrlQuery parameters;
    // Qt:ISODate returns 2018-07-27T14:18:40Z while we need 2018-07-27T14:18:40.000Z
    qDebug() << departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z");
    parameters.addQueryItem("departureTime",
                            departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z"));
    uri.setQuery(parameters);

    // Use processing methods to allow other extensions in the future if needed
    this->getPageByURIFromNetworkManager(uri);
    this->dispatcher()->addTarget(departureTime, caller);
}

void QRail::Fragments::Factory::customEvent(QEvent *event)
{
    if (event->type() == this->http()->dispatcher()->eventType()) {
        event->accept();
        QRail::Network::DispatcherEvent *networkEvent = reinterpret_cast<QRail::Network::DispatcherEvent *>
                                                        (event);
        this->processHTTPReply(networkEvent->reply());
    } else {
        event->ignore();
    }
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
 * Sends a request to the QRail::Network::Manager to retrieve a Linked
 * Connections page by URI.
 */
void QRail::Fragments::Factory::getPageByURIFromNetworkManager(const QUrl &uri)
{
    // Call the getResource slot due different threads
    emit this->getResource(uri, this);
}

// Helpers
/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Parses the JSON-LD fragment
 * @param const QJsonObject &connection
 * @return QRail::Fragments::Fragment *frag;
 * @package Fragments
 * @private
 * Converts the JSON-LD into a QRail::Fragments::Fragments object and returns
 * it.
 */
QRail::Fragments::Fragment *
QRail::Fragments::Factory::generateFragmentFromJSON(const QJsonObject &data)
{
    // Parse JSON
    QUrl uri = QUrl(data["@id"].toString());
    QUrl departureStationURI = QUrl(data["departureStop"].toString());
    QUrl arrivalStationURI = QUrl(data["arrivalStop"].toString());
    QDateTime departureTime = QDateTime::fromString(data["departureTime"].toString(), Qt::ISODate);
    QDateTime arrivalTime = QDateTime::fromString(data["arrivalTime"].toString(), Qt::ISODate);
    QUrl tripURI = QUrl(data["gtfs:trip"].toString());
    QUrl routeURI = QUrl(data["gtfs:route"].toString());
    qint16 departureDelay = 0;
    qint16 arrivalDelay = 0;
    if (data.contains("departureDelay")) {
        departureDelay = data["departureDelay"].toInt();
    }
    if (data.contains("arrivalDelay")) {
        arrivalDelay = data["arrivalDelay"].toInt();
    }
    QString direction = data["direction"].toString();

    // Create Linked Connection Fragment and return it
    QRail::Fragments::Fragment *frag = new QRail::Fragments::Fragment(
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
 * @brief Processes the network reply
 * @param QNetworkReply *reply
 * @package Fragments
 * @private
 * Reads the network reply, parses it using the helper methods.
 * When the reply is completed parsed and no errors were encountered,
 * the pageReady signal is emitted.
 * In case we faced an error, the error signal is emitted with an error message.
 */
void QRail::Fragments::Factory::processHTTPReply(QNetworkReply *reply)
{
    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
#ifdef VERBOSE_HTTP_STATUS
        qDebug() << "Content-Header:"
                 << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "Content-Length:"
                 << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong()
                 << "bytes";
        qDebug() << "HTTP status:"
                 << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Cache:"
                 << reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool();
#endif

        // Read HTTP reply
        QString replyData = (QString)reply->readAll();

        // HTTP 200 OK, create LCPage and LCFragment list
        QList<QRail::Fragments::Fragment *> fragments = QList<QRail::Fragments::Fragment *>();

        // Try to parse the data as JSON-LD
        QJsonParseError parseError;
        QJsonDocument jsonData = QJsonDocument::fromJson(replyData.toUtf8(), &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            // Validate JSON-LD context
            QJsonObject jsonObject = jsonData.object();
            if (jsonObject["@context"].isObject() && pageContext() == jsonObject["@context"].toObject()) {

                // Linked Connections fragments
                QJsonArray graph = jsonObject["@graph"].toArray();
                foreach (QJsonValue item, graph) {
                    if (item.isObject()) {
                        QJsonObject connection = item.toObject();
                        QRail::Fragments::Fragment *frag = this->generateFragmentFromJSON(connection);
                        fragments.append(frag);
                    } else {
                        qCritical() << "Fragment isn't a JSON object!";
                    }
                }

                // Linked Connections page
                QString pageURI = jsonObject["@id"].toString();
                QDateTime pageTimestamp = QDateTime::fromString(pageURI.right(24), Qt::ISODate); // TO DO REGEX
                QString hydraNext = jsonObject["hydra:next"].toString();
                QString hydraPrevious = jsonObject["hydra:previous"].toString();
                QRail::Fragments::Page *page = new QRail::Fragments::Page(pageURI, pageTimestamp, hydraNext,
                                                                          hydraPrevious, fragments);
                this->dispatcher()->dispatchPage(page);
            } else {
                qCritical() << "Fragments context validation failed!";
                emit this->error(QString("Fragments context validation failed!"));
            }
        } else {
            qCritical() << "Parsing JSON-LD data failed:" << parseError.errorString();
            emit this->error(QString("Parsing JSON-LD data failed: ").append(parseError.errorString()));
        }
    } else {
        qCritical() << "Network request failed! HTTP status:" << reply->attribute(
                        QNetworkRequest::HttpStatusCodeAttribute).toString();
        emit this->error(QString("Network request failed! HTTP status:").append(reply->attribute(
                                                                                    QNetworkRequest::HttpStatusCodeAttribute).toString()).append(reply->attribute(
                                                                                                QNetworkRequest::HttpReasonPhraseAttribute).toString()));
    }

    // Clean up the reply to avoid memory leaks
    reply->deleteLater();
}

// Getters & Setters
/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the QRail::Network::Manager instance
 * @return const QRail::Network::Manager *manager
 * @package Fragments
 * @private
 * Gets the QRail::Network::Manager instance and returns it.
 */
QRail::Network::Manager *QRail::Fragments::Factory::http() const
{
    return m_http;
}

/**
 * @file fragmentsfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the QRail::Network::Manager instance
 * @param QRail::Network::Manager *http
 * @package Fragments
 * @private
 * Sets the QRail::Network::Manager instance to the given
 * QRail::Network::Manager *http.
 */
void QRail::Fragments::Factory::setHttp(QRail::Network::Manager *http)
{
    m_http = http;
}

QRail::Fragments::Dispatcher *QRail::Fragments::Factory::dispatcher() const
{
    return m_dispatcher;
}

void QRail::Fragments::Factory::setDispatcher(QRail::Fragments::Dispatcher *dispatcher)
{
    m_dispatcher = dispatcher;
}
