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
void QRail::LiveboardEngine::Factory::getLiveboardByStationURI(const QUrl &uri,
                                                               const QRail::LiveboardEngine::Board::Mode &mode)
{
    this->getLiveboardByStationURI(uri,
                                   QDateTime::currentDateTime().toUTC(),
                                   QDateTime::currentDateTime().addSecs(0.5 * SECONDS_TO_HOURS_MULTIPLIER).toUTC(),
                                   mode);
}

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
void LiveboardEngine::Factory::abortCurrentOperation()
{
    qInfo() << "Abort registered, processing!";
    this->setAbortRequested(true);
}

// Helpers
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
        QRail::Fragments::DispatcherEvent *pageEvent = reinterpret_cast<QRail::Fragments::DispatcherEvent *>(event);
        this->processPage(pageEvent->page());
        qDebug() << "Fragment event received!";
    } else {
        event->ignore();
    }
}

void LiveboardEngine::Factory::unlockLiveboard()
{
    // Timeout timer isn't necessary anymore
    this->progressTimeoutTimer->stop();

    // Make liveboard accessible again
    this->liveboardProcessingMutex.unlock();
}

void LiveboardEngine::Factory::handleTimeout()
{
    qCritical() << "Liveboard timed out, ABORTING NOW";
    this->setAbortRequested(true);
    emit this->error("Liveboard timed out, the operation has been aborted!");
    emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance());
}

void LiveboardEngine::Factory::handleFragmentFactoryError()
{
    qCritical() << "Liveboard fragment factory error, ABORTING NOW";
    this->setAbortRequested(true);
    emit this->error("Liveboard fragment factory error, the operation has been aborted!");
    emit this->finished(QRail::LiveboardEngine::NullBoard::getInstance());
}

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
    /*foreach (QRail::Fragments::Page *page, m_usedPages) {
        if(page) {
            page->deleteLater();
        }
        else {
            qCritical() << "Page pointer is invalid!";
        }
    }
    qDebug() << "Liveboard pages scheduled for deletion";*/
}

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

void QRail::LiveboardEngine::Factory::setStationFactory(StationEngine::Factory *stationFactory)
{
    m_stationFactory = stationFactory;
}

QRail::LiveboardEngine::Board::Mode QRail::LiveboardEngine::Factory::mode() const
{
    return m_mode;
}

void QRail::LiveboardEngine::Factory::setMode(const QRail::LiveboardEngine::Board::Mode &mode)
{
    m_mode = mode;
    emit this->modeChanged();
}

QUrl QRail::LiveboardEngine::Factory::stationURI() const
{
    return m_stationURI;
}

void QRail::LiveboardEngine::Factory::setStationURI(const QUrl &stationURI)
{
    m_stationURI = stationURI;
    emit this->stationURIChanged();
}

QDateTime QRail::LiveboardEngine::Factory::until() const
{
    return m_until;
}

void QRail::LiveboardEngine::Factory::setUntil(const QDateTime &until)
{
    m_until = until;
    emit this->untilChanged();
}

QDateTime QRail::LiveboardEngine::Factory::from() const
{
    return m_from;
}

void QRail::LiveboardEngine::Factory::setFrom(const QDateTime &from)
{
    m_from = from;
    emit this->fromChanged();
}

QRail::Fragments::Factory *QRail::LiveboardEngine::Factory::fragmentsFactory() const
{
    return m_fragmentsFactory;
}

void QRail::LiveboardEngine::Factory::setFragmentsFactory(QRail::Fragments::Factory *fragmentsFactory)
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
