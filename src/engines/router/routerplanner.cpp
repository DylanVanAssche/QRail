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
#include "engines/router/routerplanner.h"
using namespace QRail;
QRail::RouterEngine::Planner *QRail::RouterEngine::Planner::m_instance = nullptr;

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Planner constructor
 * @param QObject *parent
 * @package RouterEngine
 * @private
 * Constructs a Planner to plan routes using the
 * Connection Scan Algorithm (CSA).
 */
QRail::RouterEngine::Planner::Planner(QObject *parent) : QObject(parent)
{
    // Init QRail::RouterEngine::Planner
    this->setFragmentsFactory(QRail::Fragments::Factory::getInstance());
    this->setStationFactory(StationEngine::Factory::getInstance());

    // Connect signals
    connect(this, SIGNAL(finished(QRail::RouterEngine::Journey*)), this, SLOT(unlockPlanner()));
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a QRail::RouterEngine::Planner instance
 * @param QObject *parent = nullptr
 * @return QRail::RouterEngine::Planner *planner
 * @package RouterEngine
 * @public
 * Constructs a QRail::RouterEngine::Planner if none exists and returns the
 * instance.
 */
QRail::RouterEngine::Planner *QRail::RouterEngine::Planner::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new QRail::RouterEngine::Planner";
        m_instance = new Planner();
    }
    return m_instance;
}

RouterEngine::Planner::~Planner()
{
    this->deleteUsedPages();
}

// Invokers
/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Retrieves the connections between 2 stations
 * @param const QUrl &departureStation
 * @param const QUrl &arrivalStation
 * @param const QDateTime &departureTime
 * @param const qint16 &maxTransfers
 * @package RouterEngine
 * @private
 * Retrieves the connections between 2 stations with a given departure time and
 * a maximum of transfers. Emit the finished signal when completed, the error
 * signal is emitted in case an error comes up.
 *
 * Invalid input will directly emit the finished signal with an empty list.
 */
