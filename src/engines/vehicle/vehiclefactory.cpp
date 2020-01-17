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
#include "engines/vehicle/vehiclefactory.h"
using namespace QRail;
QRail::VehicleEngine::Factory *QRail::VehicleEngine::Factory::m_instance = nullptr;

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief QRail::VehicleEngine::Factory constructor
 * @param QObject *parent = nullptr
 * @package VehicleEngine
 * @private
 * Constructs a QRail::VehicleEngine::Factory to
 * generate QRail::VehicleEngine::Vehicle objects on the fly.
 */
QRail::VehicleEngine::Factory::Factory(QObject *parent) : QObject(parent)
{
    // Setup Network::Manager
    this->setHttp(QRail::Network::Manager::getInstance());
    /*
     * QNAM and callers are living in different threads!
     * INFO:
     * https://stackoverflow.com/questions/3268073/qobject-cannot-create-children-for-a-parent-that-is-in-a-different-thread
     */
    connect(this, SIGNAL(getResource(QUrl, QObject *)), this->http(), SLOT(getResource(QUrl, QObject *)));

    // Setup StationEngine::Factory
    this->setStationFactory(StationEngine::Factory::getInstance());

    // Init caching
    m_cache = QMap<QString, QRail::VehicleEngine::Vehicle *>();
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Gets a QRail::VehicleEngine::Factory instance
 * @param QObject *parent = nullptr
 * @return QRail::VehicleEngine::Factory *factory
 * @package VehicleEngine
 * @public
 * Constructs a QRail::VehicleEngine::Factory if none exists and returns the
 * instance.
 */
QRail::VehicleEngine::Factory *QRail::VehicleEngine::Factory::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new QRail::VehicleEngine::Factory";
        m_instance = new QRail::VehicleEngine::Factory();
    }
    return m_instance;
}

// Invokers
/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Retrieves a vehicle by URI
 * @param const QUrl &uri
 * @package StationEngine
 * @public
 * Retrieves a vehicle by URI from the network iRail API.
 * When the vehicle is ready, the finished signal is emitted.
 * In case something goes wrong, a QRail::VehicleEngine::NullVehicle is returned.
 */
void QRail::VehicleEngine::Factory::getVehicleByURI(const QUrl &uri,
                                                    const QLocale::Language &language)
{
    if (uri.isValid()) {
        if(!vehicleProcessingMutex.tryLock(LOCK_TIMEOUT)) {
            emit this->error("Vehicle factory is busy. Please try again later.");
            return;
        }
        this->setLanguage(language);
        QRail::VehicleEngine::Vehicle *vehicle = this->fetchVehicleFromCache(uri);
        // Vehicle isn't in our cache yet, fetching...
        if (!vehicle) {
            emit this->getResource(uri, this);
        }
        // Vehicle found in cache, emitting directly the finished signal
        else {
            emit this->finished(vehicle);
        }
    }
    else {
        qCritical() << "Vehicle URI invalid";
        qCritical() << "URI:" << uri;
        emit this->finished(QRail::VehicleEngine::NullVehicle::getInstance());
    }
}

// Processors
/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Processes a received HTTP reply
 * @param QNetworkReply *reply
 * @package VehicleEngine
 * @private
 * Reads the QNetworkReply *reply and validates it before the processing begins.
 * After the validation, the reply is processed and a
 * QRail::VehicleEngine::Vehicle is generated with it's
 * QRail::VehicleEngine::Stop list.
 */
