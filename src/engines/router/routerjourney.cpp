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

QRail::RouterEngine::Journey::Journey(QObject *parent) : QObject(parent)
{
    m_routes = QList<QRail::RouterEngine::Route *>();
    m_departureTime = QDateTime();
    m_arrivalTime = QDateTime();
    m_hydraNext = QUrl();
    m_hydraPrevious = QUrl();
}

RouterEngine::Journey::Journey(const QList<RouterEngine::Route *> routes,
                               const QDateTime departureTime,
                               const QDateTime arrivalTime,
                               const QUrl hydraNext,
                               const QUrl hydraPrevious,
                               QObject *parent) : QObject(parent)
{
    m_routes = routes;
    m_departureTime = departureTime;
    m_arrivalTime = arrivalTime;
    m_hydraNext = hydraNext;
    m_hydraPrevious = hydraPrevious;
}

QList<QRail::RouterEngine::Route *> QRail::RouterEngine::Journey::routes() const
{
    return m_routes;
}

void QRail::RouterEngine::Journey::setRoutes(const QList<QRail::RouterEngine::Route *> &routes)
{
    m_routes = routes;
}

QUrl QRail::RouterEngine::Journey::hydraNext() const
{
    return m_hydraNext;
}

void QRail::RouterEngine::Journey::setHydraNext(const QUrl &hydraNext)
{
    // New hydraNext URI is further in time?
    if (hydraNext.hasQuery() && this->hydraNext().isValid()) {
        QUrlQuery queryNewHydraNext = QUrlQuery(hydraNext.query());
        QUrlQuery queryOldHydraNext = QUrlQuery(this->hydraNext().query());
        QDateTime timeNewHydraNext = QDateTime::fromString(queryNewHydraNext.queryItemValue("departureTime"), Qt::ISODate);
        QDateTime timeOldHydraNext = QDateTime::fromString(queryOldHydraNext.queryItemValue("departureTime"), Qt::ISODate);
        qDebug() << "HYDRA NEXT="
                 << hydraNext
                 << timeNewHydraNext
                 << "|"
                 << this->hydraNext()
                 << timeOldHydraNext;

        // Only accept URI that's later in time
        if (timeNewHydraNext > timeOldHydraNext) {
            m_hydraNext = hydraNext;
            qDebug() << "Hydra next NEW:" << m_hydraNext;
        }
    }
    // Current hydraNext is still empty, setting it to the received hydraNext
    else {
        qDebug() << "Empty hydraNext";
        m_hydraNext = hydraNext;
    }
}

QUrl QRail::RouterEngine::Journey::hydraPrevious() const
{
    return m_hydraPrevious;
}

void QRail::RouterEngine::Journey::setHydraPrevious(const QUrl &hydraPrevious)
{
    // New hydraPrevious URI is earlier in time?
    if (hydraPrevious.hasQuery() && this->hydraPrevious().hasQuery()) {
        QUrlQuery queryNewHydraPrevious = QUrlQuery(hydraPrevious.query());
        QUrlQuery queryOldHydraPrevious = QUrlQuery(this->hydraPrevious().query());
        QDateTime timeNewHydraPrevious = QDateTime::fromString(queryNewHydraPrevious.queryItemValue("departureTime"), Qt::ISODate);
        QDateTime timeOldHydraPrevious = QDateTime::fromString(queryOldHydraPrevious.queryItemValue("departureTime"), Qt::ISODate);
        qDebug() << "HYDRA PREVIOUS="
                 << hydraPrevious
                 << timeNewHydraPrevious
                 << "|"
                 << this->hydraPrevious()
                 << timeOldHydraPrevious;

        // Only accept URI that's earlier in time
        if (timeNewHydraPrevious < timeOldHydraPrevious) {
            m_hydraPrevious = hydraPrevious;
            qDebug() << "Hydra previous NEW:" << m_hydraPrevious;
        }
    }
    // Current hydraPrevious is still empty, setting it to the received hydraPrevious
    else {
        qDebug() << "Empty hydraPrevious";
        m_hydraPrevious = hydraPrevious;
    }
}

QMap<QUrl, QDateTime> QRail::RouterEngine::Journey::S_EarliestArrivalTime() const
{
    return m_S_EarliestArrivalTime;
}

void QRail::RouterEngine::Journey::setS_EarliestArrivalTime(const QMap<QUrl, QDateTime> &S_EarliestArrivalTime)
{
    m_S_EarliestArrivalTime = S_EarliestArrivalTime;
}

QUrl QRail::RouterEngine::Journey::arrivalStationURI() const
{
    return m_arrivalStationURI;
}

