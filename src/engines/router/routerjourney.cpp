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
#include "engines/router/routerjourney.h"
using namespace QRail;

RouterEngine::Journey::Journey(QObject *parent) : QObject(parent)
{

}

RouterEngine::Journey::Journey(const QList<RouterEngine::Route *> &routes,
                               const QMap<QUrl, RouterEngine::TrainProfile *> &TArray,
                               const QMap<QUrl, QList<RouterEngine::StationStopProfile *> > &SArray,
                               const QUrl &departureStation,
                               const QUrl &arrivalStation,
                               const QDateTime &departureTime,
                               const QDateTime &arrivalTime,
                               const quint16 &maxTransfers,
                               QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_routes = routes;
    m_TArray = TArray;
    m_SArray = SArray;
    m_departureStation = departureStation;
    m_arrivalStation = arrivalStation;
    m_departureTime = departureTime;
    m_arrivalTime = arrivalTime;
    m_maxTransfers = maxTransfers;
    m_hydraPrevious = QUrl();
    m_hydraNext = QUrl();
}

// Getters & Setters
QList<QRail::RouterEngine::Route *> RouterEngine::Journey::routes() const
{
    return m_routes;
}

void RouterEngine::Journey::setRoutes(const QList<QRail::RouterEngine::Route *> &routes)
{
    m_routes = routes;
    emit this->routesChanged();
}

QMap<QUrl, QRail::RouterEngine::TrainProfile *> RouterEngine::Journey::TArray() const
{
    return m_TArray;
}

void RouterEngine::Journey::setTArray(const QMap<QUrl, QRail::RouterEngine::TrainProfile *>
                                      &TArray)
{
    m_TArray = TArray;
    emit this->TArrayChanged();
}

QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > RouterEngine::Journey::SArray() const
{
    return m_SArray;
}

void RouterEngine::Journey::setSArray(const
                                      QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray)
{
    m_SArray = SArray;
    emit this->SArrayChanged();
}

QUrl RouterEngine::Journey::departureStation() const
{
    return m_departureStation;
}

void RouterEngine::Journey::setDepartureStation(const QUrl &departureStation)
{
    m_departureStation = departureStation;
    emit this->departureStationChanged();
}

QUrl RouterEngine::Journey::arrivalStation() const
{
    return m_arrivalStation;
}

void RouterEngine::Journey::setArrivalStation(const QUrl &arrivalStation)
{
    m_arrivalStation = arrivalStation;
    emit this->arrivalStationChanged();
}

quint16 RouterEngine::Journey::maxTransfers() const
{
    return m_maxTransfers;
}

void RouterEngine::Journey::setMaxTransfers(const quint16 &maxTransfers)
{
    m_maxTransfers = maxTransfers;
    emit this->maxTransfersChanged();
}

QUrl RouterEngine::Journey::hydraPrevious() const
{
    return m_hydraPrevious;
}

void RouterEngine::Journey::setHydraPrevious(const QUrl &hydraPrevious)
{
    // New hydraPrevious URI is earlier in time?
    if (hydraPrevious.hasQuery() && this->hydraPrevious().hasQuery()) {
        QUrlQuery queryNewHydraPrevious = QUrlQuery(hydraPrevious.query());
        QUrlQuery queryOldHydraPrevious = QUrlQuery(this->hydraPrevious().query());
        QDateTime timeNewHydraPrevious = QDateTime::fromString(
                                             queryNewHydraPrevious.queryItemValue("departureTime"), Qt::ISODate);
        QDateTime timeOldHydraPrevious = QDateTime::fromString(
                                             queryOldHydraPrevious.queryItemValue("departureTime"), Qt::ISODate);
        // Only accept URI that's earlier in time
        if (timeNewHydraPrevious < timeOldHydraPrevious) {
            m_hydraPrevious = hydraPrevious;
            emit this->hydraPreviousChanged();
        }
    }
    // Current hydraPrevious is still empty, setting it to the received hydraPrevious
    else {
        m_hydraPrevious = hydraPrevious;
        emit this->hydraPreviousChanged();
    }
}

QUrl RouterEngine::Journey::hydraNext() const
{
    return m_hydraNext;
}

void RouterEngine::Journey::setHydraNext(const QUrl &hydraNext)
{
    // New hydraNext URI is further in time?
    if (hydraNext.hasQuery() && this->hydraNext().isValid()) {
        QUrlQuery queryNewHydraNext = QUrlQuery(hydraNext.query());
        QUrlQuery queryOldHydraNext = QUrlQuery(this->hydraNext().query());
        QDateTime timeNewHydraNext = QDateTime::fromString(
                                         queryNewHydraNext.queryItemValue("departureTime"), Qt::ISODate);
        QDateTime timeOldHydraNext = QDateTime::fromString(
                                         queryOldHydraNext.queryItemValue("departureTime"), Qt::ISODate);

        // Only accept URI that's later in time
        if (timeNewHydraNext > timeOldHydraNext) {
            m_hydraNext = hydraNext;
            emit this->hydraNextChanged();
        }
    }
    // Current hydraNext is still empty, setting it to the received hydraNext
    else {
        m_hydraNext = hydraNext;
        emit this->hydraNextChanged();
    }
}
