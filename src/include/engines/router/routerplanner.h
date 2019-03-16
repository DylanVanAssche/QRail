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
#ifndef CSAPLANNER_H
#define CSAPLANNER_H

#include <QtCore/QDateTime>
#include <QtCore/QDate>
#include <QtCore/QTime>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>
#include <QtCore/QTimer>
#include <QtPositioning/QGeoCoordinate>
#include <algorithm> // C++ header needed for std:sort function

#include "engines/alerts/alertsmessage.h"
#include "engines/router/routerroute.h"
#include "engines/router/routerrouteleg.h"
#include "engines/router/routerroutelegend.h"
#include "engines/router/routerstationstopprofile.h"
#include "engines/router/routertrainprofile.h"
#include "engines/router/routertransfer.h"
#include "engines/router/routerjourney.h"
#include "engines/router/routernulljourney.h"
#include "engines/station/stationfactory.h"
#include "engines/station/stationstation.h"
#include "fragments/fragmentsfactory.h"
#include "engines/router/routersnapshotjourney.h"
#include "qrail.h"

// Uncomment to enable logging of CSA parts
//#define VERBOSE_PARAMETERS // Enable logging of the routing and page parameters
#define VERBOSE_TMIN // Enable logging of the Tmin calculation
//#define VERBOSE_T_ARRAY // Enable logging of the T array after each update
//#define VERBOSE_S_ARRAY // Enable logging of the S array after each update
//#define VERBOSE_LEGS // Enable logging of the legs extraction
//#define VERBOSE_FIRST_REACHABLE_CONNECTION // Enable logging of the first reachable connection options

// Constants
#define TRANSFER_EQUIVALENT_TRAVEL_TIME 240    // 240 seconds = 4 minutes
#define INTRA_STOP_FOOTPATH_TIME 300           // 300 seconds = 5 minutes
#define MAX_TRANSFER_TIME 3600                 // 3600 seconds = 1 hour
#define MILISECONDS_TO_SECONDS_MULTIPLIER 1000 // 1000 miliseconds = 1 second
#define SECONDS_TO_HOURS_MULTIPLIER 3600       // 3600 seconds = 1 hour
#define MINIMUM_PROGRESS_INCREMENT 1.0         // 1.0 = 1%

#define SEARCH_RADIUS 3.0                      // 3.0 km
#define MAX_RESULTS 5                          // 5 results maximum
#define WALKING_SPEED 5.0                      // 5.0 km/h

