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

    // Get StationEngine::Factory instance
    this->setStationFactory(StationEngine::Factory::getInstance());
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
                                   QDateTime::currentDateTime().addSecs(1 * SECONDS_TO_HOURS_MULTIPLIER).toUTC(),
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
    liveboardProcessingMutex.lock(); // Processing started
    this->setStationURI(uri);
    this->setMode(mode);
    this->setFrom(from);
    this->setUntil(until);
    this->fragmentsFactory()->getPage(this->until(), this);
    this->setLiveboard(new QRail::LiveboardEngine::Board(this));
    this->liveboard()->setEntries(QList<QRail::VehicleEngine::Vehicle *>());
    this->liveboard()->setFrom(this->from());
    this->liveboard()->setUntil(this->until());
    this->liveboard()->setMode(this->mode());
    this->liveboard()->setStation(this->stationFactory()->getStationByURI(this->stationURI()));
    this->initUsedPages();
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
    qDebug() << "Factory generated requested Linked Connection page:" << page <<
             "starting processing thread...";
    // Launch processing thread
    emit this->processing(page->uri());

    /*
    * Before processing our received page we check if we the first fragment
    * passed our departure time. We can do this because the departure times are
    * sorted in DESCENDING order.
    */
    bool finished = false;
    if (page->fragments().first()->departureTime() > this->from()) {
        qDebug() << "Requesting another page from QRail::Fragments::Factory";
        this->fragmentsFactory()->getPage(page->hydraPrevious(), this);
        emit this->requested(page->hydraPrevious());
    } else {
        finished = true;
    }
    QtConcurrent::run(this, &QRail::LiveboardEngine::Factory::parsePage, page, finished);
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
void QRail::LiveboardEngine::Factory::parsePage(QRail::Fragments::Page *page, const bool &finished)
{
    qreal previousProgress = 0.0;
    qreal currentProgress = 0.0; // Reports the page progress through the progress signal

    // Parse each connection fragment
    for (qint16 fragIndex = 0; fragIndex < page->fragments().size(); fragIndex++) {
        QRail::Fragments::Fragment *fragment = page->fragments().at(fragIndex);

        /*
         * REMARKS:
         *   - We only emit the progress signal when we reach a certain treshold to
         * avoid spamming the event loop.
         *   - Increment the fragIndex before calculating the progress to reach 100
         * % when fragIndex == 0.
         *   - 100.0 * is needed to get a qreal back between [0.0, 100.0].
         */
        currentProgress = 100.0 * (fragIndex + 1) / page->fragments().size();
        if (currentProgress - previousProgress >= MINIMUM_PROGRESS_INCREMENT) {
            previousProgress = currentProgress;
            emit this->progress(page->uri(), qRound(currentProgress));
        }

        // Lazy construction
        if ((this->mode() == QRail::LiveboardEngine::Board::Mode::ARRIVALS &&
                fragment->arrivalStationURI() == this->stationURI()) ||
                (this->mode() == QRail::LiveboardEngine::Board::Mode::DEPARTURES &&
                 fragment->departureStationURI() == this->stationURI())) {

            /*
            * Create stop information
            * TO DO: Add platform and canceled information.
            *        This is still unsupported by the Linked Connections graph!
            */
            QRail::VehicleEngine::Stop *entry = new QRail::VehicleEngine::Stop(
                this->liveboard()->station(),
                QString("?"), // platform
                true,         // isPlatformNormal
                fragment->departureTime() >= QDateTime::currentDateTime(),
                fragment->departureTime(), fragment->departureDelay(),
                false, // isDepartureCanceled
                fragment->arrivalTime(), fragment->arrivalDelay(),
                false, // isArrivalCanceled
                false, // isExtraStop
                QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED,
                QRail::VehicleEngine::Stop::Type::STOP
            );

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
        }
    }

    // Fetching fragment pages complete, emit the finished signal
    if (finished) {
        qDebug() << "Finished fetching liveboard pages";

        /*
         * Sort the entries on arrival or departure time (due concurrent access).
         *
         * We can use the .first() method to retrieve the stop since the liveboard
         * only adds one stop to the intermediaryStops list.
         * If the user wants to retrieve the full vehicle he/she can use the
         * VehicleEngine API to retrieve the full intermediaryStops list.
         */
        QList<QRail::VehicleEngine::Vehicle *> entries = this->liveboard()->entries();
        if (this->liveboard()->mode() == QRail::LiveboardEngine::Board::Mode::ARRIVALS) {
            // Use arrival time to sort
            std::sort(entries.begin(), entries.end(), [](const QRail::VehicleEngine::Vehicle * a,
            const QRail::VehicleEngine::Vehicle * b) -> bool {
                QDateTime timeA = a->intermediaryStops().first()->arrivalTime();
                QDateTime timeB = b->intermediaryStops().first()->arrivalTime();
                return timeA < timeB;
            });
        } else if (this->liveboard()->mode() == QRail::LiveboardEngine::Board::Mode::DEPARTURES) {
            // Use departure time to sort
            std::sort(entries.begin(), entries.end(), [](const QRail::VehicleEngine::Vehicle * a,
            const QRail::VehicleEngine::Vehicle * b) -> bool {
                QDateTime timeA = a->intermediaryStops().first()->departureTime();
                QDateTime timeB = b->intermediaryStops().first()->departureTime();
                return timeA < timeB;
            });
        }
        this->liveboard()->setEntries(entries);

        // Emit finished signal
        emit this->finished(this->liveboard());
        this->deleteUsedPages();
        liveboardProcessingMutex.unlock(); // Processing finished
    }

    // Queue page for deletion when Liveboard is ready
    this->addUsedPage(page);
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
        page->deleteLater();
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
