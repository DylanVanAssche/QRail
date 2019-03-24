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
#ifndef ROUTERNULLJOURNEY_H
#define ROUTERNULLJOURNEY_H

#include <QtCore/QList>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QObject>

#include "engines/router/routerjourney.h"
#include "qrail.h"

namespace QRail {
namespace RouterEngine {
//! An RouterEngine::NullJourney contains a empty journey.
/*!
    \class NullJourney
    A Journey holds all the necessary information about a journey for the connection between 2 stops.
    A journey can keep track of multiple routes to get from A to B.
    \warning Receiving a NullJourney from the QRail library indicates that something must be terrible wrong! Check you debug output carefully.
 */
class QRAIL_SHARED_EXPORT NullJourney : public Journey
{
public:
    static NullJourney *getInstance();

private:
    //! Gets a RouterEngine::NullJourney instance.
    /*!
        \return board a RouterEngine::NullJourney instance.
        \public
        Constructs a RouterEngine::NullJourney if none exists and returns the instance.
     */
    explicit NullJourney(const QList<QRail::RouterEngine::Route *> routes,
                         const QDateTime departureTime,
                         const QDateTime arrivalTime,
                         const QUrl hydraNext,
                         const QUrl hydraPrevious,
                         QObject *parent = nullptr);
    static NullJourney *m_instance;
};
}
}

#endif // ROUTERNULLJOURNEY_H
