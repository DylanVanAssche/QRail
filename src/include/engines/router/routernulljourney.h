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

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QList>

#include "engines/router/routerjourney.h"
#include "engines/router/routerroute.h"
#include "qrail.h"

namespace QRail {
namespace RouterEngine {
class QRAIL_SHARED_EXPORT NullJourney : public RouterEngine::Journey
{
public:
    static NullJourney *getInstance();

private:
    explicit NullJourney(const QList<QRail::RouterEngine::Route *> routes,
                         const QUrl hydraNext,
                         const QUrl hydraPrevious,
                         const QDateTime from,
                         const QDateTime until,
                         QObject *parent = nullptr);
    static RouterEngine::NullJourney *m_instance;
};
}
}

#endif // ROUTERNULLJOURNEY_H
