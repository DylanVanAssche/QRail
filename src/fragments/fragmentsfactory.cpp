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
    connect(this, SIGNAL(getResource(QUrl, QObject *)), this->http(), SLOT(getResource(QUrl, QObject *)));

    // Create page cache
    //m_pageCache = new QRail::Fragments::Cache();

    // Create event source
    //m_eventSource = new QRail::Network::EventSource(QUrl(REAL_TIME_URL), QRail::Network::EventSource::Subscription::SSE);
    //connect(m_eventSource, SIGNAL(messageReceived(QString)), this, SLOT(handleEventSource(QString)));
}

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
void QRail::Fragments::Factory::getPage(const QUrl &uri, QObject *caller)
{
    QUrlQuery query = QUrlQuery(uri);
    QDateTime departureTime = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
    this->dispatcher()->addTarget(departureTime, caller);

    // Page is cached, dispatching!
    if(m_pageCache.hasPage(uri)) {
        //qDebug() << "Getting page from cache:" << uri;
        QRail::Fragments::Page *page = m_pageCache.getPageByURI(uri);
        /*qDebug() << "PAGE:" << page->uri();
        qDebug() << "\thydraNext:" << page->hydraNext();
        qDebug() << "\thydraPrevious:" << page->hydraPrevious();
        qDebug() << "\ttimestamp:" << page->timestamp();
        qDebug() << "\tfragment:" << page->fragments().at(0)->uri();*/
        this->dispatcher()->dispatchPage(page);
        return;
    }

    // Page is not in cache
    this->getPageByURIFromNetworkManager(uri);
}

void QRail::Fragments::Factory::getPage(const QDateTime &departureTime, QObject *caller)
{
    // Construct the URI of the page
    QUrl uri = QUrl(BASE_URL);
    QUrlQuery parameters;
    // Qt:ISODate returns 2018-07-27T14:18:40Z while we need 2018-07-27T14:18:40.000Z
    qDebug() << departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z");
    parameters.addQueryItem("departureTime", departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z"));
    uri.setQuery(parameters);
    this->dispatcher()->addTarget(departureTime, caller);

    // Page is cached, dispatching!
    if(m_pageCache.hasPage(uri)) {
        //qDebug() << "Getting page from cache:" << uri;
        QRail::Fragments::Page *page = m_pageCache.getPageByURI(uri);
        /*qDebug() << "PAGE:" << page->uri();
        qDebug() << "\thydraNext:" << page->hydraNext();
        qDebug() << "\thydraPrevious:" << page->hydraPrevious();
        qDebug() << "\ttimestamp:" << page->timestamp();
        qDebug() << "\tfragment:" << page->fragments().at(0)->uri();*/
        this->dispatcher()->dispatchPage(page);
        return;
    }

    // Page is not in cache
    this->getPageByURIFromNetworkManager(uri);
}

void QRail::Fragments::Factory::customEvent(QEvent *event)
{
    if (event->type() == this->http()->dispatcher()->eventType()) {
        event->accept();
        QRail::Network::DispatcherEvent *networkEvent = reinterpret_cast<QRail::Network::DispatcherEvent *>(event);
        this->processHTTPReply(networkEvent->reply());
    } else {
        event->ignore();
    }
}

void Fragments::Factory::handleEventSource(QString message)
{
    qDebug() << "Received SSE message:" << message;
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = doc.object();

    QJsonArray graph = jsonObject["@graph"].toArray();
    QList<QRail::Fragments::Fragment *> fragments = QList<QRail::Fragments::Fragment *>();
    foreach (QJsonValue item, graph) {
        if (item.isObject()) {
            QJsonObject connection = item.toObject();
            QRail::Fragments::Fragment *frag = this->generateFragmentFromJSON(connection);
            if (frag) {
                fragments.append(frag);
            } else {
                qCritical() << "Corrupt Fragment detected!";
            }
        } else {
            qCritical() << "Fragment isn't a JSON object!";
        }
    }

    // Linked Connections page
    QString pageURI = jsonObject["@id"].toString();
    QDateTime pageTimestamp = QDateTime::fromString(pageURI.right(24), Qt::ISODate); // TO DO REGEX
    QString hydraNext = jsonObject["hydra:next"].toString();
    QString hydraPrevious = jsonObject["hydra:previous"].toString();
    QRail::Fragments::Page *page = new QRail::Fragments::Page(pageURI, pageTimestamp, hydraNext, hydraPrevious, fragments);
    // Recache page, the old version is automatically deleted.
    m_pageCache.cachePage(page);
}

Fragments::Fragment::GTFSTypes Fragments::Factory::parseGTFSType(QString type)
{
    if(type == GTFS_REGULAR) {
        return QRail::Fragments::Fragment::GTFSTypes::REGULAR;
    }
    else if(type == GTFS_NOT_AVAILABLE) {
        return QRail::Fragments::Fragment::GTFSTypes::NOTAVAILABLE;
    }
    else if(type == GTFS_MUST_PHONE) {
        return QRail::Fragments::Fragment::GTFSTypes::MUSTPHONE;
    }
    else if(type == GTFS_MUST_COORDINATE_WITH_DRIVER) {
        return QRail::Fragments::Fragment::GTFSTypes::MUSTCOORDINATEWITHDRIVER;
    }
    else if(type.isEmpty()) {
        //qWarning() << "GTFS type is empty";
        return QRail::Fragments::Fragment::GTFSTypes::REGULAR;
    }

    qCritical() << "Unknown GTFS type, cannot parse:" << type;
    return QRail::Fragments::Fragment::GTFSTypes::UNKNOWN;
}

// Processors
void QRail::Fragments::Factory::getPageByURIFromNetworkManager(const QUrl &uri)
{
    // Call the getResource slot due different threads
    emit this->getResource(uri, this);
}

// Helpers
QRail::Fragments::Fragment *QRail::Fragments::Factory::generateFragmentFromJSON(const QJsonObject &data)
{
    // Parse JSON
    QUrl uri = QUrl(data["@id"].toString());
    QString type = data["@type"].toString();
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
    QString pickupType = data["gtfs:pickupType"].toString();
    QString dropOffType = data["gtfs:dropOffType"].toString();

    Q_UNUSED(type); // Only connections at the moment

    // Verify the extracted data before creating a Fragment
    if (departureStationURI.isValid() && arrivalStationURI.isValid()
            && departureTime.isValid() && arrivalTime.isValid() && tripURI.isValid()
            && routeURI.isValid() && !direction.isEmpty()) {

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
            this->parseGTFSType(pickupType),
            this->parseGTFSType(dropOffType),
            m_instance
        );
        return frag;
    }

    qCritical() << "Parsing failed, throwing fragment away: " << uri;
    return nullptr;
}

void QRail::Fragments::Factory::processHTTPReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
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
                        if (frag) {
                            fragments.append(frag);
                        } else {
                            qCritical() << "Corrupt Fragment detected!";
                        }
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
                m_pageCache.cachePage(page);
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
        qCritical() << "Network request failed! HTTP status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
        emit this->error(QString("Network request failed! HTTP status:").append(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()).append(reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
    }

    // Clean up the reply to avoid memory leaks
    reply->deleteLater();
}

// Getters & Setters
QRail::Network::Manager *QRail::Fragments::Factory::http() const
{
    return m_http;
}

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
