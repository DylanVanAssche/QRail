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

#include "engines/router/routerjourney.h"
#include "qrail.h"

namespace QRail {
namespace RouterEngine {
class QRAIL_SHARED_EXPORT NullJourney : public RouterEngine::Journey
{
public:
    static NullJourney *getInstance();

private:
    explicit NullJourney(const QList<QRail::RouterEngine::Route *> &routes,
                         const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray,
                         const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray,
                         const QUrl &departureStation,
                         const QUrl &arrivalStation,
                         const quint16 &maxTransfers,
                         QObject *parent = nullptr);
    static RouterEngine::NullJourney *m_instance;
};
}
}

#endif // ROUTERNULLJOURNEY_H