void QRail::VehicleEngine::Factory::processHTTPReply(QNetworkReply *reply)
{
    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (statusCode >= 200 && statusCode < 300) {
#ifdef VERBOSE_HTTP_STATUS
        qDebug() << "Content-Header:" << reply->header(QNetworkRequest::ContentTypeHeader).toString();
        qDebug() << "Content-Length:" << reply->header(QNetworkRequest::ContentLengthHeader).toULongLong()
                 << "bytes";
        qDebug() << "HTTP status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() <<
                    reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
        qDebug() << "Cache:" << reply->attribute(QNetworkRequest::SourceIsFromCacheAttribute).toBool();
#endif
        // Read HTTP reply
        QString replyData = (QString)reply->readAll();

        // Try to parse the data as JSON-LD
        QJsonParseError parseError;
        QJsonDocument jsonData =
                QJsonDocument::fromJson(replyData.toUtf8(), &parseError);
        if (parseError.error == QJsonParseError::NoError) {
            QJsonObject jsonObject = jsonData.object();

            if (jsonObject["@context"].isObject() &&
                    vehicleContext() == jsonObject["@context"].toObject()) {
                // Vehicle intermediary stops
                QList<QRail::VehicleEngine::Stop *> intermediaryStops =
                        QList<QRail::VehicleEngine::Stop *>();
                QJsonArray graph = jsonObject["@graph"].toArray();
                QString tripDate;
                for (qint16 i = 0; i < graph.size(); i++) {
                    // Generate QRail::VehicleEngine::Stop objects for the vehicle
                    QJsonObject item = graph.at(i).toObject();
                    QRail::VehicleEngine::Stop *stop = this->generateStopFromJSON(item);

                    // The first and last QRail::VehicleEngine::Stop objects are the
                    // departure and arrival stops of the vehicle
                    if (i == 0) {
                        stop->setType(QRail::VehicleEngine::Stop::Type::DEPARTURE);
                        tripDate =
                                item["departureConnection"]
                                .toString(); // Remove http://irail.be/connections/8891173/
                        // We need to escape the C compiler and the RegEx: '\\'
                        QRegularExpression re("^(http:\\/\\/\\w+\\.\\w+\\/\\w+\\/\\d{7})\\/"
                                              "(\\d{8})\\/(.\\d{3})");
                        QRegularExpressionMatch match = re.match(tripDate);
                        if (match.hasMatch()) {
                            tripDate = match.captured(2); // Get the 2nd group (\d{8})
                            qDebug() << "tripDate:" << tripDate;
                        } else {
                            qCritical() << "Unable to retrieve trip date for the vehicle";
                        }
                    } else if (i == graph.size() - 1) {
                        stop->setType(QRail::VehicleEngine::Stop::Type::ARRIVAL);
                    }

                    // Add stop to the list
                    intermediaryStops.append(stop);
                }

                /*
                * Construct a vehicle object with the parsed JSON-LD data.
                *
                * The headsign of a vehicle is the destination station of the
                * vehicle (http://vocab.org/transit/terms/headsign).
                * Since our stops are already sorted by the resource we can pick
                * the last stop and retrieve the name of it without querying other
                * resources.
                */
                QRail::VehicleEngine::Vehicle *vehicle = new QRail::VehicleEngine::Vehicle(
                            reply->url(), // Vehicle URI
                            QUrl(reply->url().toString().append("/" + tripDate)),
                            intermediaryStops.last()->station()->name().value(this->language()),
                            intermediaryStops);

                /*
                * Add the vehicle to cache for faster responses in the future and less
                * memory usage due duplicate objects.
                */
                this->addVehicleToCache(vehicle->uri(), vehicle);

                // Emit finished signal for the caller
                emit this->finished(vehicle);
            } else {
                qCritical() << "Validation vehicle context failed!";
                emit this->error(QString("Validation vehicle context failed!"));
            }
        } else {
            qCritical() << "Parsing JSON-LD data failed:" << parseError.errorString();
            emit this->error(QString("Parsing JSON-LD data failed: ")
                             .append(parseError.errorString()));
        }

    } else {
        qCritical() << "Network request failed! HTTP status:" << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString();
        qCritical() << "\tReply:" << (QString)reply->readAll();
        emit this->error(QString("Network request failed! HTTP status:").append(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toString()).append(reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString()));
    }

    // Clean up the reply to avoid memory leaks
    reply->deleteLater();
}

// Helpers
/*void QRail::VehicleEngine::Factory::customEvent(QEvent *event)
{
    if (event->type() == this->http()->dispatcher()->eventType()) {
        event->accept();
        QRail::Network::DispatcherEvent *networkEvent = reinterpret_cast<QRail::Network::DispatcherEvent *>(event);
        this->processHTTPReply(networkEvent->reply());
    } else {
        event->ignore();
    }
}*/

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Generates QRail::VehicleEngine::Stop from JSON-LD
 * @param const QJsonObject &stop
 * @return QRail::VehicleEngine::Stop *stop
 * @package VehicleEngine
 * @private
 * Generates QRail::VehicleEngine::Stop from JSON-LD and returns it.
 */
