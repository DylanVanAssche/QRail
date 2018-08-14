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

#ifndef CSAPLANNER_H
#define CSAPLANNER_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QLocale>
#include <QtCore/QPair>
#include <QtCore/QMap>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtConcurrent/QtConcurrent>
#include <algorithm> // C++ header needed for std:sort function
#include "../fragments/fragmentsfactory.h"
#include "csastationstopprofile.h"
#include "csatrainprofile.h"
#include "csaroute.h"
#include "csarouteleg.h"
#include "csaroutelegend.h"
#include "csatransfer.h"
#include "csamessage.h"
#include "csastation.h"
#include "csastationfactory.h"

// Uncomment to enable logging of CSA parts
#define VERBOSE_PARAMETERS // Enable logging of the routing and page parameters
//#define VERBOSE_TMIN // Enable logging of the Tmin calculation
//#define VERBOSE_T_ARRAY // Enable logging of the T array after each update
//#define VERBOSE_S_ARRAY // Enable logging of the S array after each update
//#define VERBOSE_LEGS // Enable logging of the legs extraction
//#define VERBOSE_FIRST_REACHABLE_CONNECTION // Enable logging of the first reachable connection options

// Constants
#define TRANSFER_EQUIVALENT_TRAVEL_TIME 240 // 240 seconds = 4 minutes
#define INTRA_STOP_FOOTPATH_TIME 300 // 300 seconds = 5 minutes
#define MAX_TRANSFER_TIME 3600 // 3600 seconds = 1 hour
#define MILISECONDS_TO_SECONDS_MULTIPLIER 1000 // 1000 miliseconds = 1 second
#define SECONDS_TO_HOURS_MULTIPLIER 3600 // 3600 seconds = 1 hour
#define MINIMUM_PROGRESS_INCREMENT 1.0 // 1.0 = 1%

// Singleton pattern
namespace CSA {
class Planner: public QObject
{
    Q_OBJECT
public:
    static Planner *getInstance(QObject *parent = nullptr);
    void getConnections(
            const QUrl &departureStation,
            const QUrl &arrivalStation,
            const QDateTime &departureTime,
            const qint16 &maxTransfers
            );
    QDateTime calculateArrivalTime(const QDateTime &departureTime);
    QDateTime departureTime() const;
    QDateTime arrivalTime() const;
    qint16 maxTransfers() const;
    QUrl departureStationURI() const;
    QUrl arrivalStationURI() const;

signals:
    void routesFound(const QList<CSA::Route *> &routes);
    void error(const QString &message);
    void pageRequested(const QUrl &pageURI);
    void pageReceived(const QUrl &pageURI);
    void pageProgress(const QUrl &pageURI, const qint16 &progress);

private slots:
    void pageReceived(Fragments::Page *page);

private:
    mutable QMutex syncThreadMutex;
    Fragments::Factory *m_fragmentsFactory;
    CSA::StationFactory *m_stationFactory;
    QUrl m_departureStationURI;
    QUrl m_arrivalStationURI;
    QDateTime m_departureTime;
    QDateTime m_arrivalTime;
    qint16 m_maxTransfers;
    QList<CSA::Route *> m_routes;
    QMap<QUrl, QList<CSA::StationStopProfile *> > m_SArray;
    QMap<QUrl, CSA::TrainProfile *> m_TArray;
    explicit Planner(QObject *parent);
    static CSA::Planner *m_instance;
    void planPage(Fragments::Page *page);
    StationStopProfile *getFirstReachableConnection(StationStopProfile *arrivalProfile);
    Fragments::Factory *fragmentsFactory() const;
    void setFragmentsFactory(Fragments::Factory *value);
    CSA::StationFactory *stationFactory() const;
    void setStationFactory(CSA::StationFactory *stationFactory);
    QList<CSA::Route *> routes() const;
    void setRoutes(const QList<CSA::Route *> &routes);
    QMap<QUrl, QList<CSA::StationStopProfile *> > SArray() const;
    void setSArray(const QMap<QUrl, QList<CSA::StationStopProfile *> > &SArray);
    QMap<QUrl, CSA::TrainProfile *> TArray() const;
    void setTArray(const QMap<QUrl, CSA::TrainProfile *> &TArray);
    void setDepartureTime(const QDateTime &departureTime);
    void setArrivalTime(const QDateTime &arrivalTime);
    void setMaxTransfers(const qint16 &maxTransfers);
    void setDepartureStationURI(const QUrl &departureStationURI);
    void setArrivalStationURI(const QUrl &arrivalStationURI);
};
}

#endif // CSAPLANNER_H
