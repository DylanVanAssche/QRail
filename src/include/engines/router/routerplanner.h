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

#include <QtConcurrent/QtConcurrent>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QObject>
#include <QtCore/QPair>
#include <QtCore/QtGlobal>
#include <algorithm> // C++ header needed for std:sort function

#include "engines/alerts/alertsmessage.h"
#include "engines/router/routerroute.h"
#include "engines/router/routerrouteleg.h"
#include "engines/router/routerroutelegend.h"
#include "engines/router/routerstationstopprofile.h"
#include "engines/router/routertrainprofile.h"
#include "engines/router/routertransfer.h"
#include "engines/station/stationfactory.h"
#include "engines/station/stationstation.h"
#include "fragments/fragmentsfactory.h"
#include "qrail.h"

// Uncomment to enable logging of CSA parts
//#define VERBOSE_PARAMETERS // Enable logging of the routing and page
// parameters #define VERBOSE_TMIN // Enable logging of the Tmin calculation
//#define VERBOSE_T_ARRAY // Enable logging of the T array after each update
//#define VERBOSE_S_ARRAY // Enable logging of the S array after each update
//#define VERBOSE_LEGS // Enable logging of the legs extraction
//#define VERBOSE_FIRST_REACHABLE_CONNECTION // Enable logging of the first
// reachable connection options

// Constants
#define TRANSFER_EQUIVALENT_TRAVEL_TIME 240    // 240 seconds = 4 minutes
#define INTRA_STOP_FOOTPATH_TIME 300           // 300 seconds = 5 minutes
#define MAX_TRANSFER_TIME 3600                 // 3600 seconds = 1 hour
#define MILISECONDS_TO_SECONDS_MULTIPLIER 1000 // 1000 miliseconds = 1 second
#define SECONDS_TO_HOURS_MULTIPLIER 3600       // 3600 seconds = 1 hour
#define MINIMUM_PROGRESS_INCREMENT 5.0         // 5.0 = 5%

// Singleton pattern
namespace QRail {
    namespace RouterEngine {
        class QRAIL_SHARED_EXPORT Planner : public QObject {
          Q_OBJECT
        public:
          static Planner *getInstance();
          void getConnections(const QUrl &departureStation, const QUrl &arrivalStation,
                              const QDateTime &departureTime,
                              const qint16 &maxTransfers);
          void getConnections(const QGeoCoordinate &departurePosition, const QUrl &arrivalPosition,
                              const QDateTime &departureTime,
                              const qint16 &maxTransfers);
          QDateTime calculateArrivalTime(const QDateTime &departureTime);
          QDateTime departureTime() const;
          QDateTime arrivalTime() const;
          qint16 maxTransfers() const;
          QUrl departureStationURI() const;
          QUrl arrivalStationURI() const;

        protected:
          virtual void customEvent(QEvent *event);

        signals:
          void routesFound(const QList<QRail::RouterEngine::Route *> &routes);
          void error(const QString &message);
          void requested(const QUrl &pageURI);
          void processing(const QUrl &pageURI);
          void progress(const QUrl &pageURI, const qint16 &progress);

        private:
          mutable QMutex syncThreadMutex;
          QRail::Fragments::Factory *m_fragmentsFactory;
          StationEngine::Factory *m_stationFactory;
          QUrl m_departureStationURI;
          QUrl m_arrivalStationURI;
          QDateTime m_departureTime;
          QDateTime m_arrivalTime;
          qint16 m_maxTransfers;
          QList<QRail::RouterEngine::Route *> m_routes;
          QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>> m_SArray;
          QMap<QUrl, QRail::RouterEngine::TrainProfile *> m_TArray;
          QMap<QUrl, qint64> m_DArray;
          explicit Planner(QObject *parent = nullptr);
          static QRail::RouterEngine::Planner *m_instance;
          void parsePage(QRail::Fragments::Page *page);
          void processPage(QRail::Fragments::Page *page);
          StationStopProfile *
          getFirstReachableConnection(StationStopProfile *arrivalProfile);
          QRail::Fragments::Factory *fragmentsFactory() const;
          void setFragmentsFactory(QRail::Fragments::Factory *value);
          StationEngine::Factory *stationFactory() const;
          void setStationFactory(StationEngine::Factory *stationFactory);
          QList<QRail::RouterEngine::Route *> routes() const;
          void setRoutes(const QList<QRail::RouterEngine::Route *> &routes);
          QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>> SArray() const;
          void setSArray(const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *>> &SArray);
          QMap<QUrl, QRail::RouterEngine::TrainProfile *> TArray() const;
          void setTArray(const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray);
          QMap<QUrl, qint64> DArray() const;
          void setDArray(const QMap<QUrl, qint64> &DArray);
          void setDepartureTime(const QDateTime &departureTime);
          void setArrivalTime(const QDateTime &arrivalTime);
          void setMaxTransfers(const qint16 &maxTransfers);
          void setDepartureStationURI(const QUrl &departureStationURI);
          void setArrivalStationURI(const QUrl &arrivalStationURI);
        };
    } // namespace RouterEngine
} // namespace QRail

#endif // CSAPLANNER_H
