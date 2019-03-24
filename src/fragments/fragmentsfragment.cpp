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
#include "fragments/fragmentsfragment.h"
using namespace QRail;

QRail::Fragments::Fragment::Fragment(QObject *parent) : QObject(parent)
{

}

QRail::Fragments::Fragment::Fragment(
    const QUrl &uri, const QUrl &departureStationURI,
    const QUrl &arrivalStationURI,
    const QDateTime &departureTime,
    const QDateTime &arrivalTime,
    const qint16 &departureDelay,
    const qint16 &arrivalDelay,
    const QUrl &tripURI,
    const QUrl &routeURI,
    const QString &direction,
    const GTFSTypes &pickupType,
    const GTFSTypes &dropOffType,
    QObject *parent
): QObject(parent)
{

    // Avoid using setters in constructor to reduce signal emitting on instantiation.
    m_uri = uri;
    m_departureStationURI = departureStationURI;
    m_arrivalStationURI = arrivalStationURI;
    m_departureTime = departureTime;
    m_arrivalTime = arrivalTime;
    m_departureDelay = departureDelay;
    m_arrivalDelay = arrivalDelay;
    m_tripURI = tripURI;
    m_routeURI = routeURI;
    m_direction = direction;
    m_pickupType = pickupType;
    m_dropOffType = dropOffType;
}

// Getter & Setters
QUrl QRail::Fragments::Fragment::uri() const
{
    return m_uri;
}

void QRail::Fragments::Fragment::setURI(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

QUrl QRail::Fragments::Fragment::departureStationURI() const
{
    return m_departureStationURI;
}

void QRail::Fragments::Fragment::setDepartureStationURI(const QUrl &departureStationURI)
{
    m_departureStationURI = departureStationURI;
    emit this->departureStationURIChanged();
}

QUrl QRail::Fragments::Fragment::arrivalStationURI() const
{
    return m_arrivalStationURI;
}

void QRail::Fragments::Fragment::setArrivalStationURI(const QUrl &arrivalStationURI)
{
    m_arrivalStationURI = arrivalStationURI;
    emit this->arrivalStationURIChanged();
}

QDateTime QRail::Fragments::Fragment::departureTime() const
{
    return m_departureTime;
}

void QRail::Fragments::Fragment::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
    emit this->departureTimeChanged();
}

QDateTime QRail::Fragments::Fragment::arrivalTime() const
{
    return m_arrivalTime;
}

void QRail::Fragments::Fragment::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
    emit this->arrivalTimeChanged();
}

qint16 QRail::Fragments::Fragment::departureDelay() const
{
    return m_departureDelay;
}

void QRail::Fragments::Fragment::setDepartureDelay(const qint16 &departureDelay)
{
    m_departureDelay = departureDelay;
    emit this->departureDelayChanged();
}

qint16 QRail::Fragments::Fragment::arrivalDelay() const
{
    return m_arrivalDelay;
}

void QRail::Fragments::Fragment::setArrivalDelay(const qint16 &arrivalDelay)
{
    m_arrivalDelay = arrivalDelay;
    emit this->arrivalDelayChanged();
}

QUrl QRail::Fragments::Fragment::tripURI() const
{
    return m_tripURI;
}

void QRail::Fragments::Fragment::setTripURI(const QUrl &tripURI)
{
    m_tripURI = tripURI;
    emit this->tripURIChanged();
}

QUrl QRail::Fragments::Fragment::routeURI() const
{
    return m_routeURI;
}

void QRail::Fragments::Fragment::setRouteURI(const QUrl &routeURI)
{
    m_routeURI = routeURI;
    emit this->routeURIChanged();
}

QString QRail::Fragments::Fragment::direction() const
{
    return m_direction;
}

void QRail::Fragments::Fragment::setDirection(const QString &direction)
{
    m_direction = direction;
    emit this->directionChanged();
}

QRail::Fragments::Fragment::GTFSTypes QRail::Fragments::Fragment::dropOffType() const
{
    return m_dropOffType;
}

void QRail::Fragments::Fragment::setDropOffType(const GTFSTypes &dropOffType)
{
    m_dropOffType = dropOffType;
}

QRail::Fragments::Fragment::GTFSTypes QRail::Fragments::Fragment::pickupType() const
{
    return m_pickupType;
}

void QRail::Fragments::Fragment::setPickupType(const GTFSTypes &pickupType)
{
    m_pickupType = pickupType;
}