void QRail::RouterEngine::Journey::setArrivalStationURI(const QUrl &arrivalStationURI)
{
    m_arrivalStationURI = arrivalStationURI;
}

qint16 QRail::RouterEngine::Journey::maxTransfers() const
{
    return m_maxTransfers;
}

void QRail::RouterEngine::Journey::setMaxTransfers(const qint16 &maxTransfers)
{
    m_maxTransfers = maxTransfers;
}

void RouterEngine::Journey::addSnapshotJourney(RouterEngine::SnapshotJourney *snapshotJourney)
{
    m_snapshotJourneys.append(snapshotJourney);
}

void RouterEngine::Journey::restoreBeforePage(QUrl pageURI)
{
    // We can't restore anything if we don't have anything
    if(m_snapshotJourneys.length() == 0) {
        qCritical() << "Unable to restore Journey, cached journeys list is empty!";
        return;
    }

    // First cached journey is affected? Reroute completely.
    if(m_snapshotJourneys.at(0)->pageURI() == pageURI) {
        qDebug() << "First snapshot is affected, clearing Journey";
        this->setRoutes(QList<QRail::RouterEngine::Route *>());
        this->setTArray(QMap<QUrl, QRail::RouterEngine::TrainProfile *> ());
        this->setSArray(QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> >());
        this->setT_EarliestArrivalTime(QMap<QUrl, qint16>());
        this->setS_EarliestArrivalTime(QMap<QUrl, QDateTime>());
        this->setHydraNext(m_snapshotJourneys.at(0)->hydraNext());
        this->setHydraPrevious(m_snapshotJourneys.at(0)->hydraPrevious());
        return;
    }

    // Look for the page in the list and restore the Journey before that page
    for(qint64 c=0; c < m_snapshotJourneys.length(); c++) {
        qDebug() << "Searching for previous Journey, just before our affected page";
        QRail::RouterEngine::SnapshotJourney *snapshotJourney = m_snapshotJourneys.at(c);
        if(snapshotJourney->pageURI() == pageURI) {
            QRail::RouterEngine::SnapshotJourney *previousSnapshotJourney = m_snapshotJourneys.at(c-1);
            this->setRoutes(previousSnapshotJourney->routes());
            this->setTArray(previousSnapshotJourney->TArray());
            this->setSArray(previousSnapshotJourney->SArray());
            this->setT_EarliestArrivalTime(previousSnapshotJourney->T_EarliestArrivalTime());
            this->setS_EarliestArrivalTime(previousSnapshotJourney->S_EarliestArrivalTime());
            this->setHydraNext(m_snapshotJourneys.at(c)->hydraNext());
            this->setHydraPrevious(m_snapshotJourneys.at(c)->hydraPrevious());
            qDebug() << "Succesfully restored the previous Journey";
            return;
        }
    }

    qCritical() << "Page couldn't be found in the cached journeys. This might NEVER happen!";
}

QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > QRail::RouterEngine::Journey::SArray() const
{
    return m_SArray;
}

void QRail::RouterEngine::Journey::setSArray(const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray)
{
    m_SArray = SArray;
}

QMap<QUrl, QRail::RouterEngine::TrainProfile *> QRail::RouterEngine::Journey::TArray() const
{
    return m_TArray;
}

void QRail::RouterEngine::Journey::setTArray(const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray)
{
    m_TArray = TArray;
}

QUrl QRail::RouterEngine::Journey::departureStationURI() const
{
    return m_departureStationURI;
}

void QRail::RouterEngine::Journey::setDepartureStationURI(const QUrl &departureStationURI)
{
    m_departureStationURI = departureStationURI;
}

QMap<QUrl, qint16> QRail::RouterEngine::Journey::T_EarliestArrivalTime() const
{
    return m_T_EarliestArrivalTime;
}

void QRail::RouterEngine::Journey::setT_EarliestArrivalTime(const QMap<QUrl, qint16> &T_EarliestArrivalTime)
{
    m_T_EarliestArrivalTime = T_EarliestArrivalTime;
}

QDateTime QRail::RouterEngine::Journey::arrivalTime() const
{
    return m_arrivalTime;
}

void QRail::RouterEngine::Journey::setArrivalTime(const QDateTime &arrivalTime)
{
    m_arrivalTime = arrivalTime;
}

QDateTime QRail::RouterEngine::Journey::departureTime() const
{
    return m_departureTime;
}

void QRail::RouterEngine::Journey::setDepartureTime(const QDateTime &departureTime)
{
    m_departureTime = departureTime;
}
