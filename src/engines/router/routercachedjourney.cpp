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


CachedJourney::CachedJourney(QList<Route *> routes, QMap<QUrl, qint16> T_EarliestArrivalTime,
                             QMap<QUrl, QDateTime> S_EarliestArrivalTime,
                             QMap<QUrl, QList<StationStopProfile *> > SArray,
                             QMap<QUrl, TrainProfile *> TArray,
                             QObject *parent) : QObject(parent)
{
    m_routes = routes;
    m_T_EarliestArrivalTime = T_EarliestArrivalTime;
    m_S_EarliestArrivalTime = S_EarliestArrivalTime;
    m_SArray = SArray;
    m_TArray = TArray;
}

Journey *CachedJourney::restoreCachedJourney(Journey *current)
{
    current->setRoutes(m_routes);
    current->setT_EarliestArrivalTime(m_T_EarliestArrivalTime);
    current->setS_EarliestArrivalTime(m_S_EarliestArrivalTime);
    current->setSArray(m_SArray);
    current->setTArray(m_TArray);
    return current;
}
