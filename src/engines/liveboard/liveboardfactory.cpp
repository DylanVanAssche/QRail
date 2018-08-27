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
LiveboardEngine::Factory* LiveboardEngine::Factory::m_instance = nullptr;

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief LiveboardEngine::Factory constructor
 * @param QObject *parent = nullptr
 * @package Liveboard
 * @private
 * Constructs a LiveboardEngine::Factory to generate
 * LiveboardEngine::Board objects on the fly.
 */
LiveboardEngine::Factory::Factory(QObject *parent) : QObject(parent)
{
    // Get Fragments::Factory instance
    this->setFragmentsFactory(Fragments::Factory::getInstance());
    connect(this->fragmentsFactory(), SIGNAL(pageReady(Fragments::Page*)), this, SLOT(pageReceived(Fragments::Page*)));

    // Get StationEngine::Factory instance
    this->setStationFactory(StationEngine::Factory::getInstance());
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets a LiveboardEngine::Factory instance
 * @param QObject *parent = nullptr
 * @return LiveboardEngine::Factory *factory
 * @package Liveboard
 * @public
 * Constructs a LiveboardEngine::Factory if none exists and returns the instance.
 */
LiveboardEngine::Factory *LiveboardEngine::Factory::getInstance()
{
    // Singleton pattern
    if(m_instance == nullptr) {
        qDebug() << "Generating new LiveboardEngine::Factory";
        m_instance = new LiveboardEngine::Factory();
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
 * @param const LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @overload
 * @public
 * Retrieves a LiveboardEngine::Board for a station given by it's URI.
 * The LiveboardEngine::Board::Mode &mode parameter determines if the the
 * liveboard should contain all the arrivals, departures of the station.
 * Calling this method will retrieve a LiveboardEngine::Board for the current time until the current time + 3 hours.
 */
void LiveboardEngine::Factory::getLiveboardByStationURI(const QUrl &uri, const LiveboardEngine::Board::Mode &mode)
{
    this->getLiveboardByStationURI(
                uri,
                QDateTime::currentDateTime(),
                QDateTime::currentDateTime().addSecs(3 * SECONDS_TO_HOURS_MULTIPLIER),
                mode
                );
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Retrieves a liveboard by a station URI and a time range
 * @param const QUrl &url
 * @param const QDateTime &from
 * @param const QDateTime &until
 * @param const LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @overload
 * @public
 * Retrieves a LiveboardEngine::Board for a station given by it's URI.
 * The LiveboardEngine::Board::Mode &mode parameter determines if the the
 * liveboard should contain all the arrivals, departures of the station.
 */
void LiveboardEngine::Factory::getLiveboardByStationURI(const QUrl &uri, const QDateTime &from, const QDateTime &until, const LiveboardEngine::Board::Mode &mode)
{
    this->setStationURI(uri);
    qDebug() << "URI" << uri;
    qDebug() << "station URI variable" << this->stationURI();
    this->setMode(mode);
    this->setFrom(from);
    this->setUntil(until);
    this->fragmentsFactory()->getPage(this->until());
    this->setLiveboard(new LiveboardEngine::Board(this));
    this->liveboard()->setEntries(QList<VehicleEngine::Vehicle *>());
    this->liveboard()->setFrom(this->from());
    this->liveboard()->setUntil(this->until());
    this->liveboard()->setMode(this->mode());
    qDebug() << "Station factory is the bad girl";
    qDebug() << this->stationURI().toString();
    qDebug() << this->stationFactory();
    qDebug() << this->stationFactory()->getStationByURI(this->stationURI());
    this->liveboard()->setStation(this->stationFactory()->getStationByURI(this->stationURI()));
    qDebug() << "Invoked liveboard request";
}

// Helpers
/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Handler for the received Fragments::Page pages.
 * @param Fragments::Page *page
 * @package Liveboard
 * @public
 * Handles the incoming Fragments::Page pages from the Fragments::Factory.
 * New pages are requested if the time range isn't fulfilled in DESCENDING order of the departure time.
 */
void LiveboardEngine::Factory::pageReceived(Fragments::Page *page)
{
    qDebug() << "Factory generated requested Linked Connection page:" << page << "starting processing thread...";
    // Launch processing thread
    emit this->pageReceived(page->uri());

    /*
     * Before processing our received page we check if we the first fragment passed our departure time.
     * We can do this because the departure times are sorted in DESCENDING order.
     */
    bool finished = false;
    if(page->fragments().first()->departureTime() > this->from()) {
        qDebug() << "Requesting another page from Fragments::Factory";
        this->fragmentsFactory()->getPage(page->hydraPrevious());
        emit this->pageRequested(page->hydraPrevious());
    }
    else {
        finished = true;
    }
    QtConcurrent::run(this, &LiveboardEngine::Factory::parsePage, page, finished);
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Parser thread for the received pages
 * @param Fragments::Page *page
 * @param const bool &finished
 * @package Liveboard
 * @note If you want to retrieve the full intermediary stops of each vehicle in the LiveboardEngine::Board you can use the VehicleEngine::Factory.
 * @public
 * Parses the incoming pages in a separate thread using the QtConcurrent framework.
 * If the finished parameter is set to true, the LiveboardEngine::Board is ready and
 * the liveboardReady signal is emitted.
 */
void LiveboardEngine::Factory::parsePage(Fragments::Page *page, const bool &finished)
{
    // Parse each connection fragment
    foreach(Fragments::Fragment *fragment, page->fragments()) {
        // Lazy construction
        if((this->mode() == LiveboardEngine::Board::Mode::ARRIVALS && fragment->arrivalStationURI() == this->stationURI())
                || (this->mode() == LiveboardEngine::Board::Mode::DEPARTURES && fragment->departureStationURI() == this->stationURI())) {

            /*
             * Create stop information
             * TO DO: Add platform and canceled information.
             *        This is still unsupported by the Linked Connections graph!
             */
            VehicleEngine::Stop *entry = new VehicleEngine::Stop(
                        this->liveboard()->station(),
                        QString("?"), // platform
                        true, // isPlatformNormal
                        fragment->departureTime() >= QDateTime::currentDateTime(),
                        fragment->departureTime(),
                        fragment->departureDelay(),
                        false, // isDepartureCanceled
                        fragment->arrivalTime(),
                        fragment->arrivalDelay(),
                        false, // isArrivalCanceled
                        false, // isExtraStop
                        VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED,
                        VehicleEngine::Stop::Type::STOP,
                        nullptr // parent TO DO
                        );

            // Get vehicle information
            QList<VehicleEngine::Stop *> intermediaryStops = QList<VehicleEngine::Stop *>();
            intermediaryStops.append(entry);
            VehicleEngine::Vehicle *vehicle = new VehicleEngine::Vehicle(
                        fragment->routeURI(),
                        fragment->tripURI(),
                        fragment->direction(),
                        intermediaryStops,
                        nullptr // parent TO DO
                        );

            // Liveboard arrivals mode
            this->liveboard()->addEntry(vehicle);
        }
    }

    // Fetching fragment pages complete, emit the liveboardReady signal
    if(finished) {
        qDebug() << "Finished fetching liveboard pages";

        /*
         * Sort the entries on arrival or departure time (due concurrent access).
         *
         * We can use the .first() method to retrieve the stop since the liveboard
         * only adds one stop to the intermediaryStops list.
         * If the user wants to retrieve the full vehicle he/she can use the
         * VehicleEngine API to retrieve the full intermediaryStops list.
         */
        QList<VehicleEngine::Vehicle *> entries = this->liveboard()->entries();
        if(this->liveboard()->mode() == LiveboardEngine::Board::Mode::ARRIVALS) {
            std::sort(entries.begin(), entries.end(), [](const VehicleEngine::Vehicle* a, const VehicleEngine::Vehicle* b) -> bool {
                QDateTime timeA = a->intermediaryStops().first()->arrivalTime();
                QDateTime timeB = b->intermediaryStops().first()->arrivalTime();
                return timeA < timeB;
            });
        }
        else if(this->liveboard()->mode() == LiveboardEngine::Board::Mode::DEPARTURES) {
            std::sort(entries.begin(), entries.end(), [](const VehicleEngine::Vehicle* a, const VehicleEngine::Vehicle* b) -> bool {
                QDateTime timeA = a->intermediaryStops().first()->departureTime();
                QDateTime timeB = b->intermediaryStops().first()->departureTime();
                return timeA < timeB;
            });
        }
        this->liveboard()->setEntries(entries);
        emit this->liveboardReady(this->liveboard());
    }
}

// Getters & Setters
/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the LiveboardEngine::Board instance
 * @return LiveboardEngine::Board *liveboard
 * @package Liveboard
 * @public
 * Gets the LiveboardEngine::Board instance we're currently using and returns it.
 */
LiveboardEngine::Board *LiveboardEngine::Factory::liveboard() const
{
    // Lock using QMutexLocker due concurrent access
    QMutexLocker locker(&liveboardAccessMutex);
    return m_liveboard;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the LiveboardEngine::Board instance
 * @param LiveboardEngine::Board *liveboard
 * @package Liveboard
 * @public
 * Sets the LiveboardEngine::Board instance we're currently using to the given LiveboardEngine::Board *liveboard.
 */
void LiveboardEngine::Factory::setLiveboard(LiveboardEngine::Board *liveboard)
{
    // Lock using QMutexLocker due concurrent access
    QMutexLocker locker(&liveboardAccessMutex);
    m_liveboard = liveboard;
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
StationEngine::Factory *LiveboardEngine::Factory::stationFactory() const
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
 * Sets the StationEngine::Factory instance to the given StationEngine::Factory *stationfactory.
 */
void LiveboardEngine::Factory::setStationFactory(StationEngine::Factory *stationFactory)
{
    m_stationFactory = stationFactory;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the LiveboardEngine::Board::Mode mode
 * @return const LiveboardEngine::Board::Mode mode
 * @package Liveboard
 * @public
 * Gets the LiveboardEngine::Board::Mode mode and returns it.
 */
LiveboardEngine::Board::Mode LiveboardEngine::Factory::mode() const
{
    return m_mode;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the mode
 * @param const LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @public
 * Sets the mode of the liveboard to the given LiveboardEngine::Board::Mode &mode.
 * Emits the modeChanged signal.
 */
void LiveboardEngine::Factory::setMode(const LiveboardEngine::Board::Mode &mode)
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
QUrl LiveboardEngine::Factory::stationURI() const
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
void LiveboardEngine::Factory::setStationURI(const QUrl &stationURI)
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
QDateTime LiveboardEngine::Factory::until() const
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
void LiveboardEngine::Factory::setUntil(const QDateTime &until)
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
QDateTime LiveboardEngine::Factory::from() const
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
void LiveboardEngine::Factory::setFrom(const QDateTime &from)
{
    m_from = from;
    emit this->fromChanged();
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the Fragments::Factory instance
 * @return Fragments::Factory *factory
 * @package Liveboard
 * @private
 * Gets the Fragments::Factory instance and returns it.
 */
Fragments::Factory *LiveboardEngine::Factory::fragmentsFactory() const
{
    return m_fragmentsFactory;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the Fragments::Factory instance
 * @param Fragments::Factory *factory
 * @package Liveboard
 * @private
 * Sets the Fragments::Factory instance to the given Fragments::Factory *factory.
 */
void LiveboardEngine::Factory::setFragmentsFactory(Fragments::Factory *fragmentsFactory)
{
    m_fragmentsFactory = fragmentsFactory;
}