void QRail::RouterEngine::Planner::getConnections(const QUrl &departureStation,
                                                  const QUrl &arrivalStation,
                                                  const QDateTime &departureTime,
                                                  const quint16 &maxTransfers)
{
    /*
    * The CSA algorithm is based on the Connection Scan Algorithm paper, March
    * 2017 by Julian Dibbelt, Thomas Pajor, Ben Strasser, Dorothea Wagner (KIT)
    * We will focus on the Profile Connection Scan Algorithm from sections 4.1
    * - 4.2. However, we use a modified version of algorithm by Bert Marcelis to
    * reduce the number of transfers. We should keep searching for connections
    *  - while no results have been found yet
    *  - until we're at the front (hasPassedDepartureTimeLimit = true)
    *
    * OBSOLETE:
    * We use the Qt Concurrent framework to enable automatically scaling of the
    * threadpool as mentioned by the docs: The QtConcurrent namespace provides
    * high-level APIs that make it possible to write multi-threaded programs
    * without using low-level threading primitives such as mutexes,
    * read-write locks, wait conditions, or semaphores.
    *
    * Programs written with QtConcurrent automatically adjust the number of
    * threads used according to the number of processor cores available. This
    * means that applications written today will continue to scale when
    * deployed on multi-core systems in the future.
    *
    * Tutorial:
    * http://www.bogotobogo.com/Qt/Qt5_QtConcurrent_RunFunction_QThread.php Help:
    * https://stackoverflow.com/questions/23702930/how-to-call-a-function-with-arguments-by-qtconcurrent#23711170
    * Help:
    * https://stackoverflow.com/questions/7114421/qtconcurrentrun-emit-signal#41110098
    * Docs: https://doc.qt.io/qt-5.6/qtconcurrent.html
    */

    if (departureStation.isValid() && arrivalStation.isValid() && departureStation.isValid()) {
        qDebug() << "Init CSA algorithm";
        if(!plannerProcessingMutex.tryLock(LOCK_TIMEOUT)) {
            emit this->error("Planner factory is busy. Please try again later.");
            return;
        }

        // Clean up previous pages if needed
        this->deleteUsedPages();

        // Init
        this->setJourney(new QRail::RouterEngine::Journey(this));
        this->journey()->setTArray(QMap<QUrl, QRail::RouterEngine::TrainProfile *>());
        this->journey()->setSArray(QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>>());
        this->journey()->setDepartureStationURI(departureStation);
        this->journey()->setArrivalStationURI(arrivalStation);
        this->journey()->setDepartureTime(departureTime);
        this->journey()->setArrivalTime(this->calculateArrivalTime(this->journey()->departureTime()));
        this->journey()->setMaxTransfers(maxTransfers);
        this->journey()->setRoutes(QList<QRail::RouterEngine::Route *>());
        this->journey()->setT_EarliestArrivalTime(QMap<QUrl, qint16>());
        QMap<QUrl, QDateTime> S_early = QMap<QUrl, QDateTime>();
        S_early.insert(this->journey()->arrivalStationURI(), this->journey()->arrivalTime());
        this->journey()->setS_EarliestArrivalTime(S_early);
        this->initUsedPages();

        /*
         * Setup footpaths for the arrival station since CSA profile
         * goes from the end to the beginning.
         *
         * Footpaths give the user the possibility to exit at another station
         * and walk to it's destination in case that's faster than the original
         * arrival station.
         */
        QRail::StationEngine::Station *station =
                this->stationFactory()->getStationByURI(this->journey()->arrivalStationURI());

        QList<QPair<QRail::StationEngine::Station *, qreal>> nearbyStations =
                this->stationFactory()->getStationsInTheAreaByPosition(station->position(),
                                                                       SEARCH_RADIUS,
                                                                       MAX_RESULTS);

        // Jumpstart the page fetching
        this->fragmentsFactory()->getPage(this->journey()->arrivalTime(), this);
        //qApp->processEvents();
        qDebug() << "CSA init OK";
    } else {
        qCritical() << "Invalid stations or timestamps";
        qCritical() << "Departure station:" << departureStation;
        qCritical() << "Arrival station:" << arrivalStation;
        qCritical() << "Departure time:" << departureTime;
    }
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Retrieves the connections between 2 GPS positions
 * @param const QGeoCoordinate &departurePosition
 * @param const QGeoCoordinate &arrivalPosition
 * @param const QDateTime &departureTime
 * @param const qint16 &maxTransfers
 * @package RouterEngine
 * @private
 * Retrieves the connections between 2 GPS positions with a given departure time and
 * a maximum of transfers. Emit the finished signal when completed, the error
 * signal is emitted in case an error comes up.
 *
 * Invalid input will directly emit the finished signal with an empty list.
 */
void RouterEngine::Planner::getConnections(const QGeoCoordinate &departurePosition,
                                           const QGeoCoordinate &arrivalPosition,
                                           const QDateTime &departureTime,
                                           const quint16 &maxTransfers)
{
    if (departurePosition.isValid() && arrivalPosition.isValid() && departureTime.isValid()) {
        QUrl departureStationURI = this->stationFactory()->getNearestStationByPosition(departurePosition, SEARCH_RADIUS).first->uri();
        QUrl arrivalStationURI = this->stationFactory()->getNearestStationByPosition(arrivalPosition, SEARCH_RADIUS).first->uri();
        this->getConnections(departureStationURI, arrivalStationURI, departureTime, maxTransfers);
    } else {
        qCritical() << "Invalid positions or timestamps";
        qCritical() << "Departure position:" << departurePosition;
        qCritical() << "Arrival position:" << arrivalPosition;
        qCritical() << "Departure time:" << departureTime;
        emit this->finished(this->journey()); // NULL JOURNEY
    }
}

void RouterEngine::Planner::abortCurrentOperation()
{
    qInfo() << "Abort registered, processing!";
    this->setAbortRequested(true);
}

// Processors
/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Plans a single page
 * @param QRail::Fragments::Page *page
 * @package RouterEngine
 * @private
 * @note This method runs in a separated thread using the QtConcurrent
 * framework. Runs the Connection Scan Algorithm (CSA) on a single
 * QRail::Fragments::Page page. In case we need to fetch more pages, the
 * QRail::Fragments::Factory is contacted to retrieve those pages while
 * processing the current page. The order is very important, that's why only 1
 * page at the time may be processed.
 */
void QRail::RouterEngine::Planner::parsePage(QRail::Fragments::Page *page)
{
    // Lock processing to enforce the DESCENDING order of departure times
    QMutexLocker locker(&syncThreadMutex);

    // Flag to check if we're passed the departureTime
    bool hasPassedDepartureTimeLimit = false;

    // Fake an infinite time by adding 1 year to the current date, this is required for the Profile Scan Algortihm to work
    QDateTime INFINITE_TIME = QDateTime(QDateTime::currentDateTime().addYears(1));

    // Fake an infinite number of transfers by setting it to 32767 (16 bits signed 01111111 11111111)
    qint16 INFINITE_TRANSFERS = 32767;

#ifdef VERBOSE_PARAMETERS
    qDebug() << "Planning Linked Connections page:";
    qDebug() << "\tPage URI:" << page->uri();
    qDebug() << "\thydraPrevious:" << page->hydraPrevious();
    qDebug() << "\thydraNext:" << page->hydraNext();
    qDebug() << "Parameters:";
    qDebug() << "\tDeparture time:" << this->journey()->departureTime();
    qDebug() << "\tArrival time:" << this->journey()->arrivalTime();
    qDebug() << "\tmaxTransfers:" << this->journey()->maxTransfers();
#endif

    // Check if the connections are reachable in the first place using Earliest Arrival Connection Scan reverse
#ifdef VERBOSE_PARAMETERS
    qDebug() << "BEFORE number of connections:" << page->fragments().size();
#endif
    /*
     * ==================================================================
     *         EARLIEST ARRIVAL CONNECTION SCAN ALGORITHM REVERSE
     * ==================================================================
     * To implement the Earliest Arrival Connection Scan Algorithm in an easy way with the Profile Connection Scan Algorithm
     * we modified the orignal algorithm for connections in DESCENDING order. This way, we can add this 'filter' in front of the Profile CSA
     * to reduce the number of connections to scan (which reduces the processing time by roughly 15 - 21 %).
     */

    bool reachable;
    for (qint16 fragIndex = page->fragments().size() - 1; fragIndex >= 0; --fragIndex) {
        reachable = true; // We assume that everything is reachable until we prove otherwise

        // Current operation aborted by the user
        if(this->isAbortRequested()) {
            this->setAbortRequested(false);
            emit this->finished(this->journey()); // NULL JOURNEY
            qInfo() << "Aborted successfully in FOR loop";
            return;
        }

        // Cancelations are removed or the type of the connection is changed, NOT IMPLEMENTED IN UPSTREAM YET [TODO]

        /*
         * We must be able to depart from the departure station, so the pickup type must be set to GTFSTypes::REGULAR.
         * In order to arrive at our destination, we should be able to get off the vehicle at the arrival station (dropOff type == GTFSTypes::REGULAR).
         * Connections that don't arrive and departure at a stop can't be used either.
         */
        QRail::Fragments::Fragment *fragment = page->fragments().at(fragIndex);
        if((this->journey()->departureStationURI() == fragment->departureStationURI() && fragment->pickupType() != QRail::Fragments::Fragment::GTFSTypes::REGULAR)
        || (this->journey()->arrivalStationURI() == fragment->arrivalStationURI() && fragment->dropOffType() != QRail::Fragments::Fragment::GTFSTypes::REGULAR)
        || (fragment->pickupType() != QRail::Fragments::Fragment::GTFSTypes::REGULAR && fragment->dropOffType() != QRail::Fragments::Fragment::GTFSTypes::REGULAR)) {
#ifdef VERBOSE_PARAMETERS
            qDebug() << "Connection is NOT available:" << fragment->tripURI();
#endif
            reachable = false;
        }

        /*
         * Connection is available (GTFSTypes::REGULAR), we should check if it's a reachable connection using Earliest Arrival CSA.
         */
        if(reachable && (this->journey()->T_EarliestArrivalTime().contains(fragment->tripURI())
           || (this->journey()->S_EarliestArrivalTime().contains(fragment->arrivalStationURI())
               && this->journey()->S_EarliestArrivalTime().value(fragment->arrivalStationURI()) > fragment->arrivalTime())))
        {
            qint16 count = this->journey()->T_EarliestArrivalTime().value(fragment->tripURI()) + 1;
            QMap<QUrl, qint16> T_early = this->journey()->T_EarliestArrivalTime();
            T_early.insert(fragment->tripURI(), count);
            this->journey()->setT_EarliestArrivalTime(T_early);

            /*
             * We update the timestamp in the this->journey()->S_EarliestArrivalTime map for the departure station of the connection if:
             *     - The station URI doesn't exist yet.
             *     - The timestamp for the station URI is higher than our connection's departure time.
             */
            if(!this->journey()->S_EarliestArrivalTime().contains(fragment->departureStationURI())
                    || this->journey()->S_EarliestArrivalTime().value(fragment->departureStationURI()) > fragment->departureTime())
            {
                QMap<QUrl, QDateTime> S_early = this->journey()->S_EarliestArrivalTime();
                S_early.insert(fragment->departureStationURI(), fragment->departureTime());
                this->journey()->setS_EarliestArrivalTime(S_early);
            }
#ifdef VERBOSE_PARAMETERS
            qDebug() << "Connection is reachable:" << fragment->tripURI();
#endif
            reachable = true;
        }
        else {
            reachable = false;
        }

        /*
         * It's useless to process unreachable connections in the Profile Connection Scan Algorithm.
         * We remove the unreachable connections from the page before the Profile CSA is applied.
         */
        if(!reachable) {
#ifdef VERBOSE_PARAMETERS
            qDebug() << "Connection is NOT reachable:" << fragment->tripURI();
#endif
            QList<QRail::Fragments::Fragment *> frags = page->fragments();
            frags.removeAt(fragIndex);
            page->setFragments(frags);
            fragment->deleteLater();
        }
    }

#ifdef VERBOSE_PARAMETERS
    qDebug() << "AFTER number of connections:" << page->fragments().size();
#endif

    // Run the CSA Profile Scan Algorithm on the given page, looping in DESCENDING
    // departure times order
    for (qint16 fragIndex = page->fragments().size() - 1; fragIndex >= 0; --fragIndex) {
        QRail::Fragments::Fragment *fragment = page->fragments().at(fragIndex);

        // We can only process fragments which are departing after our departure time
        if (fragment->departureTime() < this->journey()->departureTime()) {
            hasPassedDepartureTimeLimit = true;
            continue;
        }

        /*
        * ====================================================
        *         GET EARLIEST ARRIVAL TIME T1, T2, T3
        * ====================================================
        */

        // Init variables for the algorithm, see the paper for the explanation
        // behind T1, T2, T3 and Tmin (Tmin = Tc in the paper).
        QDateTime T1_walkingArrivalTime, T2_stayOnTripArrivalTime, T3_transferArrivalTime, Tmin_earliestArrivalTime;
        qint16 T1_transfers, T2_transfers, T3_transfers, Tmin_transfers;
        QRail::Fragments::Fragment *newExitTrainFragment; // Save the connection when we exit the train for a transfer

        // Calculate T1, the time when walking from the current stop to the destination
        if (fragment->arrivalStationURI() == this->journey()->arrivalStationURI()) {
            /*
             * This connection ends at our destination.
             * We can walk now out of the station towards our destination.
             * The time to arrive is the arrival time in the station
             * + the walking time of the footpath to our destination.
             *
             * WARNING: The D array stores distances, we need to divide
             * them by a given WALKING_SPEED before adding them to the
             * T1_walkingArrivalTime!
             */
            T1_walkingArrivalTime = fragment->arrivalTime();
            T1_transfers = 0; // Walking, no transfers between arrival and destination.
        } else {
            /*
             * Walking is infinite times slower than taking the train.
             * Default value to avoid undefined variables.
             * The loop will skip results like this anyway.
             */
            T1_walkingArrivalTime = INFINITE_TIME;
            T1_transfers = INFINITE_TRANSFERS;
        }

        // Calculate T2, the earliest time to arrive at our destination when we
        // remain seated on the train.
        if (this->journey()->TArray().contains(fragment->tripURI())) {
            /*
            * If you stay on the train you will arrive as soon as the train arrives
            * at the destination. The number of transfers stays the same (between
            * this connection and the destination) as we remain seated.
            */
            T2_stayOnTripArrivalTime = this->journey()->TArray().value(fragment->tripURI())->arrivalTime();
            T2_transfers = this->journey()->TArray().value(fragment->tripURI())->transfers();
        } else {
            /*
            * If the key doesn't exist then we don't have the fastest arrival time to
            * the destination yet. This means that we haven't found a connection:
            *   - to arrive at the destination using this vehicle.
            *   - to transfer to another vehicle in another station.
            *
            * Default value to avoid undefined variables. The loop will skip results
            * like this anyway.
            */
            T2_stayOnTripArrivalTime = INFINITE_TIME;
            T2_transfers = INFINITE_TRANSFERS;
        }

        // Calculate T3, the time of arrival when taking the best possible transfer in this station.
        if (this->journey()->SArray().contains(fragment->arrivalStationURI())) {
            /*
            * If there are connections departing from this station,
            * get the one which departs after we arrive, but arrive as soon as
            * possible. The earliest departure is in the back of the array, keep
            * track of it using the position integer. As long as we're arriving AFTER
            * the pair departure or have too much transfers to reach our destination,
            * move forward in the list until we find a departure which is reachable.
            * We move from the back to the front since the list is sorted by
            * DESCENDING departure time, the earliest departure times are in the
            * back.
            *
            * INTRA_STOP_FOOTPATH_TIME: Time needed for the passenger to switch
            * between vehicles
            */
            qint16 position = this->journey()->SArray().value(fragment->arrivalStationURI()).size() - 1;

            QRail::RouterEngine::StationStopProfile *stopProfile = this->journey()->SArray().value(fragment->arrivalStationURI()).at(position);

            // Needs extension for footpath support
            while ((((stopProfile->departureTime().toMSecsSinceEpoch() - INTRA_STOP_FOOTPATH_TIME *
                      MILISECONDS_TO_SECONDS_MULTIPLIER) < fragment->arrivalTime().toMSecsSinceEpoch()) ||
                    stopProfile->transfers() >= this->journey()->maxTransfers()) && position > 0)
            {
                position--;
                stopProfile = this->journey()->SArray().value(fragment->arrivalStationURI()).at(position);
            }

            if (((stopProfile->departureTime().toMSecsSinceEpoch() - INTRA_STOP_FOOTPATH_TIME *
                  MILISECONDS_TO_SECONDS_MULTIPLIER) >= fragment->arrivalTime().toMSecsSinceEpoch()) &&
                    (stopProfile->transfers() <= this->journey()->maxTransfers())) {
                /*
                * If a result appears in this list then we know the earliest arrival
                * time when transferring here. We prefer to remain seated on a train
                * when the arrival time is the same as when transferring. We add the
                * TRANSFER_EQUIVALENT_TRAVEL_TIME to the arrival time for this. A
                * journey will have 1 less transfer in exchange for a slightly longer
                * trip (INTRA_STOP_FOOTPATH_TIME + TRANSFER_EQUIVALENT_TRAVEL_TIME = 10
                * minutes).
                *
                * OPTIONAL: Adding 1 seconds to the arrival time will ensure that the
                * route with the smallest number of legs is chosen. This won't affect
                * the journey extraction, we only get a route with less legs
                * when arrival times are identical.
                */
                T3_transferArrivalTime = stopProfile->arrivalTime().addSecs(TRANSFER_EQUIVALENT_TRAVEL_TIME);
                T3_transfers = stopProfile->transfers() + 1; // We transfer here, increment the number of transfers
            } else {
                /*
                * If we can't find a reachable connection then a transfer isn't
                * possible. Set T3_transferArrivalTime and T3_transfers to INFINITE to
                * ignore them in the for loop.
                */
                T3_transferArrivalTime = INFINITE_TIME;
                T3_transfers = INFINITE_TRANSFERS;
            }
        } else {
            /*
            * If we can't find a reachable connection then a transfer isn't possible.
            * Set T3_transferArrivalTime and T3_transfers to INFINITE to ignore them
            * in the for loop.
            */
            T3_transferArrivalTime = INFINITE_TIME;
            T3_transfers = INFINITE_TRANSFERS;
        }

        /*
         * ==========================================================================================
         *         FIND THE EARLIEST ARRIVAL TIME Tmin BETWEEN THE 3 POSSIBILITIES
         *         T1, T2, T3
         * ==========================================================================================
         *
         * In the CSA paper (march 2017) they describe the JourneyLeg Extraction
         * algorithm to extract the journey. We will use a different approach since
         * we also want to extract the departure and arrival times for every single
         * leg. To achieve this, we will save the arrival connection for each hop
         * instead of the arrival connvection for the whole journey.
         *
         * - T3 < T2, we prefer a transfer since it's faster then remaining seated.
         * - T3 == T2, we prefer T3. This ensures we don't go A - (B) - (C) - D -
         * (C) - (B) - E when searching for A - E but get the prefered route A - B -
         * E instead.
         * - We force already the least amount of transfers since T3 is already
         * incremented with the TRANSFER_EQUIVALENT_TRAVEL_TIME time.
         */
        if (T3_transferArrivalTime <= T2_stayOnTripArrivalTime) {
            /*
            * We're transfering since the transfer will lead us to a faster trip than
            * we remain seated. We exit the train at this stop.
            */
            Tmin_earliestArrivalTime = T3_transferArrivalTime;
            Tmin_transfers = T3_transfers; // Already incremented above
            newExitTrainFragment = fragment;
        } else {
            /*
            * We're staying on this trip, if that's the case we should have an entry
            * in the T array for this trip. We're getting off tat the previous exit
            * for this vehicle.
            */
            Tmin_earliestArrivalTime = T2_stayOnTripArrivalTime;
            if (T2_stayOnTripArrivalTime < INFINITE_TIME) {
                newExitTrainFragment = this->journey()->TArray().value(fragment->tripURI())->arrivalConnection();
            } else {
                newExitTrainFragment = nullptr;
            }
            Tmin_transfers = T2_transfers;
        }

        /*
        * When walking time is the same as or less than the earliest arrival time
        * we prefer arriving.
        */
        if (T1_walkingArrivalTime <= Tmin_earliestArrivalTime) {
            Tmin_earliestArrivalTime = T1_walkingArrivalTime;
            newExitTrainFragment = fragment;
            Tmin_transfers = T1_transfers;
        }

        /*
        * We need to skip connections with INFINITE_TIME or when the
        * newExitTrainFragment is NULL. The reasons for this are explained above
        * when we assigned the variables to INFINITE_TIME. If no
        * newExitTrainFragment is assigned we don't have to proceed either
        * (updating the arrays).
        */
        if (Tmin_earliestArrivalTime == INFINITE_TIME || newExitTrainFragment == nullptr) {
            continue;
        }

#ifdef VERBOSE_TMIN
        else {
            if (fragment->tripURI().toString().contains("S11788")) {
                qDebug() << "Tmin calculation:";
                qDebug() << "\t Fragment" << fragment->uri().toString();
                qDebug() << "\t Tmin:" << Tmin_earliestArrivalTime;
                qDebug() << "\t T1:" << T1_walkingArrivalTime;
                qDebug() << "\t T2:" << T2_stayOnTripArrivalTime;
                qDebug() << "\t T3:" << T3_transferArrivalTime;
            }
        }
#endif

        /*
        * ====================================
        *         UPDATING THE T ARRAY
        * ====================================
        *
        * Now that we determined the earliest arrival time we can update the T
        * array. We will set below the fatest arrival time for this vechile and the
        * connection at which we have to hop off.
        *
        * When there's a faster way for this trip, it's by getting off at this
        * connection's arrival station and transferring (or having arrived).
        *
        * It's also possible that the time is equal for certain transfers
        * (Brussels-North/Brussels-Central/Brussels-South) then we will update an
        * existing connection, with a way to get off earlier (DESCENDING departure
        * times). This only modifies the transfer stop and nothing else in the
        * journey.
        */
        if (this->journey()->TArray().contains(fragment->tripURI())) {
            if ((Tmin_earliestArrivalTime == this->journey()->TArray().value(fragment->tripURI())->arrivalTime()) &&
                    (this->journey()->TArray().value(fragment->tripURI())->arrivalConnection()->arrivalStationURI() !=
                     this->journey()->arrivalStationURI()) &&
                    T3_transferArrivalTime == T2_stayOnTripArrivalTime &&
                    this->journey()->SArray().contains(this->journey()->TArray().value(
                                                fragment->tripURI())->arrivalConnection()->arrivalStationURI()) &&
                    this->journey()->SArray().contains(fragment->arrivalStationURI())) {
                /*
                * When the arrival time is the same, the number of transfer should also
                * be the same! We prefer the exit connection with the largest transfer
                * time for the passenger. Suppose we exit the train at our current
                * connection (fragment), does this improve the transfer time?
                */
                QRail::Fragments::Fragment *currentExitTrainFragment =
                        this->journey()->TArray().value(fragment->tripURI())->arrivalConnection();

                /*
                * Now we need the departure in the next station.
                * We create a quadruple to koolup the first reachable connection in the
                * S array. It's necessary to create one since we don't know where we
                * would get on this train.
                *
                * First, we will calculate the current duration of the trip.
                * Secondly, we try the new situation and compare it with the current
                * situation. If this is an improvement, we update the T array
                * accordingly.
                */

                // Current situation
                QRail::RouterEngine::StationStopProfile *currentStationStopProfile =
                        new QRail::RouterEngine::StationStopProfile(
                            fragment->departureTime(),
                            Tmin_earliestArrivalTime,
                            fragment,
                            currentExitTrainFragment,
                            Tmin_transfers
                            );
                QRail::RouterEngine::StationStopProfile *currentFirstReachableProfile =
                        this->getFirstReachableConnection(currentStationStopProfile);
                qint64 currentTransferDuration = -1;

                // Only when we found a reachable connection
                if (currentFirstReachableProfile) {
                    currentTransferDuration = currentExitTrainFragment->arrivalTime().secsTo(
                                currentFirstReachableProfile->departureTime());
                } else {
                    qCritical() << "Unable to get first reachable connection!";
                    emit this->error("Unable to retrieve the first reachable connection!");
                }

                // New situation
                QRail::RouterEngine::StationStopProfile *newStationStopProfile =
                        new QRail::RouterEngine::StationStopProfile(
                            fragment->departureTime(),
                            Tmin_earliestArrivalTime,
                            fragment,
                            newExitTrainFragment,
                            Tmin_transfers
                            );
                QRail::RouterEngine::StationStopProfile *newFirstReachableProfile =
                        this->getFirstReachableConnection(newStationStopProfile);
                qint64 newTransferDuration = -1;

                // Only when we found a reachable connection
                if (newFirstReachableProfile) {
                    newTransferDuration = newExitTrainFragment->arrivalTime().secsTo(
                                newFirstReachableProfile->departureTime());
                } else {
                    qCritical() << "Unable to get first reachable connection!";
                    emit this->error("Unable to retrieve the first reachable connection!");
                }

                // Comparing: is the new situation better?
                if (currentTransferDuration > 0 && newTransferDuration > 0
                        && newTransferDuration > currentTransferDuration) {
                    QRail::RouterEngine::TrainProfile *newTrainProfile =
                            new QRail::RouterEngine::TrainProfile(
                                Tmin_earliestArrivalTime,
                                newExitTrainFragment,
                                Tmin_transfers
                                );
                    QMap<QUrl, QRail::RouterEngine::TrainProfile *> T = this->journey()->TArray();
                    T.insert(fragment->tripURI(), newTrainProfile);
                    this->journey()->setTArray(T);
                }
            }

            // We found a faster way, update the T array
            if (Tmin_earliestArrivalTime < this->journey()->TArray().value(fragment->tripURI())->arrivalTime()) {
                QRail::RouterEngine::TrainProfile *fasterTrainProfile =
                        new QRail::RouterEngine::TrainProfile(
                            Tmin_earliestArrivalTime,
                            newExitTrainFragment,
                            Tmin_transfers
                            );
                QMap<QUrl, QRail::RouterEngine::TrainProfile *> T = this->journey()->TArray();
                T.insert(fragment->tripURI(), fasterTrainProfile);
                this->journey()->setTArray(T);
            }
        } else {
            QRail::RouterEngine::TrainProfile *nonExistingTrainProfile =
                    new QRail::RouterEngine::TrainProfile(
                        Tmin_earliestArrivalTime,
                        newExitTrainFragment,
                        Tmin_transfers
                        );
            QMap<QUrl, QRail::RouterEngine::TrainProfile *> T = this->journey()->TArray();
            T.insert(fragment->tripURI(), nonExistingTrainProfile);
            this->journey()->setTArray(T);
        }

#ifdef VERBOSE_T_ARRAY
        qDebug() << "T-ARRAY";
        foreach (QUrl k, this->journey()->TArray().keys()) {
            qDebug() << k.toString() << ":"
                     << this->journey()->TArray().value(k)->arrivalTime().toString("hh:mm");
        }
#endif

        /*
        * ====================================
        *         UPDATING THE S ARRAY
        * ====================================
        * Create a new StationStopProfile to update the S array.
        * The existing StationStopProfiles shouldn't dominate our
        * StationStopProfile. This is automatically the case since the new
        * departure time is always less or equal than the ones already stored in
        * the S array (departures are sorted by DESCENDING departure times).
        */

        QRail::RouterEngine::StationStopProfile *updatedStationStopProfile =
                new QRail::RouterEngine::StationStopProfile(
                    fragment->departureTime(),
                    Tmin_earliestArrivalTime,
                    fragment,
                    this->journey()->TArray().value(fragment->tripURI())->arrivalConnection(),
                    Tmin_transfers
                    );

        // Entry already exists in the S array
        if (this->journey()->SArray().contains(fragment->departureStationURI())) {
            qint16 numberOfPairs = this->journey()->SArray().value(fragment->departureStationURI()).size();
            QRail::RouterEngine::StationStopProfile *existingStationStopProfile = this->journey()->SArray().value(
                        fragment->departureStationURI()).at(numberOfPairs - 1);
            if (updatedStationStopProfile->arrivalTime() < existingStationStopProfile->arrivalTime()) {
                // Replace existing StationStopProfile at the back when departure times are equal
                if (updatedStationStopProfile->departureTime() == existingStationStopProfile->departureTime()) {
                    /*
                    * COMPILER ERROR:
                    * Modifying the return value of S.value(URI) isn't allowed.
                    * The return value is declared by Qt as a constant,
                    * modifying it directly violates this restriction. Copying this QList
                    * into a variable, modifying it and then replace it in the S array
                    * circumvents this issue.
                    *
                    * OUTPUT:
                    * error: passing 'const QList<StationStopProfile*>' as 'this'
                    * argument of 'void QList<T>::replace(int, const T&) [with T
                    * = StationStopProfile*]' discards qualifiers [-fpermissive]
                    * S.value(fragment-> departureStationURI()).replace(numberOfPairs -
                    * 1, updatedStationStopProfile); ^
                    */

                    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>> S = this->journey()->SArray();
                    QList<QRail::RouterEngine::StationStopProfile *> SProfiles = S.value(
                                fragment->departureStationURI());
                    // Replace profile when departure times are equal
                    SProfiles.replace(numberOfPairs - 1, updatedStationStopProfile);
                    S.insert(fragment->departureStationURI(), SProfiles);
                    this->journey()->setSArray(S);
                }
                // We're iterating over DESCENDING departure times, inserting the
                // StationStopProfile at the back
                else {
                    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>> S = this->journey()->SArray();
                    QList<QRail::RouterEngine::StationStopProfile *> SProfiles = S.value(
                                fragment->departureStationURI());
                    // Add profile when we have different departure times
                    SProfiles.append(updatedStationStopProfile);
                    S.insert(fragment->departureStationURI(), SProfiles);
                    this->journey()->setSArray(S);
                }
            }
        }
        // New entry in the S array
        else {
            QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>> S = this->journey()->SArray();
            QList<QRail::RouterEngine::StationStopProfile *> stationStopProfileList =
                    QList<QRail::RouterEngine::StationStopProfile *>();

            // Add new entry if it doesn't exist yet
            stationStopProfileList.append(updatedStationStopProfile);
            S.insert(fragment->departureStationURI(), stationStopProfileList);
            this->journey()->setSArray(S);
        }

        /*
         * Inserting possible footpaths into the S array.
         * if (c_deptime, Tc) is non-dominated in profile of S[c_depstop] then:
         *      for all footpaths f with f_arrstop = c_depstop do:
         *          incorporate (c_deptime - f_dur, Tc) into profile of S[f_depstop];
         *
         * WARNING: We can't guarantee any longer that the non-domination as mentioned earlier is guaranteed!
         *          We need to run a check before actually executing the profile insertion.
         */

#ifdef VERBOSE_S_ARRAY
        qDebug() << "S-ARRAY";
        foreach (QUrl k, this->journey()->SArray().keys()) {
            qDebug() << k.toString();
            foreach (QRail::RouterEngine::StationStopProfile *p,
                     this->journey()->SArray().value(k)) {
                qDebug() << "\t" << p->departureConnection()->tripURI().toString()
                         << ":" << p->arrivalTime().toString("hh:mm") << "|"
                         << p->departureTime().toString("hh:mm");
            }
        }
#endif
    } // END OF FOR LOOP

    /*
    * ======================================
    *         RETRIEVING THE RESULTS
    * ======================================
    * If we have found results in our Page,
    * we return it to the client by calling the finished signal.
    *
    * In case we haven't found any results, we load more data or
    * stop if we passed the departure time limit (see hasPassedDepartureTimeLimit
    * flag).
    */

    // Results found, process them
    if (this->journey()->SArray().contains(this->journey()->departureStationURI())) {
        qDebug() << "Processing results: " << this->journey()->SArray().value(this->journey()->departureStationURI());
        foreach (QRail::RouterEngine::StationStopProfile *profile, this->journey()->SArray().value(this->journey()->departureStationURI())) {
            QList<QRail::RouterEngine::RouteLeg *> legs = QList<QRail::RouterEngine::RouteLeg *>();

            while (profile->arrivalConnection()->arrivalStationURI() != this->journey()->arrivalStationURI()) {
#ifdef VERBOSE_LEGS
                qDebug() << "Extracted profile:";
                qDebug() << profile->departureConnection()
                            ->departureStationURI()
                            .toString()
                         << profile->departureConnection()->departureTime().toString(
                                "hh:mm");
                qDebug()
                        << profile->arrivalConnection()->arrivalStationURI().toString()
                        << profile->arrivalConnection()->arrivalTime().toString("hh:mm");
#endif
                QRail::RouterEngine::RouteLegEnd *departureLeg =
                        new QRail::RouterEngine::RouteLegEnd(
                            profile->departureConnection()->uri(),
                            profile->departureConnection()->departureTime(),
                            this->stationFactory()->getStationByURI( profile->departureConnection()->departureStationURI()),
                            QString("?"),
                            true,
                            profile->departureConnection()->departureDelay(),
                            false,
                            profile->departureConnection()->departureTime() < QDateTime::currentDateTime(),
                            QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED);

                QRail::RouterEngine::RouteLegEnd *arrivalLeg =
                        new QRail::RouterEngine::RouteLegEnd(
                            profile->arrivalConnection()->uri(),
                            profile->arrivalConnection()->arrivalTime(),
                            this->stationFactory()->getStationByURI(profile->arrivalConnection()->arrivalStationURI()),
                            QString("?"),
                            true,
                            profile->arrivalConnection()->departureDelay(),
                            false,
                            profile->arrivalConnection()->arrivalTime() < QDateTime::currentDateTime(),
                            QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED);

                // Create vehicle information
                QRail::VehicleEngine::Vehicle *vehicle =
                        new QRail::VehicleEngine::Vehicle(
                            profile->departureConnection()->routeURI(),
                            profile->departureConnection()->tripURI(),
                            profile->departureConnection()->direction());

                QRail::RouterEngine::RouteLeg *routeLeg =
                        new QRail::RouterEngine::RouteLeg(
                            QRail::RouterEngine::RouteLeg::Type::TRAIN, vehicle,
                            departureLeg, arrivalLeg);
                legs.append(routeLeg);

                // Search for the next reachable hop
                profile = this->getFirstReachableConnection(profile);
            }

#ifdef VERBOSE_LEGS
            qDebug() << "Extracted profile:";
            qDebug() << profile->departureConnection()->departureStationURI()
                     << profile->departureConnection()->departureTime().toString("hh:mm");
            qDebug() << profile->arrivalConnection()->arrivalStationURI()
                     << profile->arrivalConnection()->arrivalTime().toString("hh:mm");
#endif
            // We need to add the arrival leg too!
            QRail::RouterEngine::RouteLegEnd *departureLeg =
                    new QRail::RouterEngine::RouteLegEnd(
                        profile->departureConnection()->uri(),
                        profile->departureConnection()->departureTime(),
                        this->stationFactory()->getStationByURI(profile->departureConnection()->departureStationURI()),
                        QString("?"),
                        true,
                        profile->departureConnection()->departureDelay(),
                        false,
                        profile->departureConnection()->departureTime() < QDateTime::currentDateTime(),
                        QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED);

            QRail::RouterEngine::RouteLegEnd *arrivalLeg =
                    new QRail::RouterEngine::RouteLegEnd(
                        profile->arrivalConnection()->uri(),
                        profile->arrivalConnection()->arrivalTime(),
                        this->stationFactory()->getStationByURI(profile->arrivalConnection()->arrivalStationURI()),
                        QString("?"),
                        true,
                        profile->arrivalConnection()->departureDelay(),
                        false,
                        profile->arrivalConnection()->arrivalTime() < QDateTime::currentDateTime(),
                        QRail::VehicleEngine::Stop::OccupancyLevel::UNSUPPORTED
                        );

            // Create vehicle information
            QRail::VehicleEngine::Vehicle *vehicle =
                    new QRail::VehicleEngine::Vehicle(
                        profile->departureConnection()->routeURI(),
                        profile->departureConnection()->tripURI(),
                        profile->departureConnection()->direction()
                        );

            // Create route
            QRail::RouterEngine::RouteLeg *routeLeg =
                    new QRail::RouterEngine::RouteLeg(
                        QRail::RouterEngine::RouteLeg::Type::TRAIN,
                        vehicle,
                        departureLeg,
                        arrivalLeg
                        );
            legs.append(routeLeg);

            QRail::RouterEngine::Route *route = new QRail::RouterEngine::Route(legs);

            // Avoid duplicates
            bool newRoute = true;
            foreach (QRail::RouterEngine::Route *r, this->journey()->routes()) {
                // Routes with the same arrival and departure times are duplicates
                if (route->departureTime() == r->departureTime()
                        && route->arrivalTime() == r->arrivalTime()) {
                    qDebug() << "Duplicate found, skipping";
                    newRoute = false;
                    break;
                }
            }

            if (newRoute) {
                emit this->stream(route);
                QList<QRail::RouterEngine::Route *> routeList = this->journey()->routes();
                routeList.append(route);
                this->journey()->setRoutes(routeList);
            }
        }

        /*
        * Sort routes by departure time
        * Get the routeList before modifying it (declared as const)
        * This way, we will certainly fire the right signals when the list has
        * been modified!
        *
        * REMARK: C++11 support needed for lambda expressions
        */
        QList<QRail::RouterEngine::Route *> routeList = this->journey()->routes();
        std::sort(routeList.begin(), routeList.end(), [](const QRail::RouterEngine::Route * a,
                  const QRail::RouterEngine::Route * b) -> bool {
            QDateTime timeA = a->departureTime();
            QDateTime timeB = b->departureTime();
            return timeA < timeB;
        });
        this->journey()->setRoutes(routeList);
    }

    /*
    * ===========================
    *         CLEANING UP
    * ===========================
    * In case we haven't found any results, we load more data or
    * stop if we passed the departure time limit (see hasPassedDepartureTimeLimit
    * flag).
    */

    // No results found, load more data if needed or return empty list
    if (hasPassedDepartureTimeLimit) {
        qDebug() << "Departure time limit passed, wrapping up";

        // Emit the error signal when we haven't found any routes
        if (this->journey()->routes().size() == 0) {
            emit this->error("No routes found!");
            emit this->finished(this->journey()); // NULL JOURNEY
        }

        // Emit finished signal when we completely parsed and processed all Linked Connections pages
        emit this->finished(this->journey());
    }
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the first reachable connection
 * @param QRail::RouterEngine::StationStopProfile *arrivalProfile
 * @return QRail::RouterEngine::StationStopProfile *profile
 * @package RouterEngine
 * @private
 * Tries to find the first reachable connection for a given
 * QRail::RouterEngine::StationStopProfile *arrivalProfile and returns it.
 */
QRail::RouterEngine::StationStopProfile *QRail::RouterEngine::Planner::getFirstReachableConnection(
        QRail::RouterEngine::StationStopProfile *arrivalProfile)
{
    /*
    * Find the next hop, this is the first reachable hop.
    * Stricter defined: the hop which will get us to the destination at the same
    * arrival time.
    *
    * REMARK: Due leg optimization, there will be a difference of 1 second
    * between the arrival times. ERROR: When no reachable connections are found
    * we return a nullptr. This circumvents the 'index out of range' error.
    */
    QList<QRail::RouterEngine::StationStopProfile *> options = this->journey()->SArray().value(
                arrivalProfile->arrivalConnection()->arrivalStationURI());
    qint16 i = options.size() - 1;

#ifdef VERBOSE_FIRST_REACHABLE_CONNECTION
    qDebug() << "Reachable connections for" <<
                arrivalProfile->arrivalConnection()->arrivalStationURI().toString();
    foreach (QRail::RouterEngine::StationStopProfile *profile, options) {
        qDebug() << "\t" << profile->departureConnection()->departureStationURI().toString() <<
                    profile->departureConnection()->departureTime().toString("hh:mm");
    }
#endif

    // Find the first reachable connection
    while (i > 0
           && (options.at(i)->arrivalTime().toMSecsSinceEpoch() !=
               (arrivalProfile->arrivalTime().toMSecsSinceEpoch() - TRANSFER_EQUIVALENT_TRAVEL_TIME *
                MILISECONDS_TO_SECONDS_MULTIPLIER))) {
        i--;
    }

    // Return the profile
    return options.at(i);
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Received page handler
 * @param QRail::Fragments::Page *page
 * @package RouterEngine
 * @private
 * Handler for the generated pages from the QRail::Fragments::Factory.
 * When a page is received from the QRail::Fragments::Factory this handler
 * will launch a separate thread to process the page.
 */
void QRail::RouterEngine::Planner::processPage(QRail::Fragments::Page *page)
{
    qDebug() << "Factory generated requested Linked Connection page:" << page <<
                "starting processing thread...";
    emit this->processing(page->uri());
    this->addToUsedPages(page);

    /*
    * Before processing our received page we check if we the first fragment
    * passed our departure time. We can do this because the departure times are
    * sorted in DESCENDING order.
    *
    * If not, fetch new pages
    *
    * /!\ In case we encounter empty pages, a CRITICAL warning is written to
    *     the console and the next page is fetched.
    */
    if (page->fragments().size() <= 0 || page->fragments().first()->departureTime() > this->journey()->departureTime()) {
        qDebug() << "Requesting another page from QRail::Fragments::Factory";
        if(page->fragments().size() <= 0) {
            qCritical() << "Page" << page->uri() << "is empty, no fragments available!";
        }

        // Aborted, no new requests should be made
        if(!this->isAbortRequested()) {
            this->fragmentsFactory()->getPage(page->hydraPrevious(), this);
            emit this->requested(page->hydraPrevious());
        }
    }

    // Launch processing thread
    this->parsePage(page);
}

// Helpers
/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Guesses the arrival time
 * @param const QDateTime &departureTime
 * @package RouterEngine
 * @public
 * Guesses the arrival time for a given QDateTime &departureTime.
 */
QDateTime QRail::RouterEngine::Planner::calculateArrivalTime(const QDateTime &departureTime)
{
    /*
     * The current implementation guesses the arrival time
     * Calculating the arrival time can reduce the number of LC pages we need to fetch
     */
    QTime time = departureTime.time();
    QDateTime arrivalTime = QDateTime(departureTime);

    if (time.hour() > 22 || time.hour() < 1) {
        arrivalTime = arrivalTime.addSecs(8 * SECONDS_TO_HOURS_MULTIPLIER);
        return arrivalTime;
    } else if (time.hour() > 18 || time.hour() < 4) {
        arrivalTime = arrivalTime.addSecs(6 * SECONDS_TO_HOURS_MULTIPLIER);
        return arrivalTime;
    } else {
        arrivalTime = arrivalTime.addSecs(5 * SECONDS_TO_HOURS_MULTIPLIER);
        return arrivalTime;
    }

    return arrivalTime;
}

// Getters & Setters
void QRail::RouterEngine::Planner::customEvent(QEvent *event)
{
    if (event->type() == this->fragmentsFactory()->dispatcher()->eventType()) {
        event->accept();
        QRail::Fragments::DispatcherEvent *pageEvent =
                reinterpret_cast<QRail::Fragments::DispatcherEvent *>(event);
        this->processPage(pageEvent->page());
    } else {
        event->ignore();
    }
}

void RouterEngine::Planner::unlockPlanner()
{
    // Make planner accessible again
    plannerProcessingMutex.unlock();
}

QRail::RouterEngine::Journey *QRail::RouterEngine::Planner::journey() const
{
    return m_journey;
}

void QRail::RouterEngine::Planner::setJourney(QRail::RouterEngine::Journey *journey)
{
    m_journey = journey;
}

bool QRail::RouterEngine::Planner::isAbortRequested() const
{
    return m_abortRequested;
}

void QRail::RouterEngine::Planner::setAbortRequested(bool abortRequested)
{
    m_abortRequested = abortRequested;
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the QRail::Fragments::Factory instance
 * @return QRail::Fragments::Factory *factory
 * @package RouterEngine
 * @public
 * Gets the QRail::Fragments::Factory instance and returns it.
 */
QRail::Fragments::Factory *QRail::RouterEngine::Planner::fragmentsFactory() const
{
    return m_fragmentsFactory;
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the QRail::Fragments::Factory instance
 * @param const QRail::Fragments::Factory *factory
 * @package RouterEngine
 * @private
 * Sets the QRail::Fragments::Factory instance to the given
 * QRail::Fragments::Factory *factory.
 */
void QRail::RouterEngine::Planner::setFragmentsFactory(QRail::Fragments::Factory *factory)
{
    m_fragmentsFactory = factory;
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the StationEngine::Factory instance
 * @return StationEngine::Factory *factory
 * @package RouterEngine
 * @public
 * Gets the StationEngine::Factory instance and returns it.
 */
StationEngine::Factory *QRail::RouterEngine::Planner::stationFactory() const
{
    return m_stationFactory;
}

/**
 * @file routerplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the StationEngine::Factory instance
 * @param const StationEngine::Factory *value
 * @package RouterEngine
 * @private
 * Sets the StationEngine::Factory instance to the given StationEngine::Factory
 * *stationFactory.
 */
void QRail::RouterEngine::Planner::setStationFactory(StationEngine::Factory *stationFactory)
{
    m_stationFactory = stationFactory;
}

void RouterEngine::Planner::addToUsedPages(Fragments::Page *page)
{
    m_usedPages.append(page);
}

void RouterEngine::Planner::deleteUsedPages()
{
    qDebug() << "Deleting previous used pages";
    foreach (QRail::Fragments::Page *page, m_usedPages) {
        delete page;
    }
}

void RouterEngine::Planner::initUsedPages()
{
    m_usedPages = QList<QRail::Fragments::Page *>();
}
