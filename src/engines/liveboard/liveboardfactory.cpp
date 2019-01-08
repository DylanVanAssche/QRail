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
#include "engines/liveboard/liveboardfactory.h"
using namespace QRail;
QRail::LiveboardEngine::Factory *QRail::LiveboardEngine::Factory::m_instance = nullptr;

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief QRail::LiveboardEngine::Factory constructor
 * @param QObject *parent = nullptr
 * @package Liveboard
 * @private
 * Constructs a QRail::LiveboardEngine::Factory to generate
 * QRail::LiveboardEngine::Board objects on the fly.
 */
QRail::LiveboardEngine::Factory::Factory(QObject *parent) : QObject(parent)
{
    // Get QRail::Fragments::Factory instance
    this->setFragmentsFactory(QRail::Fragments::Factory::getInstance());
    connect(this->fragmentsFactory(), SIGNAL(error(QString)), SLOT(handleFragmentFactoryError()));

    // Get StationEngine::Factory instance
    this->setStationFactory(StationEngine::Factory::getInstance());

    // Abort is default false
    this->setAbortRequested(false);

    // Timeout preventing
    this->progressTimeoutTimer = new QTimer(this);
    this->progressTimeoutTimer->setInterval(HTTP_TIMEOUT);
    connect(this->progressTimeoutTimer, SIGNAL(timeout()), this, SLOT(handleTimeout()));

    // Connect signals
    connect(this, SIGNAL(finished(QRail::LiveboardEngine::Board*)), this, SLOT(unlockLiveboard()));
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets a QRail::LiveboardEngine::Factory instance
 * @param QObject *parent = nullptr
 * @return QRail::LiveboardEngine::Factory *factory
 * @package Liveboard
 * @public
 * Constructs a QRail::LiveboardEngine::Factory if none exists and returns the
 * instance.
 */
QRail::LiveboardEngine::Factory *QRail::LiveboardEngine::Factory::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new QRail::LiveboardEngine::Factory";
        m_instance = new QRail::LiveboardEngine::Factory();
    }
    return m_instance;
}

// Invokers
/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Retrieves a liveboard by a station URI
 * @param const QUrl &url
 * @param const QRail::LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @overload
 * @public
 * Retrieves a QRail::LiveboardEngine::Board for a station given by it's URI.
 * The QRail::LiveboardEngine::Board::Mode &mode parameter determines if the the
 * liveboard should contain all the arrivals, departures of the station.
 * Calling this method will retrieve a QRail::LiveboardEngine::Board for the
 * current time until the current time + 1 hour.
 */
void QRail::LiveboardEngine::Factory::getLiveboardByStationURI(const QUrl &uri,
                                                               const QRail::LiveboardEngine::Board::Mode &mode)
{
    this->getLiveboardByStationURI(uri,
                                   QDateTime::currentDateTime().toUTC(),
                                   QDateTime::currentDateTime().addSecs(0.5 * SECONDS_TO_HOURS_MULTIPLIER).toUTC(),
                                   mode);
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Retrieves a liveboard by a station URI and a time range
 * @param const QUrl &url
 * @param const QDateTime &from
 * @param const QDateTime &until
 * @param const QRail::LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @overload
 * @public
 * Retrieves a QRail::LiveboardEngine::Board for a station given by it's URI.
 * The QRail::LiveboardEngine::Board::Mode &mode parameter determines if the the
 * liveboard should contain all the arrivals, departures of the station.
 */
void QRail::LiveboardEngine::Factory::getLiveboardByStationURI(const QUrl &uri,
                                                               const QDateTime &from,
                                                               const QDateTime &until,
                                                               const QRail::LiveboardEngine::Board::Mode &mode)
{
    if(!liveboardProcessingMutex.tryLock(LOCK_TIMEOUT)) {
        qDebug() << "Emiting locked error";
        emit this->error("Liveboard factory is busy. Please try again later.");
        return;
    }

    if (uri.isValid() && from.isValid() && until.isValid()) {
        this->setAbortRequested(false);
        this->deleteUsedPages(); // Clean up previous pages if needed
        this->setStationURI(uri);
        this->setMode(mode);
        this->setFrom(from);
        this->setUntil(until);
        this->setLiveboard(new QRail::LiveboardEngine::Board(this));
        this->liveboard()->setEntries(QList<QRail::VehicleEngine::Vehicle *>());
        this->liveboard()->setFrom(this->from());
        this->liveboard()->setUntil(this->until());
        this->liveboard()->setMode(this->mode());
        this->liveboard()->setStation(this->stationFactory()->getStationByURI(this->stationURI()));
        this->initUsedPages();
        this->setIsExtending(false);
        this->fragmentsFactory()->getPage(this->from(), this);
        this->progressTimeoutTimer->start();
    } else {
        qCritical() << "Station URI or timestamps are invalid";
        qCritical() << "URI:" << uri;
        qCritical() << "From:" << from;
        qCritical() << "Until:" << until;
        emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance());
    }
}

