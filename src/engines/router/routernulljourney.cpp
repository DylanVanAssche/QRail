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
#include "routernulljourney.h"
using namespace QRail;
RouterEngine::NullJourney *RouterEngine::NullJourney::m_instance = nullptr;

/**
 * @file routernulljourney.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Gets a RouterEngine::NullJourney instance
 * @return RouterEngine::NullJourney *journey
 * @package RouterEngine
 * @public
 * Constructs a RouterEngine::NullJourney if none exists and returns the instance.
 */
RouterEngine::NullJourney::NullJourney(
    const QList<QRail::RouterEngine::Route *> &routes,
    const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray,
    const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray,
    const QUrl &departureStation,
    const QUrl &arrivalStation,
    const quint16 &maxTransfers,
    const QUrl &hydraPrevious,
    const QUrl &hydraNext,
    QObject *parent) : QObject(routes, TArray, SArray, departureStation, arrivalStation, maxTransfers,
                                   hydraPrevious, hydraNext, parent)
{

}

/**
 * @file routernulljourney.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Gets a RouterEngine::NullJourney instance
 * @return RouterEngine::NullJourney *journey
 * @package RouterEngine
 * @public
 * Constructs a RouterEngine::NullJourney if none exists and returns the instance.
 */
RouterEngine::NullJourney *QRail::RouterEngine::NullJourney::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new NullJourney";
        m_instance = new NullJourney(QList<QRail::RouterEngine::Route *>(),
                                     QMap<QUrl, QRail::RouterEngine::TrainProfile *>(),
                                     QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> >(),
                                     QUrl(),
                                     QUrl(),
                                     0,
                                     QUrl(),
                                     QUrl());
    }
    return m_instance;
}
