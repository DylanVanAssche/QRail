/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of BeRail.                                               *
 *                                                                            *
 *   BeRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   BeRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with BeRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#include "csaplanner.h"
CSA::Planner* CSA::Planner::m_instance = nullptr;

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Planner constructor
 * @param QObject *parent
 * @package CSA
 * @private
 * Constructs a Planner to plan routes using the
 * Connection Scan Algorithm (CSA).
 */
CSA::Planner::Planner(QObject *parent) : QObject(parent)
{
    // Automatically clean up when parent is destroyed
    this->setParent(parent);

    // Init CSA::Planner
    this->setFragmentsFactory(Fragments::Factory::getInstance(this));
    this->setStationFactory(CSA::StationFactory::getInstance(this));
    this->setRoutes(QList<CSA::Route *> ()); // Init variable
    connect(this->fragmentsFactory(), SIGNAL(pageReady(Fragments::Page*)), this, SLOT(pageReceived(Fragments::Page*)));
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a CSA::Planner instance
 * @param QObject *parent = nullptr
 * @return CSA::Planner *planner
 * @package CSA
 * @public
 * Constructs a CSA::Planner if none exists and returns the instance.
 */
CSA::Planner *CSA::Planner::getInstance(QObject *parent)
{
    // Singleton pattern
    if(m_instance == nullptr) {
        qDebug() << "Generating new CSA::Planner";
        m_instance = new Planner(parent);
    }
    return m_instance;
}

// Invokers
/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Retrieves the connections between 2 stations
 * @param const QUrl &departureStation
 * @param const QUrl &arrivalStation
 * @param const QDateTime &departureTime
 * @param const qint16 &maxTransfers
 * @package CSA
 * @private
 * Retrieves the connections between 2 stations with a given departure time and a maximum of transfers.
 * Emit the routesFound signal when completed, the error signal is emitted in case an error comes up.
 */
void CSA::Planner::getConnections(const QUrl &departureStation, const QUrl &arrivalStation, const QDateTime &departureTime, const qint16 &maxTransfers)
{
    /*
     * The CSA algorithm is based on the Connection Scan Algorithm paper, March 2017 by Julian Dibbelt, Thomas Pajor, Ben Strasser, Dorothea Wagner (KIT)
     * We will focus on the Profile Connection Scan Algorithm from sections 4.1 - 4.2.
     * However, we use a modified version of algorithm by Bert Marcelis to reduce the number of transfers.
     * We should keep searching for connections
     *  - while no results have been found yet
     *  - until we're at the front (hasPassedDepartureTimeLimit = true)
     *
     * We use the Qt Concurrent framework to enable automatically scaling of the threadpool
     * as mentioned by the docs:
     *      The QtConcurrent namespace provides high-level APIs that make it possible to
     *      write multi-threaded programs without using low-level threading primitives such
     *      as mutexes, read-write locks, wait conditions, or semaphores.
     *
     *      Programs written with QtConcurrent automatically adjust the number of threads used
     *      according to the number of processor cores available. This means that applications
     *      written today will continue to scale when deployed on multi-core systems in the future.
     *
     * Tutorial: http://www.bogotobogo.com/Qt/Qt5_QtConcurrent_RunFunction_QThread.php
     * Help: https://stackoverflow.com/questions/23702930/how-to-call-a-function-with-arguments-by-qtconcurrent#23711170
     * Help: https://stackoverflow.com/questions/7114421/qtconcurrentrun-emit-signal#41110098
     * Docs: https://doc.qt.io/qt-5.6/qtconcurrent.html
     */

    qDebug() << "Init CSA algorithm";
    this->setTArray(QMap<QUrl, CSA::TrainProfile *>());
    this->setSArray(QMap<QUrl, QList<CSA::StationStopProfile *> >());
    this->setDepartureStationURI(departureStation);
    this->setArrivalStationURI(arrivalStation);
    this->setDepartureTime(departureTime);
    this->setArrivalTime(this->calculateArrivalTime(this->departureTime()));
    this->setMaxTransfers(maxTransfers);
    this->fragmentsFactory()->getPage(this->arrivalTime());
}

// Processors
/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Plans a single page
 * @param Fragments::Page *page
 * @package CSA
 * @private
 * @note This method runs in a separated thread using the QtConcurrent framework.
 * Runs the Connection Scan Algorithm (CSA) on a single Fragments::Page page.
 * In case we need to fetch more pages, the Fragments::Factory is contacted
 * to retrieve those pages while processing the current page. The order is very
 * important, that's why only 1 page at the time may be processed.
 */
void CSA::Planner::planPage(Fragments::Page *page)
{  
    QMutexLocker locker(&syncThreadMutex); // Lock processing to enforce the DESCENDING order of departure times
    bool hasPassedDepartureTimeLimit = false; // Flag to check if we're passed the departureTime
    QDateTime INFINITE_TIME = QDateTime(QDateTime::currentDateTime().addYears(1)); // Fake an infinite time by adding 1 year to the current date, this is required for the Profile Scan Algortihm to work
    qint16 INFINITE_TRANSFERS = 32767; // Fake an infinite number of transfers by setting it to 32767 (16 bits signed 01111111 11111111)
    qreal previousProgress = 0.0;
    qreal currentProgress = 0.0; // Reports the page progress through the pageProgress signal

#ifdef VERBOSE_PARAMETERS
    qDebug() << "Planning Linked Connections page:";
    qDebug() << "\tPage URI:" << page->uri();
    qDebug() << "\thydraPrevious:" << page->hydraPrevious();
    qDebug() << "\thydraNext:" << page->hydraNext();
    qDebug() << "Parameters:";
    qDebug() << "\tDeparture time:" << this->departureTime();
    qDebug() << "\tArrival time:" << this->arrivalTime();
    qDebug() << "\tmaxTransfers:" << this->maxTransfers();
#endif

    /*
     * Before processing our received page we check if we the first fragment passed our departure time.
     * We can do this because the departure times are sorted in DESCENDING order.
     */
    if(page->fragments().first()->departureTime() > this->departureTime()) {
        qDebug() << "Requesting another page from Fragments::Factory";
        this->fragmentsFactory()->getPage(page->hydraPrevious());
        emit this->pageRequested(page->hydraPrevious());
    }

    // Run the CSA Profile Scan Algorithm on the given page, looping in DESCENDING departure times order
    for(qint16 fragIndex = page->fragments().size() - 1; fragIndex >= 0; fragIndex--) {
        Fragments::Fragment *fragment = page->fragments().at(fragIndex);

        /*
         * REMARKS:
         *   - We only emit the pageProgress signal when we reach a certain treshold to avoid spamming the event loop.
         *   - Substract the progress from 100.0 (100 %) since we are looping in the opposite direction.
         *   - Increment the fragIndex before calculating the progress to reach 100 % when fragIndex == 0.
         *   - Casting to qreal is needed to get a percentage back.
         */
        currentProgress = 100.0 - 100.0*(fragIndex+1)/page->fragments().size();
        if(currentProgress - previousProgress >= MINIMUM_PROGRESS_INCREMENT) {
            previousProgress = currentProgress;
            emit this->pageProgress(page->uri(), qRound(currentProgress));
        }

        // We can only process fragments which are departing after our departure time
        if(fragment->departureTime() < this->departureTime()) {
            hasPassedDepartureTimeLimit = true;
            continue;
        }

        /*
         * ====================================================
         *         GET EARLIEST ARRIVAL TIME T1, T2, T3
         * ====================================================
         */

        // Init variables for the algorithm, see the paper for the explanation behind T1, T2, T3 and Tmin (Tmin = Tc in the paper).
        QDateTime T1_walkingArrivalTime, T2_stayOnTripArrivalTime, T3_transferArrivalTime, Tmin_earliestArrivalTime;
        qint16 T1_transfers, T2_transfers, T3_transfers, Tmin_transfers;
        Fragments::Fragment *newExitTrainFragment; // Save the connection when we exit the train for a transfer

        // Calculate T1, the time when walking from the current stop to the destination
        if(fragment->arrivalStationURI() == this->arrivalStationURI()) {
            /*
             * This connection ends at our destination.
             * We can walk now out of the station, our implementation doesn't cover the footpath at the end (yet)
             */
            T1_walkingArrivalTime = fragment->arrivalTime(); // Due the footpath limitation, we arrive at our destination when we arrive with the vehicle.
            T1_transfers = 0; // Walking, no transfers between arrival and destination.
        }
        else {
            /*
             * Walking is infinite times slower than taking the train.
             */
            // Default value to avoid undefined variables. The loop will skip results like this anyway.
            T1_walkingArrivalTime = INFINITE_TIME;
            T1_transfers = INFINITE_TRANSFERS;
        }

        // Calculate T2, the earliest time to arrive at our destination when we remain seated on the train.
        if(this->TArray().contains(fragment->tripURI())) {
            /*
             * If you stay on the train you will arrive as soon as the train arrives at the destination.
             * The number of transfers stays the same (between this connection and the destination) as we remain seated.
             */
            T2_stayOnTripArrivalTime = this->TArray().value(fragment->tripURI())->arrivalTime();
            T2_transfers = this->TArray().value(fragment->tripURI())->transfers();
        }
        else {
            /*
             * If the key doesn't exist then we don't have the fastest arrival time to the destination yet.
             * This means that we haven't found a connection:
             *   - to arrive at the destination using this vehicle.
             *   - to transfer to another vehicle in another station.
             */
            // Default value to avoid undefined variables. The loop will skip results like this anyway.
            T2_stayOnTripArrivalTime = INFINITE_TIME;
            T2_transfers = INFINITE_TRANSFERS;
        }

        // Calculate T3, the time of arrival when taking the best possible transfer in this station.
        if(this->SArray().contains(fragment->arrivalStationURI())) {
            /*
             * If there are connections departing from this station,
             * get the one which departs after we arrive, but arrive as soon as possible.
             * The earliest departure is in the back of the array, keep track of it using the position integer.
             * As long as we're arriving AFTER the pair departure or have too much transfers to reach our destination,
             * move forward in the list until we find a departure which is reachable.
             * We move from the back to the front since the list is sorted by DESCENDING departure time,
             * the earliest departure times are in the back.
             *
             * INTRA_STOP_FOOTPATH_TIME: Time needed for the passenger to switch between vehicles
             */
            qint16 position = this->SArray().value(fragment->arrivalStationURI()).size() - 1;
            CSA::StationStopProfile *stopProfile = this->SArray().value(fragment->arrivalStationURI()).at(position);

            while((((stopProfile->departureTime().toMSecsSinceEpoch() - INTRA_STOP_FOOTPATH_TIME * MILISECONDS_TO_SECONDS_MULTIPLIER) < fragment->arrivalTime().toMSecsSinceEpoch())
                   || stopProfile->transfers() >= this->maxTransfers())
                  && position > 0)
            {
                position--;
                stopProfile = this->SArray().value(fragment->arrivalStationURI()).at(position);
            }

            if(((stopProfile->departureTime().toMSecsSinceEpoch() - INTRA_STOP_FOOTPATH_TIME * MILISECONDS_TO_SECONDS_MULTIPLIER) >= fragment->arrivalTime().toMSecsSinceEpoch())
                    && (stopProfile->transfers() <= this->maxTransfers()))
            {
                /*
                 * If a result appears in this list then we know the earliest arrival time when transferring here.
                 * We prefer to remain seated on a train when the arrival time is the same as when transferring.
                 * We add the TRANSFER_EQUIVALENT_TRAVEL_TIME to the arrival time for this. A journey will have 1
                 * less transfer in exchange for a slightly longer trip (INTRA_STOP_FOOTPATH_TIME + TRANSFER_EQUIVALENT_TRAVEL_TIME = 10 minutes).
                 *
                 * OPTIONAL: Adding 1 seconds to the arrival time will ensure that the route with the smallest number of legs is chosen.
                 *           This won't affect the journey extraction, we only get a route with less legs when arrival times are identical.
                 */
                T3_transferArrivalTime = stopProfile->arrivalTime().addSecs(TRANSFER_EQUIVALENT_TRAVEL_TIME);
                T3_transfers = stopProfile->transfers() + 1; // We transfer here, increment the number of transfers
            }
            else {
                /*
                 * If we can't find a reachable connection then a transfer isn't possible.
                 * Set T3_transferArrivalTime and T3_transfers to INFINITE to ignore them in the for loop.
                 */
                T3_transferArrivalTime = INFINITE_TIME;
                T3_transfers = INFINITE_TRANSFERS;
            }
        }
        else {
            /*
             * If we can't find a reachable connection then a transfer isn't possible.
             * Set T3_transferArrivalTime and T3_transfers to INFINITE to ignore them in the for loop.
             */
            T3_transferArrivalTime = INFINITE_TIME;
            T3_transfers = INFINITE_TRANSFERS;
        }

        /*
         * ==========================================================================================
         *         FIND THE EARLIEST ARRIVAL TIME Tmin BETWEEN THE 3 POSSIBILITIES T1, T2, T3
         * ==========================================================================================
         *
         * In the CSA paper (march 2017) they describe the JourneyLeg Extraction algorithm to extract
         * the journey. We will use a different approach since we also want to extract the departure
         * and arrival times for every single leg. To achieve this, we will save the arrival connection
         * for each hop instead of the arrival connvection for the whole journey.
         *
         * - T3 < T2, we prefer a transfer since it's faster then remaining seated.
         * - T3 == T2, we prefer T3. This ensures we don't go A - (B) - (C) - D - (C) - (B) - E
         *   when searching for A - E but get the prefered route A - B - E instead.
         * - We force already the least amount of transfers since T3 is already incremented with the
         *   TRANSFER_EQUIVALENT_TRAVEL_TIME time.
         */
        if(T3_transferArrivalTime <= T2_stayOnTripArrivalTime) {
            /*
             * We're transfering since the transfer will lead us to a faster trip than we remain seated.
             * We exit the train at this stop.
             */
            Tmin_earliestArrivalTime = T3_transferArrivalTime;
            Tmin_transfers = T3_transfers; // Already incremented above
            newExitTrainFragment = fragment;
        }
        else {
            /*
             * We're staying on this trip, if that's the case we should have an entry in the T array for this trip.
             * We're getting off tat the previous exit for this vehicle.
             */
            Tmin_earliestArrivalTime = T2_stayOnTripArrivalTime;
            if(T2_stayOnTripArrivalTime < INFINITE_TIME) {
                newExitTrainFragment = this->TArray().value(fragment->tripURI())->arrivalConnection();
            }
            else {
                newExitTrainFragment = nullptr;
            }
            Tmin_transfers = T2_transfers;
        }

        /*
         * When walking time is the same as or less than the earliest arrival time we prefer arriving.
         */
        if(T1_walkingArrivalTime <= Tmin_earliestArrivalTime) {
            Tmin_earliestArrivalTime = T1_walkingArrivalTime;
            newExitTrainFragment = fragment;
            Tmin_transfers = T1_transfers;
        }

        /*
         * We need to skip connections with INFINITE_TIME or when the newExitTrainFragment is NULL.
         * The reasons for this are explained above when we assigned the variables to INFINITE_TIME.
         * If no newExitTrainFragment is assigned we don't have to proceed either (updating the arrays).
         */
        if(Tmin_earliestArrivalTime == INFINITE_TIME || newExitTrainFragment == nullptr) {
            continue;
        }

#ifdef VERBOSE_TMIN
        else {
            if(fragment->tripURI().toString().contains("S11788")) {
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
         * Now that we determined the earliest arrival time we can update the T array.
         * We will set below the fatest arrival time for this vechile and the connection
         * at which we have to hop off.
         *
         * When there's a faster way for this trip, it's by getting off at this
         * connection's arrival station and transferring (or having arrived).
         *
         * It's also possible that the time is equal for certain transfers (Brussels-North/Brussels-Central/Brussels-South)
         * then we will update an existing connection, with a way to get off earlier (DESCENDING departure times).
         * This only modifies the transfer stop and nothing else in the journey.
         */

        if(this->TArray().contains(fragment->tripURI())) {
            if((Tmin_earliestArrivalTime == this->TArray().value(fragment->tripURI())->arrivalTime())
                    && (this->TArray().value(fragment->tripURI())->arrivalConnection()->arrivalStationURI() != this->arrivalStationURI())
                    && T3_transferArrivalTime == T2_stayOnTripArrivalTime
                    && this->SArray().contains(this->TArray().value(fragment->tripURI())->arrivalConnection()->arrivalStationURI())
                    && this->SArray().contains(fragment->arrivalStationURI())) {
                /*
                 * When the arrival time is the same, the number of transfer should also be the same!
                 * We prefer the exit connection with the largest transfer time for the passenger.
                 * Suppose we exit the train at our current connection (fragment), does this improve the transfer time?
                 */
                Fragments::Fragment *currentExitTrainFragment = this->TArray().value(fragment->tripURI())->arrivalConnection();

                /*
                 * Now we need the departure in the next station.
                 * We create a quadruple to koolup the first reachable connection in the S array.
                 * It's necessary to create one since we don't know where we would get on this train.
                 *
                 * First, we will calculate the current duration of the trip.
                 * Secondly, we try the new situation and compare it with the current situation.
                 * If this is an improvement, we update the T array accordingly.
                 */

                // Current situation
                CSA::StationStopProfile *currentStationStopProfile = new CSA::StationStopProfile(
                            fragment->departureTime(),
                            Tmin_earliestArrivalTime,
                            fragment,
                            currentExitTrainFragment,
                            Tmin_transfers
                            );
                CSA::StationStopProfile *currentFirstReachableProfile = this->getFirstReachableConnection(currentStationStopProfile);
                qint64 currentTransferDuration = -1;

                // Only when we found a reachable connection
                if(currentFirstReachableProfile) {
                    currentTransferDuration = currentExitTrainFragment->arrivalTime().secsTo(currentFirstReachableProfile->departureTime());
                }
                else {
                    qCritical() << "Unable to get first reachable connection!";
                    emit this->error("Unable to retrieve the first reachable connection!");
                }

                // New situation
                CSA::StationStopProfile *newStationStopProfile = new CSA::StationStopProfile(
                            fragment->departureTime(),
                            Tmin_earliestArrivalTime,
                            fragment,
                            newExitTrainFragment,
                            Tmin_transfers
                            );
                CSA::StationStopProfile *newFirstReachableProfile = this->getFirstReachableConnection(newStationStopProfile);
                qint64 newTransferDuration = -1;

                // Only when we found a reachable connection
                if(newFirstReachableProfile) {
                    newTransferDuration = newExitTrainFragment->arrivalTime().secsTo(newFirstReachableProfile->departureTime());
                }
                else {
                    qCritical() << "Unable to get first reachable connection!";
                    emit this->error("Unable to retrieve the first reachable connection!");
                }

                // Comparing: is the new situation better?
                if(currentTransferDuration > 0 && newTransferDuration > 0 && newTransferDuration > currentTransferDuration) {
                    CSA::TrainProfile *newTrainProfile = new CSA::TrainProfile(
                                Tmin_earliestArrivalTime,
                                newExitTrainFragment,
                                Tmin_transfers
                                );
                    QMap<QUrl, CSA::TrainProfile *> T = this->TArray();
                    T.insert(fragment->tripURI(), newTrainProfile); // Key is automatically updated when key already exists
                    this->setTArray(T);
                }
            }

            // We found a faster way, update the T array
            if(Tmin_earliestArrivalTime < this->TArray().value(fragment->tripURI())->arrivalTime()) {
                CSA::TrainProfile *fasterTrainProfile = new CSA::TrainProfile(
                            Tmin_earliestArrivalTime,
                            newExitTrainFragment,
                            Tmin_transfers
                            );
                QMap<QUrl, CSA::TrainProfile *> T = this->TArray();
                T.insert(fragment->tripURI(), fasterTrainProfile); // Key is automatically updated when key already exists
                this->setTArray(T);
            }
        }
        else {
            CSA::TrainProfile *nonExistingTrainProfile = new CSA::TrainProfile(
                        Tmin_earliestArrivalTime,
                        newExitTrainFragment,
                        Tmin_transfers
                        );
            QMap<QUrl, CSA::TrainProfile *> T = this->TArray();
            T.insert(fragment->tripURI(), nonExistingTrainProfile); // Key is automatically updated when key already exists
            this->setTArray(T);
        }

#ifdef VERBOSE_T_ARRAY
        qDebug() << "T-ARRAY";
        foreach(QUrl k, this->TArray().keys()) {
            qDebug() << k.toString() << ":" << this->TArray().value(k)->arrivalTime().toString("hh:mm");
        }
#endif

        /*
         * ====================================
         *         UPDATING THE S ARRAY
         * ====================================
         * Create a new StationStopProfile to update the S array.
         * The existing CSAStationStopProfiles shouldn't dominate our StationStopProfile.
         * This is automatically the case since the new departure time is always less or equal than
         * the ones already stored in the S array (departures are sorted by DESCENDING departure times).
         */

        CSA::StationStopProfile *updatedStationStopProfile = new CSA::StationStopProfile(
                    fragment->departureTime(),
                    Tmin_earliestArrivalTime,
                    fragment,
                    this->TArray().value(fragment->tripURI())->arrivalConnection(),
                    Tmin_transfers
                    );

        // Entry already exists in the S array
        if(this->SArray().contains(fragment->departureStationURI())) {
            qint16 numberOfPairs = this->SArray().value(fragment->departureStationURI()).size();
            CSA::StationStopProfile *existingStationStopProfile = this->SArray().value(fragment->departureStationURI()).at(numberOfPairs - 1);

            if(updatedStationStopProfile->arrivalTime() < existingStationStopProfile->arrivalTime()) {
                // Replace existing StationStopProfile at the back when departure times are equal
                if(updatedStationStopProfile->departureTime() == existingStationStopProfile->departureTime()) {
                    /*
                     * COMPILER ERROR: Modifying the return value of S.value(URI) isn't allowed.
                     * The return value is declared by Qt as a constant, modifying it directly violates this restriction.
                     * Copying this QList into a variable, modifying it and then replace it in the S array circumvents this
                     * issue.
                     *
                     * OUTPUT: /home/dylan/Projects/harbour-berail/src/linkedconnections/csaplanner.cpp:395: error: passing
                     *         'const QList<StationStopProfile*>' as 'this' argument of 'void QList<T>::replace(int, const T&)
                     *         [with T = StationStopProfile*]' discards qualifiers [-fpermissive] S.value(fragment->
                     *         departureStationURI()).replace(numberOfPairs - 1, updatedStationStopProfile);                                                                                   ^
                     */

                    QMap<QUrl, QList<CSA::StationStopProfile *> > S = this->SArray();
                    QList<CSA::StationStopProfile *> SProfiles = S.value(fragment->departureStationURI());
                    SProfiles.replace(numberOfPairs - 1, updatedStationStopProfile); // Replace profile when departure times are equal
                    S.insert(fragment->departureStationURI(), SProfiles);
                    this->setSArray(S);
                }
                // We're iterating over DESCENDING departure times, inserting the StationStopProfile at the back
                else {
                    QMap<QUrl, QList<CSA::StationStopProfile *> > S = this->SArray();
                    QList<CSA::StationStopProfile *> SProfiles = S.value(fragment->departureStationURI());
                    SProfiles.append(updatedStationStopProfile); // Add profile when we have different departure times
                    S.insert(fragment->departureStationURI(), SProfiles);
                    this->setSArray(S);
                }
            }
        }
        // New entry in the S array
        else {
            QMap<QUrl, QList<CSA::StationStopProfile *> > S = this->SArray();
            QList<CSA::StationStopProfile *> stationStopProfileList = QList<CSA::StationStopProfile *>();
            stationStopProfileList.append(updatedStationStopProfile);
            S.insert(fragment->departureStationURI(), stationStopProfileList);
            this->setSArray(S);
        }

#ifdef VERBOSE_S_ARRAY
        qDebug() << "S-ARRAY";
        foreach(QUrl k, this->SArray().keys()) {
            qDebug() << k.toString();
            foreach(CSA::StationStopProfile* p, this->SArray().value(k)) {
               qDebug() << "\t" << p->departureConnection()->tripURI().toString() << ":" << p->arrivalTime().toString("hh:mm") << "|" << p->departureTime().toString("hh:mm");
            }
        }
#endif
    } // END OF FOR LOOP

    /*
     * ======================================
     *         RETRIEVING THE RESULTS
     * ======================================
     * If we have found results in our Page,
     * we return it to the client by calling the routesFound signal.
     *
     * In case we haven't found any results, we load more data or
     * stop if we passed the departure time limit (see hasPassedDepartureTimeLimit flag).
     */

    // No results found, load more data if needed or return empty list
    if(hasPassedDepartureTimeLimit) {
        qDebug() << "Departure time limit passed, stop requesting new pages";

        // Results found, process them
        if(this->SArray().contains(this->departureStationURI())) {
            qDebug() << "Processing results: " << this->SArray().value(this->departureStationURI());
            foreach(CSA::StationStopProfile *profile, this->SArray().value(this->departureStationURI())) {
                QList<CSA::RouteLeg *> legs = QList<CSA::RouteLeg *>();

                // TODO: investigate the use of a do-while loop to avoid code repetition
                while(profile->arrivalConnection()->arrivalStationURI() != this->arrivalStationURI()) {
#ifdef VERBOSE_LEGS
                    qDebug() << "Extracted profile:";
                    qDebug() << profile->departureConnection()->departureStationURI().toString() << profile->departureConnection()->departureTime().toString("hh:mm");
                    qDebug() << profile->arrivalConnection()->arrivalStationURI().toString() << profile->arrivalConnection()->arrivalTime().toString("hh:mm");
#endif
                    CSA::RouteLegEnd *departureLeg = new CSA::RouteLegEnd(
                                profile->departureConnection()->uri(),
                                profile->departureConnection()->departureTime(),
                                this->stationFactory()->getStationByURI(profile->departureConnection()->departureStationURI()),
                                QString("?"),
                                true,
                                profile->departureConnection()->departureDelay(),
                                false,
                                profile->departureConnection()->departureDelayedTime() < QDateTime::currentDateTime(),
                                CSA::Vehicle::OccupancyLevel::UNSUPPORTED
                                );
                    CSA::RouteLegEnd *arrivalLeg = new CSA::RouteLegEnd(
                                profile->arrivalConnection()->uri(),
                                profile->arrivalConnection()->arrivalTime(),
                                this->stationFactory()->getStationByURI(profile->arrivalConnection()->arrivalStationURI()),
                                QString("?"),
                                true,
                                profile->arrivalConnection()->departureDelay(),
                                false,
                                profile->arrivalConnection()->arrivalDelayedTime() < QDateTime::currentDateTime(),
                                CSA::Vehicle::OccupancyLevel::UNSUPPORTED
                                );
                    // Create vehicle information
                    CSA::Vehicle *vehicle = new CSA::Vehicle(
                                profile->departureConnection()->tripURI(),
                                profile->departureConnection()->routeURI(),
                                profile->departureConnection()->direction()
                                );
                    CSA::RouteLeg *routeLeg = new CSA::RouteLeg(
                                CSA::RouteLeg::Type::TRAIN,
                                vehicle,
                                departureLeg,
                                arrivalLeg
                                );
                    legs.append(routeLeg);

                    // Search for the next reachable hop
                    profile = this->getFirstReachableConnection(profile);
                }

#ifdef VERBOSE_LEGS
                qDebug() << "Extracted profile:";
                qDebug() << profile->departureConnection()->departureStationURI() << profile->departureConnection()->departureTime().toString("hh:mm");
                qDebug() << profile->arrivalConnection()->arrivalStationURI() << profile->arrivalConnection()->arrivalTime().toString("hh:mm");
#endif
                // We need to add the arrival leg too!
                CSA::RouteLegEnd *departureLeg = new CSA::RouteLegEnd(
                            profile->departureConnection()->uri(),
                            profile->departureConnection()->departureTime(),
                            this->stationFactory()->getStationByURI(profile->departureConnection()->departureStationURI()),
                            QString("?"),
                            true,
                            profile->departureConnection()->departureDelay(),
                            false,
                            profile->departureConnection()->departureDelayedTime() < QDateTime::currentDateTime(),
                            CSA::Vehicle::OccupancyLevel::UNSUPPORTED
                            );

                CSA::RouteLegEnd *arrivalLeg = new CSA::RouteLegEnd(
                            profile->arrivalConnection()->uri(),
                            profile->arrivalConnection()->arrivalTime(),
                            this->stationFactory()->getStationByURI(profile->arrivalConnection()->arrivalStationURI()),
                            QString("?"),
                            true,
                            profile->arrivalConnection()->departureDelay(),
                            false,
                            profile->arrivalConnection()->arrivalDelayedTime() < QDateTime::currentDateTime(),
                            Vehicle::OccupancyLevel::UNSUPPORTED
                            );

                // Create vehicle information
                CSA::Vehicle *vehicle = new CSA::Vehicle(
                            profile->departureConnection()->tripURI(),
                            profile->departureConnection()->routeURI(),
                            profile->departureConnection()->direction()
                            );

                // Create route
                CSA::RouteLeg *routeLeg = new CSA::RouteLeg(
                            CSA::RouteLeg::Type::TRAIN,
                            vehicle,
                            departureLeg,
                            arrivalLeg
                            );
                legs.append(routeLeg);

                CSA::Route *route = new CSA::Route(
                            legs
                            );

                QList<CSA::Route *> routeList = this->routes();
                routeList.append(route);
                this->setRoutes(routeList);
            }

            /*
             * Sort routes by departure time
             * Get the routeList before modifying it (declared as const)
             * This way, we will certainly fire the right signals when the list has been modified!
             *
             * REMARK: C++11 support needed for lambda expressions
             */
            QList<CSA::Route *> routeList = this->routes();
            std::sort(routeList.begin(), routeList.end(), [](const CSA::Route* a, const CSA::Route* b) -> bool {
                QDateTime timeA = a->departureTime();
                QDateTime timeB = b->departureTime();
                return timeA < timeB;
            });
            this->setRoutes(routeList);
        }

        // Emit the error signal when we haven't found any routes
        if(this->routes().size() == 0) {
            emit this->error("No routes found!");
        }

        // Emit routesFound signal as soon as we parsed a full LC page without results
        emit this->routesFound(this->routes());
        this->setRoutes(QList<CSA::Route *>()); // Clear the routes list for a new calculation
    }
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the first reachable connection
 * @param CSA::StationStopProfile *arrivalProfile
 * @return CSA::StationStopProfile *profile
 * @package CSA
 * @private
 * Tries to find the first reachable connection for a given
 * CSA::StationStopProfile *arrivalProfile and returns it.
 */
CSA::StationStopProfile *CSA::Planner::getFirstReachableConnection(CSA::StationStopProfile *arrivalProfile)
{
    /*
     * Find the next hop, this is the first reachable hop.
     * Stricter defined: the hop which will get us to the destination at the same arrival time.
     *
     * REMARK: Due leg optimization, there will be a difference of 1 second between the arrival times.
     * ERROR: When no reachable connections are found we return a nullptr. This circumvents the 'index out of range' error.
     */
    QList<CSA::StationStopProfile *> options = this->SArray().value(arrivalProfile->arrivalConnection()->arrivalStationURI());
    qint16 i = options.size() - 1;

#ifdef VERBOSE_FIRST_REACHABLE_CONNECTION
    qDebug() << "Reachable connections for" << arrivalProfile->arrivalConnection()->arrivalStationURI().toString();
    foreach(CSA::StationStopProfile* profile, options) {
        qDebug() << "\t" << profile->departureConnection()->departureStationURI().toString() << profile->departureConnection()->departureTime().toString("hh:mm");
    }
#endif

    // Find the first reachable connection
    while(i > 0 && (options.at(i)->arrivalTime().toMSecsSinceEpoch() != (arrivalProfile->arrivalTime().toMSecsSinceEpoch() - TRANSFER_EQUIVALENT_TRAVEL_TIME * MILISECONDS_TO_SECONDS_MULTIPLIER)))
    {
        i--;
    }

    // Return the profile
    return options.at(i);
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Received page handler
 * @param Fragments::Page *page
 * @package CSA
 * @private
 * Handler for the generated pages from the Fragments::Factory.
 * When a page is received from the Fragments::Factory this handler
 * will launch a separate thread to process the page.
 */
void CSA::Planner::pageReceived(Fragments::Page *page)
{
    qDebug() << "Factory generated requested Linked Connection page:" << page << "starting processing thread...";
    QtConcurrent::run(this, &CSA::Planner::planPage, page);
    emit this->pageReceived(page->uri());
}

// Helpers
/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Guesses the arrival time
 * @param const QDateTime &departureTime
 * @package CSA
 * @public
 * Guesses the arrival time for a given QDateTime &departureTime.
 */
QDateTime CSA::Planner::calculateArrivalTime(const QDateTime &departureTime)
{
    // The current implementation guesses the arrival time
    // Calculating the arrival time can reduce the number of LC pages we need to fetch
    QTime time = departureTime.time();
    QDateTime arrivalTime = QDateTime(departureTime);

    if(time.hour() > 22 || time.hour() < 1) {
        arrivalTime = arrivalTime.addSecs(8 * SECONDS_TO_HOURS_MULTIPLIER);
        return arrivalTime;
    }
    else if(time.hour() > 18 || time.hour() < 4) {
        arrivalTime = arrivalTime.addSecs(6 * SECONDS_TO_HOURS_MULTIPLIER);
        return arrivalTime;
    }
    else {
        arrivalTime = arrivalTime.addSecs(5 * SECONDS_TO_HOURS_MULTIPLIER);
        return arrivalTime;
    }
}

// Getters & Setters
/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the maximum amount of transfers
 * @return const qint16 maxTransfers
 * @package CSA
 * @public
 * Gets the maximum amount of transfers and returns it.
 */
qint16 CSA::Planner::maxTransfers() const
{
    return m_maxTransfers;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the maximum amount of transfers
 * @param const qint16 &maxTransfers
 * @package CSA
 * @private
 * Sets the maximum amount of transfers to the given qint16 &maxTransfers
 */
void CSA::Planner::setMaxTransfers(const qint16 &maxTransfers)
{
    m_maxTransfers = maxTransfers;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival time
 * @return const QDateTime arrivalTime
 * @package CSA
 * @public
 * Gets the arrival time of the current routing and returns it.
 */
QDateTime CSA::Planner::arrivalTime() const
{
    return m_arrivalTime;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival time
 * @param const QDateTime &arrivalTime
 * @package CSA
 * @private
 * Sets the arrival time of the current routing to the given QDateTime &arrivalTime.
 */
void CSA::Planner::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure time
 * @return const QDateTime departureTime
 * @package CSA
 * @public
 * Gets the departure time of the current routing and returns it.
 */
QDateTime CSA::Planner::departureTime() const
{
    return m_departureTime;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure time
 * @param const QDateTime &departureTime
 * @package CSA
 * @private
 * Sets the departure time of the current routing to the given QDateTime &departureTime.
 */
void CSA::Planner::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the arrival station URI
 * @return const QUrl arrivalStationURI
 * @package CSA
 * @public
 * Gets the arrival station URI and returns it.
 */
QUrl CSA::Planner::arrivalStationURI() const
{
    return m_arrivalStationURI;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the arrival station URI
 * @param const QUrl &arrivalStationURI
 * @package CSA
 * @private
 * Sets the arrival station URI to the given QUrl &arrivalStationURI
 */
void CSA::Planner::setArrivalStationURI(const QUrl &arrivalStationURI)
{
    m_arrivalStationURI = arrivalStationURI;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the departure station URI
 * @return const QUrl departureStationURI
 * @package CSA
 * @public
 * Gets the departure station URI and returns it.
 */
QUrl CSA::Planner::departureStationURI() const
{
    return m_departureStationURI;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the departure station URI
 * @param const QUrl &departureStationURI
 * @package CSA
 * @private
 * Sets the departure station URI to the given QUrl &departureStationURI
 */
void CSA::Planner::setDepartureStationURI(const QUrl &departureStationURI)
{
    m_departureStationURI = departureStationURI;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the routes
 * @return const QList<CSA::Route *> routes
 * @package CSA
 * @public
 * Gets the routes retrieved by the Connection Scan Algorithm (CSA) and returns it.
 */
QList<CSA::Route *> CSA::Planner::routes() const
{
    return m_routes;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the routes
 * @param const QList<CSA::Route *> &routes
 * @package CSA
 * @private
 * Sets the routes retrieved by the Connection Scan Algorithm (CSA) to the given QList<CSA::Route *> &routes.
 */
void CSA::Planner::setRoutes(const QList<CSA::Route *> &routes)
{
    m_routes = routes;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the Fragments::Factory instance
 * @return Fragments::Factory *factory
 * @package CSA
 * @public
 * Gets the Fragments::Factory instance and returns it.
 */
Fragments::Factory *CSA::Planner::fragmentsFactory() const
{
    return m_fragmentsFactory;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the Fragments::Factory instance
 * @param const Fragments::Factory *factory
 * @package CSA
 * @private
 * Sets the Fragments::Factory instance to the given Fragments::Factory *factory.
 */
void CSA::Planner::setFragmentsFactory(Fragments::Factory *factory)
{
    m_fragmentsFactory = factory;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the CSA::StationFactory instance
 * @return CSA::StationFactory *factory
 * @package CSA
 * @public
 * Gets the CSA::StationFactory instance and returns it.
 */
CSA::StationFactory *CSA::Planner::stationFactory() const
{
    return m_stationFactory;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the CSA::StationFactory instance
 * @param const CSA::StationFactory *value
 * @package CSA
 * @private
 * Sets the CSA::StationFactory instance to the given CSA::StationFactory *stationFactory.
 */
void CSA::Planner::setStationFactory(CSA::StationFactory *stationFactory)
{
    m_stationFactory = stationFactory;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the T array
 * @return const QMap<QUrl, QList<CSA::StationStopProfile *> > TArray
 * @package CSA
 * @public
 * Gets the T array for the Connection Scan Algorithm (CSA) and returns it.
 */
QMap<QUrl, CSA::TrainProfile *> CSA::Planner::TArray() const
{
    return m_TArray;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets T array
 * @param const QMap<QUrl, CSA::TrainProfile *> &TArray
 * @package CSA
 * @private
 * Sets the T array for the Connection Scan Algorithm (CSA) to the given QMap<QUrl, CSA::TrainProfile *> &TArray.
 */
void CSA::Planner::setTArray(const QMap<QUrl, CSA::TrainProfile *> &TArray)
{
    m_TArray = TArray;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets S array
 * @return const QMap<QUrl, QList<CSA::StationStopProfile *> > SArray
 * @package CSA
 * @public
 * Gets the S array for the Connection Scan Algorithm (CSA) and returns it.
 */
QMap<QUrl, QList<CSA::StationStopProfile *> > CSA::Planner::SArray() const
{
    return m_SArray;
}

/**
 * @file csaplanner.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets S array
 * @param const QMap<QUrl, CSA::TrainProfile *> &SArray
 * @package CSA
 * @private
 * Sets the S array for the Connection Scan Algorithm (CSA) to the given QMap<QUrl, CSA::TrainProfile *> &SArray.
 */
void CSA::Planner::setSArray(const QMap<QUrl, QList<CSA::StationStopProfile *> > &SArray)
{
    m_SArray = SArray;
}