QRail::VehicleEngine::Stop *QRail::VehicleEngine::Factory::generateStopFromJSON(
        const QJsonObject &stop)
{
    // Parse JSON-LD data
    QString platform = stop["platforminfo"].toObject()["name"].toString();
    bool isPlatformNormal = stop["platforminfo"].toObject()["normal"].toBool();
    bool hasLeft = stop["left"].toBool();
    qint16 departureDelay = (qint16)stop["departureDelay"].toInt();
    QDateTime departureTime;
    departureTime.setTime_t(stop["scheduledDepartureTime"].toString().toUInt()); // Qt 5.9 deprecated: https://doc.qt.io/qt-5/qdatetime.html#fromSecsSinceEpoch
    departureTime = departureTime.addSecs(departureDelay);
    bool isDepartureCanceled = stop["departureCanceled"].toBool();
    qint16 arrivalDelay = (qint16)stop["arrivalDelay"].toInt();
    QDateTime arrivalTime;
    arrivalTime.setTime_t(stop["scheduledArrivalTime"].toString().toUInt()); // Qt 5.9 deprecated: https://doc.qt.io/qt-5/qdatetime.html#fromSecsSinceEpoch
    arrivalTime = arrivalTime.addSecs(arrivalDelay);
    bool isArrivalCanceled = stop["arrivalCanceled"].toBool();
    bool isExtraStop = stop["isExtraStop"].toBool();
    QJsonObject occupancyLevel = stop["occupancy"].toObject();
    QRail::VehicleEngine::Stop::Type type = QRail::VehicleEngine::Stop::Type::STOP;
    QUrl stationURI = QUrl(stop["stationinfo"].toObject()["@id"].toString());

    // Generate QRail::VehicleEngine::Stop object
    return new QRail::VehicleEngine::Stop(
                QUrl(stationURI),
                this->stationFactory()->getStationByURI(stationURI), platform,
                isPlatformNormal, hasLeft, departureTime, departureDelay,
                isDepartureCanceled, arrivalTime, arrivalDelay, isArrivalCanceled,
                isExtraStop, this->generateOccupancyLevelFromJSON(occupancyLevel), type);
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Generates QRail::VehicleEngine::Stop::OccupancyLevel from JSON-LD
 * @param const QJsonObject &occupancy
 * @return QRail::VehicleEngine::Stop::OccupancyLevel level
 * @package VehicleEngine
 * @private
 * Generates QRail::VehicleEngine::Stop::OccupancyLevel from JSON-LD and
 * returns it.
 */
QRail::VehicleEngine::Stop::OccupancyLevel
QRail::VehicleEngine::Factory::generateOccupancyLevelFromJSON(const QJsonObject &occupancy) const
{
    QUrl occupancyURI = QUrl(occupancy["@id"].toString());

    if (occupancyURI == QUrl("http://api.irail.be/terms/unknown")) {
        return QRail::VehicleEngine::Stop::OccupancyLevel::UNKNOWN;
    } else if (occupancyURI == QUrl("http://api.irail.be/terms/low")) {
        return QRail::VehicleEngine::Stop::OccupancyLevel::LOW;
    } else if (occupancyURI == QUrl("http://api.irail.be/terms/medium")) {
        return QRail::VehicleEngine::Stop::OccupancyLevel::MEDIUM;
    } else if (occupancyURI == QUrl("http://api.irail.be/terms/high")) {
        return QRail::VehicleEngine::Stop::OccupancyLevel::HIGH;
    } else {
        return QRail::VehicleEngine::Stop::OccupancyLevel::UNKNOWN;
    }
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 31 Aug 2018
 * @brief Tries to fetch a vehicle from the cache
 * @return QRail::VehicleEngine::Vehicle *vehicle
 * @package VehicleEngine
 * @private
 * Tries to fetch a vehicle from the cache.
 * In case the vehicle can't be retrieved from the cache, a nullptr is returned.
 */
VehicleEngine::Vehicle *VehicleEngine::Factory::fetchVehicleFromCache(const QUrl &uri)
{
    QString id = this->stripIDFromVehicleURI(uri);
#ifdef VERBOSE_CACHE
    qDebug() << "Vehicle cache:";
    foreach (QString key, m_cache.keys()) {
        qDebug() << "\t" << key << m_cache.value(key);
    }
#endif

    if (m_cache.contains(id)) {
#ifdef VERBOSE_CACHE
        qDebug() << "Found vehicle in cache:" << id;
#endif
        return this->m_cache.value(id);
    }
    return nullptr;
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 31 Aug 2018
 * @brief Adds a vehicle to the cache
 * @param const QUrl &uri
 * @param QRail::VehicleEngine::Vehicle *vehicle
 * @package VehicleEngine
 * @private
 * Adds a vehicle to the cache.
 * In case the URI of the vehicle already exists in the cache, the entry is
 * updated with the new vehicle.
 */
void VehicleEngine::Factory::addVehicleToCache(const QUrl &uri, VehicleEngine::Vehicle *vehicle)
{
    QString id = this->stripIDFromVehicleURI(uri);
    m_cache.insert(id, vehicle);
#ifdef VERBOSE_CACHE
    qDebug() << "Added vehicle to cache:" << id;
#endif
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 31 Aug 2018
 * @brief Strips the vehicle ID from it's URI.
 * @param const QUrl &uri
 * @package VehicleEngine
 * @private
 * Strips the vehicle ID from the URI and returns it.
 */
QString VehicleEngine::Factory::stripIDFromVehicleURI(const QUrl &uri)
{
    /*
     * Avoiding cache misses due HTTPS upgrades.
     * We can't guarantee that our data resource already use HTTPS for it's links.
     * If not, the links might upgrade to HTTPS for security reasons.
     * To avoid cache misses we only use internally the ID of the vehicle instead of the complete URI.
     * Escaping C strings and Regex by using '\\'
     */
    QString id;
    QRegularExpression re("(^https:\\/\\/|^http:\\/\\/)(\\w+\\.\\w+\\/\\w+\\/)(\\w+)");
    QRegularExpressionMatch match = re.match(uri.toString());
    if (match.hasMatch()) {
        id = match.captured(3); // Get the 3nd group (\\w+)
#ifdef VERBOSE_CACHE
        qDebug() << "Stripped ID:" << id;
#endif
    } else {
        qCritical() << "Unable to retrieve ID for the vehicle";
    }
    return id;
}

// Getters & Setters
/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Gets the QRail::Network::Manager instance
 * @return QRail::Network::Manager *http
 * @package VehicleEngine
 * @private
 * Gets the QRail::Network::Manager instance and returns it.
 */
QRail::Network::Manager *QRail::VehicleEngine::Factory::http() const
{
    return m_http;
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Sets the QRail::Network::Manager instance
 * @param QRail::Network::Manager *http
 * @package VehicleEngine
 * @private
 * Sets the QRail::Network::Manager instance to the given
 * QRail::Network::Manager *http.
 */
void QRail::VehicleEngine::Factory::setHttp(QRail::Network::Manager *http)
{
    m_http = http;
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Gets the StationEngine::Factory instance
 * @return StationEngine::Factory *stationFactory
 * @package VehicleEngine
 * @private
 * Gets the StationEngine::Factory instance and returns it.
 */
StationEngine::Factory *QRail::VehicleEngine::Factory::stationFactory() const
{
    return m_stationFactory;
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Sets the StationEngine::Factory instance
 * @param StationEngine::Factory *stationFactory
 * @package VehicleEngine
 * @private
 * Sets the StationEngine::Factory instance to the given
 * StationEngine::Factory *stationFactory.
 */
void QRail::VehicleEngine::Factory::setStationFactory(StationEngine::Factory *stationFactory)
{
    m_stationFactory = stationFactory;
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Gets the language
 * @return QRail::Network::Manager *http
 * @package VehicleEngine
 * @private
 * Gets the language and returns it.
 */
QLocale::Language QRail::VehicleEngine::Factory::language() const
{
    return m_language;
}

/**
 * @file vehiclefactory.cpp
 * @author Dylan Van Assche
 * @date 27 Aug 2018
 * @brief Sets the language
 * @param const QLocale::Language &language
 * @package VehicleEngine
 * @private
 * Sets the language to the given const QLocale::Language &language.
 */
void QRail::VehicleEngine::Factory::setLanguage(const QLocale::Language &language)
{
    m_language = language;
}
