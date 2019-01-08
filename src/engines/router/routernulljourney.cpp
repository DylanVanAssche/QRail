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
#include "engines/router/routernulljourney.h"
using namespace QRail;
RouterEngine::NullJourney *RouterEngine::NullJourney::m_instance = nullptr;

/**
 * @file routernulljourney.cpp
 * @author Dylan Van Assche
 * @date 08 Jan 2019
 * @brief RouterEngine::NullJourney constructor
 * @param const QList<QRail::RouterEngine::Route *> routes
 * @param const QDateTime departureTime
 * @param const QDateTime arrivalTime
 * @param const QDateTime &until
 * @param const QUrl hydraNext
 * @param const QUrl hydraPrevious
 * @param QObject *parent
 * @package RouterEngine
 * @private
 * Constructs a RouterEngine::NullJourney according to the Null design pattern.
 */
QRail::RouterEngine::NullJourney::NullJourney(const QList<QRail::RouterEngine::Route *> routes,
                                              const QDateTime departureTime,
                                              const QDateTime arrivalTime,
                                              const QUrl hydraNext,
                                              const QUrl hydraPrevious,
                                              QObject *parent) : Journey(routes,
                                                                         departureTime,
                                                                         arrivalTime,
                                                                         hydraNext,
                                                                         hydraPrevious,
                                                                         parent)
{

}

/**
 * @file routernulljourney.cpp
 * @author Dylan Van Assche
 * @date 08 Jan 2019
 * @brief Gets a RouterEngine::NullJourney instance
 * @return RouterEngine::NullJourney *board
 * @package RouterEngine
 * @public
 * Constructs a RouterEngine::NullJourney if none exists and returns the instance.
 */
QRail::RouterEngine::NullJourney *QRail::RouterEngine::NullJourney::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new NullJourney";
        m_instance = new NullJourney(
                    QList<QRail::RouterEngine::Route *>(),
                    QDateTime(),
                    QDateTime(),
                    QUrl(),
                    QUrl()
                    );
    }
    return m_instance;
}
