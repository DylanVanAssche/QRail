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

QRail::Fragments::Factory::Factory(QRail::Network::EventSource::Subscription subscriptionType, QObject *parent) : QObject(parent)
{
    // Setup the QRail::Network::Manager
    m_http = QRail::Network::Manager::getInstance();

    /*
     * QNAM and callers are living in different threads!
     * INFO:
     * https://stackoverflow.com/questions/3268073/qobject-cannot-create-children-for-a-parent-that-is-in-a-different-thread
     */
    connect(this, SIGNAL(getResource(QUrl)), m_http, SLOT(getResource(QUrl)));

    // Create event source
    if(subscriptionType == QRail::Network::EventSource::Subscription::POLLING) {
        qDebug() << "Instantiated EventSource POLL";
        m_eventSource = new QRail::Network::EventSource(QUrl(REAL_TIME_URL_POLL), QRail::Network::EventSource::Subscription::POLLING);
    }
    else {
        qDebug() << "Instantiated EventSource SSE";
        m_eventSource = new QRail::Network::EventSource(QUrl(REAL_TIME_URL_SSE), QRail::Network::EventSource::Subscription::SSE);
    }

    connect(m_eventSource, SIGNAL(messageReceived(QString)), this, SLOT(handleEventSource(QString)));

    // Create page cache
    this->setPageCache(new QRail::Fragments::Cache());
}

QRail::Fragments::Factory *QRail::Fragments::Factory::getInstance(QRail::Network::EventSource::Subscription subscriptionType)
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new Factory";
        m_instance = new QRail::Fragments::Factory(subscriptionType);
    }
    return m_instance;
}

// Invokers
void QRail::Fragments::Factory::getPage(const QUrl &uri)
{
    QUrlQuery query = QUrlQuery(uri);
    QDateTime departureTime = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
    //this->dispatcher()->addTarget(departureTime.toUTC(), caller);

    // Page is cached, dispatching!
    QSharedPointer<QRail::Fragments::Page> page = this->pageCache()->getPageByURI(uri);
    if(page) {
        //this->dispatcher()->dispatchPage(page);
        emit this->pageReady(page);
        qDebug() << "Retrieving page from cache...:" << uri;
        return;
    }

    // Page is not in cache
    qDebug() << "Retrieving page from server...:" << uri;
    this->getPageByURIFromNetworkManager(uri);
}

void QRail::Fragments::Factory::getPage(const QDateTime &departureTime)
{
    // Construct the URI of the page
    QUrl uri = QUrl(BASE_URL);
    QUrlQuery parameters;
    // Qt:ISODate returns 2018-07-27T14:18:40Z while we need 2018-07-27T14:18:40.000Z
    qDebug() << departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z");
    parameters.addQueryItem("departureTime", departureTime.toString(Qt::ISODate).replace(QRegularExpression("Z"), ".000Z"));
    uri.setQuery(parameters);
    //this->dispatcher()->addTarget(departureTime.toUTC(), caller);
    //qDebug() << "Dispatcher added target:" << departureTime.toUTC() << caller;

    // Page is cached, dispatching!
    QSharedPointer<QRail::Fragments::Page> page = this->pageCache()->getPageByURI(uri);
    if(page) {
        //this->dispatcher()->dispatchPage(page);
        qDebug() << "Retrieving page from cache...:" << uri;
        emit this->pageReady(page);
        return;
    }

    // Page is not in cache
    qDebug() << "Retrieving page from server...:" << uri;
    this->getPageByURIFromNetworkManager(uri);
}