// Singleton pattern
namespace QRail {
namespace RouterEngine {
//! A RouterEngine::Planner allows you to generate RouterEngine::Journey objects.
/*!
    \class Planner
    The factory design pattern allows you to create Journey objects in an easy way. Several modes are available to fetch your Journey.
 */
class QRAIL_SHARED_EXPORT Planner : public QObject
{
    Q_OBJECT
public:
    //! Gets a RouterEngine::Planner instance.
    /*!
        \return An instance of RouterEngine::Planner.
        \public
        Constructs a RouterEngine::Planner if none exists and returns the instance.
     */
    static Planner *getInstance();
    //! Planner object destructor
    ~Planner();
    //! Retrieves a Journey between 2 given stops.
    /*!
        \param departureStation The URI of the departure stop.
        \param arrivalStation The URI of the arrival stop.
        \param departureTime The requested departure time.
        \public
        Searches for possible routes between the 2 stops using the CSA.<br>
        In case something goes wrong, a RouterEngine::NullJourney instance is returned.
     */
    void getConnections(const QUrl &departureStation,
                        const QUrl &arrivalStation,
                        const QDateTime &departureTime,
                        const quint16 &maxTransfers);
    //! Retrieves a Journey between 2 given stops.
    /*!
        \param departurePosition The GPS location of the departure location.
        \param arrivalPosition The GPS location of the arrival location.
        \param departureTime The requested departure time.
        \overload
        \note The closest stops are used to plan a journey.
        \public
        Searches for possible routes between the 2 stops using the CSA.<br>
        In case something goes wrong, a RouterEngine::NullJourney instance is returned.
     */
    void getConnections(const QGeoCoordinate &departurePosition,
                        const QGeoCoordinate &arrivalPosition,
                        const QDateTime &departureTime,
                        const quint16 &maxTransfers);
    //! Guess the worst case arrival time based on the departure time.
    QDateTime calculateArrivalTime(const QDateTime &departureTime);
    //! Cancels a current operation
    void abortCurrentOperation();
    //! Gets the departure time of the current Journey.
    /*!
        \return departureTime The requested departure time.
        \public
        Gets the departure time of the current Journey and returns it.
     */
    QDateTime departureTime() const;
    //! Gets the arrival time of the current Journey.
    /*!
        \return arrivalTime The requested arrival time.
        \public
        Gets the arrival time of the current Journey and returns it.
     */
    QDateTime arrivalTime() const;
    //! Gets the maximum amount of transfers for the current Journey.
    /*!
        \return station StationEngine::Station object with information about the associated station.
        \note The real amount of transfers is always lower than or equal to this one.
        \public
        Gets the maximum amount of transfers for the current Journey and returns it.
     */
    qint16 maxTransfers() const;
    //! Gets the departure station URI of the current Journey.
    /*!
        \return departureStationURI The URI of the departure station.
        \public
        Gets the departure station URI of the current Journey and returns it.
     */
    QUrl departureStationURI() const;
    //! Gets the arrival station URI of the current Journey.
    /*!
        \return arrivalStationURI The URI of the arrival station.
        \public
        Gets the arrival station URI of the current Journey and returns it.
     */
    QUrl arrivalStationURI() const;
    //! Gets the current Journey.
    /*!
        \return journey The current Journey in the Planner.
        \public
        Gets current Journey and returns it.
     */
    QRail::RouterEngine::Journey *journey() const;
    //! Sets the current Journey.
    /*!
        \param journey The current Journey in the Planner.
        \public
        Sets current Journey.
     */
    void setJourney(QRail::RouterEngine::Journey *journey);

    //! Test purposes
    QRail::Fragments::Factory *fragmentsFactory() const;

protected:
    //! Dispatcher protected method, only here as a reference.
    virtual void customEvent(QEvent *event);

signals:
    //! Emitted when the Journey calculation is finished.
    void finished(QRail::RouterEngine::Journey *journey);
    //! Emitted when a route has been found for the Journey.
    void stream(QRail::RouterEngine::Route *route);
    //! Emitted when something goes wrong.
    void error(const QString &message);
    //! Emitted when a new Fragments::Page has been requested.
    void requested(const QUrl &pageURI);
    //! Emitted when a new Fragments::Page has been received.
    void processing(const QUrl &pageURI);

private slots:
    void unlockPlanner();
    void handleTimeout();
    void handleFragmentFactoryError();

private:
    QTimer *progressTimeoutTimer;
    mutable QMutex plannerProcessingMutex;
    mutable QMutex syncThreadMutex;
    QRail::Fragments::Factory *m_fragmentsFactory;
    StationEngine::Factory *m_stationFactory;
    QRail::RouterEngine::Journey *m_journey;
    QList<QRail::Fragments::Page *> m_usedPages;
    bool m_abortRequested;
    explicit Planner(QObject *parent = nullptr);
    static QRail::RouterEngine::Planner *m_instance;
    void parsePage(QRail::Fragments::Page *page);
    void processPage(QRail::Fragments::Page *page);
    StationStopProfile *getFirstReachableConnection(StationStopProfile *arrivalProfile);
    void setFragmentsFactory(QRail::Fragments::Factory *value);
    StationEngine::Factory *stationFactory() const;
    void setStationFactory(StationEngine::Factory *stationFactory);
    void addToUsedPages(QRail::Fragments::Page *page);
    void deleteUsedPages();
    void initUsedPages();
    bool isAbortRequested() const;
    void setAbortRequested(bool abortRequested);
};
} // namespace RouterEngine
} // namespace QRail

#endif // CSAPLANNER_H