void LiveboardEngine::Factory::getNextResultsForLiveboard(LiveboardEngine::Board *board)
{
    if (board->hydraNext().isValid() && board->hydraNext().hasQuery()) {
        liveboardProcessingMutex.lock(); // Processing started
        this->setStationURI(board->station()->uri());
        this->setMode(board->mode());
        this->setFrom(board->until());
        this->setLiveboard(board);
        this->initUsedPages();
        QUrlQuery query = QUrlQuery(board->hydraNext().query());
        QDateTime timestamp = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
        this->setUntil(timestamp);
        this->setIsExtending(true);
        m_extendingDirection = QRail::LiveboardEngine::Factory::Direction::NEXT;
        this->fragmentsFactory()->getPage(board->hydraNext(), this);
        this->progressTimeoutTimer->start();
    } else {
        qCritical() << "hydraNext URI invalid, can't extend liveboard";
    }
}

void LiveboardEngine::Factory::getPreviousResultsForLiveboard(LiveboardEngine::Board *board)
{
    if (board->hydraPrevious().isValid() && board->hydraPrevious().hasQuery()) {
        liveboardProcessingMutex.lock(); // Processing started
        this->setStationURI(board->station()->uri());
        this->setMode(board->mode());
        this->setUntil(board->from());
        this->setLiveboard(board);
        this->initUsedPages();
        QUrlQuery query = QUrlQuery(board->hydraPrevious().query());
        QDateTime timestamp = QDateTime::fromString(query.queryItemValue("departureTime"), Qt::ISODate);
        qCritical() << "Extending previous timestamp=" << timestamp;
        this->setFrom(timestamp);
        this->setIsExtending(true);
        m_extendingDirection = QRail::LiveboardEngine::Factory::Direction::PREVIOUS;
        this->fragmentsFactory()->getPage(board->hydraPrevious(), this);
        this->progressTimeoutTimer->start();
    } else {
        qCritical() << "hydraPrevious URI invalid, can't extend liveboard";
    }
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 25 Nov 2018
 * @brief Cancel the current operation.
 * @package Liveboard
 * @public
 * If the user wants to cancel the current operation,
 * this method must be called before performing a new action.
 */
void LiveboardEngine::Factory::abortCurrentOperation()
{
    qInfo() << "Abort registered, processing!";
    this->setAbortRequested(true);
}

// Helpers
/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Handler for the received QRail::Fragments::Page pages.
 * @param QRail::Fragments::Page *page
 * @package Liveboard
 * @public
 * Handles the incoming QRail::Fragments::Page pages from the
 * QRail::Fragments::Factory. New pages are requested if the time range isn't
 * fulfilled in DESCENDING order of the departure time.
 */
void QRail::LiveboardEngine::Factory::processPage(QRail::Fragments::Page *page)
{
    qDebug() << "Factory generated requested Linked Connection page:"
             << page
             << "starting processing thread...";

    // Launch processing thread
    emit this->processing(page->uri());

    // Queue page for deletion when Liveboard is ready and reset timeout timer
    this->addUsedPage(page);
    this->progressTimeoutTimer->start();

    /*
    * Before processing our received page we check if we the first fragment
    * passed our departure time. We can do this because the departure times are
    * sorted in DESCENDING order.
    *
    * Or if we're extending the liveboard we keep searching until a result has
    * been retrieved.
    */
    bool finished = false;
    QUrlQuery queryHydraNext = QUrlQuery(page->hydraNext().query());
    QDateTime timeHydraNext = QDateTime::fromString(queryHydraNext.queryItemValue("departureTime"), Qt::ISODate);

    // Extending requires a small change in the auto fetching system
    if(!this->isExtending()) {
        if (timeHydraNext < this->until() && !this->isAbortRequested()) {
            qDebug() << "Requesting another page from QRail::Fragments::Factory automatically";
            this->fragmentsFactory()->getPage(page->hydraNext(), this);
            emit this->requested(page->hydraNext());
        } else {
            finished = true;
        }
    }
    this->parsePage(page, finished);
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Parser thread for the received pages
 * @param QRail::Fragments::Page *page
 * @param const bool &finished
 * @package Liveboard
 * @note If you want to retrieve the full intermediary stops of each vehicle in the QRail::LiveboardEngine::Board you can use the QRail::VehicleEngine::Factory.
 * @public
 * Parses the incoming pages in a separate thread using the QtConcurrent framework.
 * If the finished parameter is set to true, the LiveboardEngine::Board is ready and
 * the finished signal is emitted.
 */
void QRail::LiveboardEngine::Factory::parsePage(QRail::Fragments::Page *page, bool &finished)
{
    // Update hydra pagination
    this->liveboard()->setHydraNext(page->hydraNext());
    this->liveboard()->setHydraPrevious(page->hydraPrevious());

    // Parse each connection fragment
    bool hasResult = false;
    for (qint16 fragIndex = 0; fragIndex < page->fragments().size(); fragIndex++) {
        QRail::Fragments::Fragment *fragment = page->fragments().at(fragIndex);

        // Current operation aborted by the user
        if(this->isAbortRequested()) {
            this->setAbortRequested(false);
            qInfo() << "Aborted successfully in FOR loop";
            emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance());
            return;
        }

        // Lazy construction
        if (
                // ARRIVAL mode
                (this->mode() == QRail::LiveboardEngine::Board::Mode::ARRIVALS
                 && fragment->arrivalStationURI() == this->stationURI()
                 && fragment->dropOffType() == QRail::Fragments::Fragment::GTFSTypes::REGULAR)
                ||
                // DEPARTURE mode
                (this->mode() == QRail::LiveboardEngine::Board::Mode::DEPARTURES
                 && fragment->departureStationURI() == this->stationURI()
                 && fragment->pickupType() == QRail::Fragments::Fragment::GTFSTypes::REGULAR)) {

            /*
            * Create stop information
            * TO DO:
            *       - Add platform and canceled information.
            *         This is still unsupported by the Linked Connections graph!
            *       - Add arrival time or departure time when Liveboard::Mode is inverted,
            *         this requires vehicle route data.
            *         This is still unsupported by the Linked Connections graph!
            *
            *          Departure                    Arrival
            *           +-----------------------------+
            *
            *          /!\ Using arrival time for the departure stop as arrival time is wrong!
            *              Circumventing by setting the arrival and departure time of a stop to
            *              the same depending on the Liveboard::Mode.
            */

            QRail::VehicleEngine::Stop *entry = nullptr;
            if (this->mode() == QRail::LiveboardEngine::Board::Mode::DEPARTURES) {
                entry = new QRail::VehicleEngine::Stop(
                            this->liveboard()->station(),
                            QString("?"), // platform
                            true,         // isPlatformNormal
                            fragment->departureTime() >= QDateTime::currentDateTime(), // hasLeft
                            fragment->departureTime().addSecs(-fragment->departureDelay()), // Delays are included, remove them
                            fragment->departureDelay(),
                            false, // isDepartureCanceled
                            fragment->departureTime().addSecs(-fragment->departureDelay()),
                            fragment->departureDelay(),
                            false, // isArrivalCanceled
                            false, // isExtraStop
                            QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED,
                            QRail::VehicleEngine::Stop::Type::STOP
                            );
            } else if (this->mode() == QRail::LiveboardEngine::Board::Mode::ARRIVALS) {
                entry = new QRail::VehicleEngine::Stop(
                            this->liveboard()->station(),
                            QString("?"), // platform
                            true,         // isPlatformNormal
                            fragment->arrivalTime() >= QDateTime::currentDateTime(), // hasLeft
                            fragment->arrivalTime().addSecs(-fragment->arrivalDelay()), // Delays are included, remove them
                            fragment->arrivalDelay(),
                            false, // isDepartureCanceled
                            fragment->arrivalTime().addSecs(-fragment->arrivalDelay()),
                            fragment->arrivalDelay(),
                            false, // isArrivalCanceled
                            false, // isExtraStop
                            QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED,
                            QRail::VehicleEngine::Stop::Type::STOP
                            );
            } else {
                qCritical() << "Unknown LiveboardEngine::Board::Mode, can't fill Liveboard::Board entries!";
            }


            // Get vehicle information
            QList<QRail::VehicleEngine::Stop *> intermediaryStops = QList<QRail::VehicleEngine::Stop *>();
            intermediaryStops.append(entry);
            QRail::VehicleEngine::Vehicle *vehicle =
                    new QRail::VehicleEngine::Vehicle(
                        fragment->routeURI(),
                        fragment->tripURI(),
                        fragment->direction(),
                        intermediaryStops
                        );
            this->liveboard()->addEntry(vehicle);
            this->stream(vehicle);
            hasResult = true;
            qDebug() << "RESULT=" << vehicle->uri() << " headsign:" << vehicle->headsign();
        }
    }

    // Fetch if needed when extending
    if(this->isExtending()) {
        if(hasResult) {
            qDebug() << "Extending found at least 1 result, finishing...";
            finished = true;
        }
        else {
            if(this->isAbortRequested()) {
                finished = true;
                qDebug() << "Extending canceled due ABORT";
            }
            else {
                qDebug() << "Extending couldn't find any results in this page, fetching a new one";
                if(m_extendingDirection == QRail::LiveboardEngine::Factory::Direction::PREVIOUS) {
                    this->fragmentsFactory()->getPage(page->hydraPrevious(), this);
                    emit this->requested(page->hydraPrevious());
                }
                else if(m_extendingDirection == QRail::LiveboardEngine::Factory::Direction::NEXT) {
                    this->fragmentsFactory()->getPage(page->hydraNext(), this);
                    emit this->requested(page->hydraNext());
                }
                else {
                    qCritical() << "Unable to search further for extension, unknown direction";
                }
            }
        }
    }

    // Fetching fragment pages complete, emit the finished signal
    if (finished) {
        qDebug() << "Finished fetching liveboard pages";

        // Extending is always ended here
        this->setIsExtending(false);

        // Update the time boundaries of the liveboard
        // TO DO: Handle ARRIVALS and DEPARTURES mode, both timestamps are the same at the moment so this valid.
        if(this->liveboard()->entries().length() > 0) {
            this->liveboard()->setFrom(this->liveboard()->entries().first()->intermediaryStops().first()->departureTime());
            this->liveboard()->setUntil(this->liveboard()->entries().last()->intermediaryStops().first()->departureTime());
            emit this->finished(this->liveboard());
        }
        // No entries, NullBoard!
        else {
            emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance());
        }
    }
}

