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

#include "liveboard/liveboardfactory.h"
Liveboard::Factory* Liveboard::Factory::m_instance = nullptr;

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Liveboard::Factory constructor
 * @param QObject *parent = nullptr
 * @package Liveboard
 * @private
 * Constructs a Liveboard::Factory to generate
 * Liveboard::Board objects on the fly.
 */
Liveboard::Factory::Factory(QObject *parent) : QObject(parent)
{
    // Get Fragments::Factory instance
    this->setFragmentsFactory(Fragments::Factory::getInstance());
    connect(this->fragmentsFactory(), SIGNAL(pageReady(Fragments::Page*)), this, SLOT(pageReceived(Fragments::Page*)));

    // Get CSA::StationFactory instance
    this->setStationFactory(CSA::StationFactory::getInstance());
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets a Liveboard::Factory instance
 * @param QObject *parent = nullptr
 * @return Liveboard::Factory *factory
 * @package Liveboard
 * @public
 * Constructs a Liveboard::Factory if none exists and returns the instance.
 */
Liveboard::Factory *Liveboard::Factory::getInstance()
{
    // Singleton pattern
    if(m_instance == nullptr) {
        qDebug() << "Generating new Liveboard::Factory";
        m_instance = new Liveboard::Factory();
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
 * @param const Liveboard::Board::Mode &mode
 * @package Liveboard
 * @overload
 * @public
 * Retrieves a Liveboard::Board for a station given by it's URI.
 * The Liveboard::Board::Mode &mode parameter determines if the the
 * liveboard should contain all the arrivals, departures of the station.
 * Calling this method will retrieve a Liveboard::Board for the current time until the current time + 3 hours.
 */
void Liveboard::Factory::getLiveboardByStationURI(const QUrl &uri, const Liveboard::Board::Mode &mode)
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
 * @param const Liveboard::Board::Mode &mode
 * @package Liveboard
 * @overload
 * @public
 * Retrieves a Liveboard::Board for a station given by it's URI.
 * The Liveboard::Board::Mode &mode parameter determines if the the
 * liveboard should contain all the arrivals, departures of the station.
 */
void Liveboard::Factory::getLiveboardByStationURI(const QUrl &uri, const QDateTime &from, const QDateTime &until, const Liveboard::Board::Mode &mode)
{
    this->setStationURI(uri);
    qDebug() << "URI" << uri;
    qDebug() << "station URI variable" << this->stationURI();
    this->setMode(mode);
    this->setFrom(from);
    this->setUntil(until);
    this->fragmentsFactory()->getPage(this->until());
    this->setLiveboard(new Liveboard::Board(this));
    this->liveboard()->setEntries(QList<CSA::VehicleStop *>());
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
void Liveboard::Factory::pageReceived(Fragments::Page *page)
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
    QtConcurrent::run(this, &Liveboard::Factory::parsePage, page, finished);
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Parser thread for the received pages
 * @param Fragments::Page *page
 * @param const bool &finished
 * @package Liveboard
 * @public
 * Parses the incoming pages in a separate thread using the QtConcurrent framework.
 * If the finished parameter is set to true, the Liveboard::Board is ready and
 * the liveboardReady signal is emitted.
 */
void Liveboard::Factory::parsePage(Fragments::Page *page, const bool &finished)
{
    // Parse each connection fragment
    foreach(Fragments::Fragment *fragment, page->fragments()) {
        // Lazy construction
        if((this->mode() == Liveboard::Board::Mode::ARRIVALS && fragment->arrivalStationURI() == this->stationURI())
                || (this->mode() == Liveboard::Board::Mode::DEPARTURES && fragment->departureStationURI() == this->stationURI())) {

            // Get vehicle information
            CSA::Vehicle *vehicle = new CSA::Vehicle(
                        fragment->tripURI(),
                        fragment->routeURI(),
                        fragment->direction(),
                        nullptr // parent TO DO
                        );

            /*
             * Create stop information
             * TO DO: Add platform and canceled information.
             *        This is still unsupported by the Linked Connections graph!
             */
            CSA::VehicleStop *entry = new CSA::VehicleStop(
                        vehicle,
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
                        CSA::Vehicle::OccupancyLevel::UNSUPPORTED,
                        CSA::VehicleStop::Type::STOP,
                        nullptr // parent TO DO
                        );

            // Liveboard arrivals mode
            this->liveboard()->addEntry(entry);
        }
    }

    // Fetching fragment pages complete, emit the liveboardReady signal
    if(finished) {
        qDebug() << "Finished fetching liveboard pages";

        // Sort the entries on arrival or departure time (due concurrent access)
        QList<CSA::VehicleStop *> entries = this->liveboard()->entries();
        if(this->liveboard()->mode() == Liveboard::Board::Mode::ARRIVALS) {
            std::sort(entries.begin(), entries.end(), [](const CSA::VehicleStop* a, const CSA::VehicleStop* b) -> bool {
                QDateTime timeA = a->arrivalTime();
                QDateTime timeB = b->arrivalTime();
                return timeA < timeB;
            });
        }
        else if(this->liveboard()->mode() == Liveboard::Board::Mode::DEPARTURES) {
            std::sort(entries.begin(), entries.end(), [](const CSA::VehicleStop* a, const CSA::VehicleStop* b) -> bool {
                QDateTime timeA = a->departureTime();
                QDateTime timeB = b->departureTime();
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
 * @brief Gets the Liveboard::Board instance
 * @return Liveboard::Board *liveboard
 * @package Liveboard
 * @public
 * Gets the Liveboard::Board instance we're currently using and returns it.
 */
Liveboard::Board *Liveboard::Factory::liveboard() const
{
    // Lock using QMutexLocker due concurrent access
    QMutexLocker locker(&liveboardAccessMutex);
    return m_liveboard;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the Liveboard::Board instance
 * @param Liveboard::Board *liveboard
 * @package Liveboard
 * @public
 * Sets the Liveboard::Board instance we're currently using to the given Liveboard::Board *liveboard.
 */
void Liveboard::Factory::setLiveboard(Liveboard::Board *liveboard)
{
    // Lock using QMutexLocker due concurrent access
    QMutexLocker locker(&liveboardAccessMutex);
    m_liveboard = liveboard;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the CSA::StationFactory instance
 * @return CSA::StationFactory *factory
 * @package Liveboard
 * @private
 * Gets the CSA::StationFactory instance and returns it.
 */
CSA::StationFactory *Liveboard::Factory::stationFactory() const
{
    return m_stationFactory;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the CSA::StationFactory instance
 * @param CSA::StationFactory *factory
 * @package Liveboard
 * @private
 * Sets the CSA::StationFactory instance to the given CSA::StationFactory *stationfactory.
 */
void Liveboard::Factory::setStationFactory(CSA::StationFactory *stationFactory)
{
    m_stationFactory = stationFactory;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the Liveboard::Board::Mode mode
 * @return const Liveboard::Board::Mode mode
 * @package Liveboard
 * @public
 * Gets the Liveboard::Board::Mode mode and returns it.
 */
Liveboard::Board::Mode Liveboard::Factory::mode() const
{
    return m_mode;
}

/**
 * @file liveboardfactory.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the mode
 * @param const Liveboard::Board::Mode &mode
 * @package Liveboard
 * @public
 * Sets the mode of the liveboard to the given Liveboard::Board::Mode &mode.
 * Emits the modeChanged signal.
 */
void Liveboard::Factory::setMode(const Liveboard::Board::Mode &mode)
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
QUrl Liveboard::Factory::stationURI() const
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
void Liveboard::Factory::setStationURI(const QUrl &stationURI)
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
QDateTime Liveboard::Factory::until() const
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
void Liveboard::Factory::setUntil(const QDateTime &until)
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
QDateTime Liveboard::Factory::from() const
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
void Liveboard::Factory::setFrom(const QDateTime &from)
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
Fragments::Factory *Liveboard::Factory::fragmentsFactory() const
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
void Liveboard::Factory::setFragmentsFactory(Fragments::Factory *fragmentsFactory)
{
    m_fragmentsFactory = fragmentsFactory;
}
