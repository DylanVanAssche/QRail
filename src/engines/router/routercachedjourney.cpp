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
#include "engines/router/routercachedjourney.h"
using namespace QRail;
using namespace RouterEngine;


CachedJourney::CachedJourney(QUrl pageURI, QList<Route *> routes, QMap<QUrl, qint16> T_EarliestArrivalTime,
                             QMap<QUrl, QDateTime> S_EarliestArrivalTime,
                             QMap<QUrl, QList<StationStopProfile *> > SArray,
                             QMap<QUrl, TrainProfile *> TArray,
                             QObject *parent) : QObject(parent)
{
    m_pageURI = pageURI;
    QUrlQuery pageQuery = QUrlQuery(m_pageURI.query());
    m_pageTimestamp = QDateTime::fromString(pageQuery["departureTime"], Qt::ISODate);
    qDebug() << "Page timestamp extracted:" << m_pageTimestamp.toString(Qt::ISODate);
    m_routes = routes;
    m_T_EarliestArrivalTime = T_EarliestArrivalTime;
    m_S_EarliestArrivalTime = S_EarliestArrivalTime;
    m_SArray = SArray;
    m_TArray = TArray;
}

QUrl CachedJourney::pageURI() const
{
    return m_pageURI;
}

QList<QRail::RouterEngine::Route *> CachedJourney::routes() const
{
    return m_routes;
}

QMap<QUrl, qint16> CachedJourney::T_EarliestArrivalTime() const
{
    return m_T_EarliestArrivalTime;
}

QMap<QUrl, QDateTime> CachedJourney::S_EarliestArrivalTime() const
{
    return m_S_EarliestArrivalTime;
}

QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > CachedJourney::SArray() const
{
    return m_SArray;
}

QMap<QUrl, QRail::RouterEngine::TrainProfile *> CachedJourney::TArray() const
{
    return m_TArray;
}

QDateTime CachedJourney::pageTimestamp() const
{
    return m_pageTimestamp;
}