// Getters & Setters
/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the QRail::LiveboardEngine::Board instance
 * @return QRail::LiveboardEngine::Board *liveboard
 * @package Liveboard
 * @public
 * Gets the QRail::LiveboardEngine::Board instance we're currently using and
 * returns it.
 */
QRail::LiveboardEngine::Board *QRail::LiveboardEngine::Factory::liveboard() const
{
    // Lock using QMutexLocker due concurrent access
    QMutexLocker locker(&liveboardAccessMutex);
    return m_liveboard;
}

void QRail::LiveboardEngine::Factory::customEvent(QEvent *event)
{
    qDebug() << "Factory liveboard received event";
    if (event->type() == this->fragmentsFactory()->dispatcher()->eventType()) {
        event->accept();
        QRail::Fragments::DispatcherEvent *pageEvent =
                reinterpret_cast<QRail::Fragments::DispatcherEvent *>(event);
        this->processPage(pageEvent->page());
        qDebug() << "Network event received!";
    } else {
        event->ignore();
    }
}

void LiveboardEngine::Factory::unlockLiveboard()
{
    // Make liveboard accessible again
    liveboardProcessingMutex.unlock();
}

void LiveboardEngine::Factory::handleTimeout()
{
    qCritical() << "Liveboard timed out, ABORTING NOW";
    this->setAbortRequested(true);
    emit this->error("Liveboard timed out, the operation has been aborted!");
    emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance()); // NULL JOURNEY
}

