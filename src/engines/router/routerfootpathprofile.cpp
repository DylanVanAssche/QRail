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
#include "engines/router/routerfootpathprofile.h"
using namespace QRail;

RouterEngine::FootpathProfile::FootpathProfile(StationEngine::Station *arrivalStation,
                                               StationEngine::Station *departureStation,
                                               qreal distance,
                                               QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal fire on construction
    m_arrivalStation = arrivalStation;
    m_departureStation = departureStation;
    m_distance = distance;
}

// Getters & Setters
qreal RouterEngine::FootpathProfile::distance() const
{
    return m_distance;
}

void RouterEngine::FootpathProfile::setDistance(const qreal &distance)
{
    m_distance = distance;
}

QRail::StationEngine::Station *RouterEngine::FootpathProfile::departureStation() const
{
    return m_departureStation;
}

void RouterEngine::FootpathProfile::setDepartureStation(QRail::StationEngine::Station
                                                        *departureStation)
{
    m_departureStation = departureStation;
}

QRail::StationEngine::Station *RouterEngine::FootpathProfile::arrivalStation() const
{
    return m_arrivalStation;
}

void RouterEngine::FootpathProfile::setArrivalStation(QRail::StationEngine::Station *arrivalStation)
{
    m_arrivalStation = arrivalStation;
}
