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
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QtGlobal>
#include <QtCore/QDebug>
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
#include "engines/station/stationfactory.h"
#include "engines/station/stationstation.h"
#include "fragments/fragmentsfactory.h"
#include "qrail.h"

// Uncomment to enable logging of CSA parts
//#define VERBOSE_PARAMETERS // Enable logging of the routing and page parameters
//#define VERBOSE_TMIN // Enable logging of the Tmin calculation
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

#define SEARCH_RADIUS 3.0 // 3.0 km
#define MAX_RESULTS 5 // 5 results maximum
#define WALKING_SPEED 5.0 // 5.0 km/h

// Singleton pattern
namespace QRail {
namespace RouterEngine {
class QRAIL_SHARED_EXPORT Planner : public QObject
{
    Q_OBJECT
public:
    static Planner *getInstance();
    ~Planner();
    void getConnections(const QUrl &departureStation,
                        const QUrl &arrivalStation,
                        const QDateTime &departureTime,
                        const quint16 &maxTransfers);
    void getConnections(const QGeoCoordinate &departurePosition,
                        const QGeoCoordinate &arrivalPosition,
                        const QDateTime &departureTime,
                        const quint16 &maxTransfers);
    void abortCurrentOperation();
    QDateTime calculateArrivalTime(const QDateTime &departureTime);
    QDateTime departureTime() const;
    QDateTime arrivalTime() const;
    qint16 maxTransfers() const;
    QUrl departureStationURI() const;
    QUrl arrivalStationURI() const;
    QRail::RouterEngine::Journey *journey() const;
    void setJourney(QRail::RouterEngine::Journey *journey);

protected:
    virtual void customEvent(QEvent *event);

signals:
    void finished(QRail::RouterEngine::Journey *journey);
    void stream(QRail::RouterEngine::Route *route);
    void error(const QString &message);
    void requested(const QUrl &pageURI);
    void processing(const QUrl &pageURI);

private slots:
    void unlockPlanner();

private:
    QMap<QUrl, qint16> T_EarliestArrivalTime;
    QMap<QUrl, QDateTime> S_EarliestArrivalTime;
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
    QRail::Fragments::Factory *fragmentsFactory() const;
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