void LiveboardEngine::Factory::handleFragmentFactoryError()
{
    qCritical() << "Liveboard fragment factory error, ABORTING NOW";
    this->setAbortRequested(true);
    emit this->error("Liveboard fragment factory error, the operation has been aborted!");
    emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance()); // NULL JOURNEY
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the QRail::LiveboardEngine::Board instance
 * @param QRail::LiveboardEngine::Board *liveboard
 * @package Liveboard
 * @public
 * Sets the QRail::LiveboardEngine::Board instance we're currently using to the
 * given QRail::LiveboardEngine::Board *liveboard.
 */
void QRail::LiveboardEngine::Factory::setLiveboard(QRail::LiveboardEngine::Board *liveboard)
{
    // Lock using QMutexLocker due concurrent access
    QMutexLocker locker(&liveboardAccessMutex);
    m_liveboard = liveboard;
}

void LiveboardEngine::Factory::initUsedPages()
{
    m_usedPages = QList<QRail::Fragments::Page *>();
}

void LiveboardEngine::Factory::addUsedPage(Fragments::Page *page)
{
    m_usedPages.append(page);
}

void LiveboardEngine::Factory::deleteUsedPages()
{
    foreach (QRail::Fragments::Page *page, m_usedPages) {
        if(page) {
            page->deleteLater();
        }
        else {
            qCritical() << "Page pointer is invalid!";
        }
    }
    qDebug() << "Liveboard pages scheduled for deletion";
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the StationEngine::Factory instance
 * @return StationEngine::Factory *factory
 * @package Liveboard
 * @private
 * Gets the StationEngine::Factory instance and returns it.
 */
StationEngine::Factory *QRail::LiveboardEngine::Factory::stationFactory() const
{
    return m_stationFactory;
}

bool QRail::LiveboardEngine::Factory::isExtending() const
{
    QMutexLocker locker(&liveboardExtendingMutex);
    return m_isExtending;
}

void QRail::LiveboardEngine::Factory::setIsExtending(bool isExtending)
{
    QMutexLocker locker(&liveboardExtendingMutex);
    m_isExtending = isExtending;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the StationEngine::Factory instance
 * @param StationEngine::Factory *factory
 * @package Liveboard
 * @private
 * Sets the StationEngine::Factory instance to the given StationEngine::Factory
 * *stationfactory.
 */
void QRail::LiveboardEngine::Factory::setStationFactory(StationEngine::Factory *stationFactory)
{
    m_stationFactory = stationFactory;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the QRail::LiveboardEngine::Board::Mode mode
 * @return const QRail::LiveboardEngine::Board::Mode mode
 * @package Liveboard
 * @public
 * Gets the QRail::LiveboardEngine::Board::Mode mode and returns it.
 */
QRail::LiveboardEngine::Board::Mode QRail::LiveboardEngine::Factory::mode() const
{
    return m_mode;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the mode
 * @param const QRail::LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @public
 * Sets the mode of the liveboard to the given
 * QRail::LiveboardEngine::Board::Mode &mode. Emits the modeChanged signal.
 */
void QRail::LiveboardEngine::Factory::setMode(const QRail::LiveboardEngine::Board::Mode &mode)
{
    m_mode = mode;
    emit this->modeChanged();
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the station URI
 * @return const QUrl stationURI
 * @package Liveboard
 * @public
 * Gets the station URI and returns it.
 */
QUrl QRail::LiveboardEngine::Factory::stationURI() const
{
    return m_stationURI;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the station URI
 * @param const QUrl &stationURI
 * @package Liveboard
 * @public
 * Sets the station URI to the given QUrl &stationURI.
 * Emits the stationURIChanged signal.
 */
void QRail::LiveboardEngine::Factory::setStationURI(const QUrl &stationURI)
{
    m_stationURI = stationURI;
    emit this->stationURIChanged();
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the until time
 * @return const QDateTime until
 * @package Liveboard
 * @public
 * Gets the until time and returns it.
 */
QDateTime QRail::LiveboardEngine::Factory::until() const
{
    return m_until;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the until time
 * @param const QDateTime &until
 * @package Liveboard
 * @public
 * Sets the until time to the given QDateTime &until.
 * Emits the untilChanged signal.
 */
void QRail::LiveboardEngine::Factory::setUntil(const QDateTime &until)
{
    m_until = until;
    emit this->untilChanged();
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the from time
 * @return const QDateTime from
 * @package Liveboard
 * @public
 * Gets the from time and returns it.
 */
QDateTime QRail::LiveboardEngine::Factory::from() const
{
    return m_from;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the from time
 * @param const QDateTime &from
 * @package Liveboard
 * @public
 * Sets the from time to the given QDateTime &from.
 * Emits the fromChanged signal.
 */
void QRail::LiveboardEngine::Factory::setFrom(const QDateTime &from)
{
    m_from = from;
    emit this->fromChanged();
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the QRail::Fragments::Factory instance
 * @return QRail::Fragments::Factory *factory
 * @package Liveboard
 * @private
 * Gets the QRail::Fragments::Factory instance and returns it.
 */
QRail::Fragments::Factory *QRail::LiveboardEngine::Factory::fragmentsFactory() const
{
    return m_fragmentsFactory;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the QRail::Fragments::Factory instance
 * @param QRail::Fragments::Factory *factory
 * @package Liveboard
 * @private
 * Sets the QRail::Fragments::Factory instance to the given
 * QRail::Fragments::Factory *factory.
 */
void QRail::LiveboardEngine::Factory::setFragmentsFactory(QRail::Fragments::Factory
                                                          *fragmentsFactory)
{
    m_fragmentsFactory = fragmentsFactory;
}

bool QRail::LiveboardEngine::Factory::isAbortRequested() const
{
    return m_abortRequested;
}

void QRail::LiveboardEngine::Factory::setAbortRequested(bool abortRequested)
{
    m_abortRequested = abortRequested;
}
