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
#ifndef ROUTERCACHEDJOURNEY_H
#define ROUTERCACHEDJOURNEY_H

#include <QtCore/QObject>
#include <QtCore/QMap>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include <QtCore/QSharedPointer>

#include "engines/router/routerroute.h"
#include "engines/router/routerstationstopprofile.h"
#include "engines/router/routertrainprofile.h"

namespace QRail {
namespace RouterEngine {
class SnapshotJourney : public QObject
{
    Q_OBJECT
public:
    //! Constructs a SnapshotJourney object with all the needed parameters.
    explicit SnapshotJourney(QUrl pageURI,
                           QUrl hydraNext,
                           QUrl hydraPrevious,
                           QList<QSharedPointer<QRail::RouterEngine::Route >> routes,
                           QMap<QUrl, qint16> T_EarliestArrivalTime,
                           QMap<QUrl, QDateTime> S_EarliestArrivalTime,
                           QMap<QUrl, QList<QSharedPointer<QRail::RouterEngine::StationStopProfile> > > SArray,
                           QMap<QUrl, QSharedPointer<QRail::RouterEngine::TrainProfile> > TArray,
                           QObject *parent = nullptr);
    QUrl pageURI() const;
    QUrl hydraNext() const;
    QUrl hydraPrevious() const;
    QList<QSharedPointer<QRail::RouterEngine::Route >> routes() const;
    QMap<QUrl, qint16> T_EarliestArrivalTime() const;
    QMap<QUrl, QDateTime> S_EarliestArrivalTime() const;
    QMap<QUrl, QList<QSharedPointer<QRail::RouterEngine::StationStopProfile> > > SArray() const;
    QMap<QUrl, QSharedPointer<QRail::RouterEngine::TrainProfile> > TArray() const;
    QDateTime pageTimestamp() const;

private:
    QUrl m_pageURI;
    QUrl m_hydraNext;
    QUrl m_hydraPrevious;
    QDateTime m_pageTimestamp;
    QList<QSharedPointer<QRail::RouterEngine::Route >> m_routes;
    QMap<QUrl, qint16> m_T_EarliestArrivalTime;
    QMap<QUrl, QDateTime> m_S_EarliestArrivalTime;
    QMap<QUrl, QList<QSharedPointer<QRail::RouterEngine::StationStopProfile> > > m_SArray;
    QMap<QUrl, QSharedPointer<QRail::RouterEngine::TrainProfile> > m_TArray;
};
}}

#endif // ROUTERCACHEDJOURNEY_H