void Fragments::Factory::handleEventSource(QString message)
{
    emit this->updateReceived(QDateTime::currentMSecsSinceEpoch());
    qDebug() << "Received Event Source message:" << message.length() << "chars";
    QJsonDocument doc = QJsonDocument::fromJson(message.toUtf8());
    QJsonObject jsonObject = doc.object();
    qDebug() << jsonObject["@graph"].toArray();

    QJsonArray graph = jsonObject["@graph"].toArray();
    foreach (QJsonValue item, graph) {
        if (item.isObject()) {
            QJsonObject event = item.toObject();
            QJsonObject connection = event["sosa:hasResult"].toObject()["Connection"].toObject();
            QSharedPointer<QRail::Fragments::Fragment> frag = this->generateFragmentFromJSON(connection);
            if (frag) {
                qDebug() << "Is object OK";
                QUrl updatedPageURI = QUrl(this->pageCache()->updateFragment(frag));
                qDebug() << "Updated page URI:" << updatedPageURI;

                if(updatedPageURI.isValid()) {
                    emit this->pageUpdated(updatedPageURI);
                    emit this->fragmentUpdated(frag);
                    emit this->fragmentAndPageUpdated(frag, updatedPageURI);
                }
            }
            else {
                qCritical() << "Corrupt Fragment detected!";
            }
        }
        else {
            qCritical() << "Fragment isn't a JSON object!";
        }
    }

    // Processing complete, let the listeners know that
    emit this->updateProcessed(QDateTime::currentMSecsSinceEpoch());
}

QRail::Fragments::Cache* QRail::Fragments::Factory::pageCache() const
{
    QMutexLocker lock(&m_cache_mutex);
    return m_pageCache;
}

void QRail::Fragments::Factory::setPageCache(QRail::Fragments::Cache* pageCache)
{
    QMutexLocker lock(&m_cache_mutex);
    m_pageCache = pageCache;
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
    // Async HTTP slot calling
    m_reply = QSharedPointer<QNetworkReply>(m_http->getResource(uri));
    connect(m_reply.data(), SIGNAL(finished()), this, SLOT(processHTTPReply()));
}

// Helpers
QSharedPointer<QRail::Fragments::Fragment> QRail::Fragments::Factory::generateFragmentFromJSON(const QJsonObject &data)
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
        QSharedPointer<QRail::Fragments::Fragment> frag = QSharedPointer<QRail::Fragments::Fragment>(new QRail::Fragments::Fragment(
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
                    this->parseGTFSType(dropOffType)
                    ));
        return frag;
    }

    qCritical() << "Parsing failed, throwing fragment away: " << uri;
    return nullptr;
}

void QRail::Fragments::Factory::processHTTPReply()
{
    qDebug() << "Processing HTTP reply";
    int statusCode = m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
#ifdef VERBOSE_HTTP_STATUS
        qDebug() << "Content-Header:"
                 << m_reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "Content-Length:"
                 << m_reply->header(QNetworkRequest::ContentLengthHeader).toULongLong()
                 << "bytes";
        qDebug() << "HTTP status:"
                 << m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()
                 << m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Cache:"
                 << m_reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool();
#endif

        // Read HTTP reply
        QString replyData = (QString)m_reply->readAll();

        // HTTP 200 OK, create LCPage and LCFragment list
        QList<QSharedPointer<QRail::Fragments::Fragment>> fragments = QList<QSharedPointer<QRail::Fragments::Fragment>>();

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
                        QSharedPointer<QRail::Fragments::Fragment> frag = this->generateFragmentFromJSON(connection);
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
                QUrl pageURI = QUrl(jsonObject["@id"].toString());
                QUrlQuery pageQuery = QUrlQuery(pageURI.query());
                QDateTime pageTimestamp = QDateTime::fromString(pageQuery.queryItemValue("departureTime"), Qt::ISODate);
                QString hydraNext = jsonObject["hydra:next"].toString();
                QString hydraPrevious = jsonObject["hydra:previous"].toString();
                QSharedPointer<QRail::Fragments::Page> page = QSharedPointer<QRail::Fragments::Page>(new QRail::Fragments::Page(pageURI, pageTimestamp, hydraNext, hydraPrevious, fragments));

                // Cache page for updates
                this->pageCache()->cachePage(page);

                // Page is ready for CSA/Liveboard
                emit this->pageReady(page);
            } else {
                qCritical() << "Fragments context validation failed!";
                emit this->error(QString("Fragments context validation failed!"));
            }
        } else {
            qCritical() << "Parsing JSON-LD data failed:" << parseError.errorString();
            emit this->error(QString("Parsing JSON-LD data failed: ").append(parseError.errorString()));
        }
    } else {
        qCritical() << "Network request failed! HTTP status:" << m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
        emit this->error(QString("Network request failed! HTTP status:").append(m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()).append(m_reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
    }
}
