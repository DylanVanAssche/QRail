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
#ifndef ROUTERJOURNEY_H
#define ROUTERJOURNEY_H

#include <QtCore/QObject>
#include <QtCore/QList>
#include <QtCore/QUrl>
#include <QtCore/QMap>
#include <QtCore/QDateTime>
#include <QtCore/QUrlQuery>

#include "engines/router/routerroute.h"
#include "engines/router/routerstationstopprofile.h"
#include "engines/router/routertrainprofile.h"

namespace QRail {
namespace RouterEngine {
class Journey : public QObject
{
    Q_OBJECT
public:
    explicit Journey(QObject *parent = nullptr);
    explicit Journey(const QList<QRail::RouterEngine::Route *> &routes,
                     const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray,
                     const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray,
                     const QUrl &departureStation,
                     const QUrl &arrivalStation,
                     const quint16 &maxTransfers,
                     QObject *parent = nullptr);
    QList<QRail::RouterEngine::Route *> routes() const;
    void setRoutes(const QList<QRail::RouterEngine::Route *> &routes);
    QMap<QUrl, QRail::RouterEngine::TrainProfile *> TArray() const;
    void setTArray(const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray);
    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > SArray() const;
    void setSArray(const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray);
    QUrl departureStation() const;
    void setDepartureStation(const QUrl &departureStation);
    QUrl arrivalStation() const;
    void setArrivalStation(const QUrl &arrivalStation);
    quint16 maxTransfers() const;
    void setMaxTransfers(const quint16 &maxTransfers);
    QUrl hydraPrevious() const;
    void setHydraPrevious(const QUrl &hydraPrevious);
    QUrl hydraNext() const;
    void setHydraNext(const QUrl &hydraNext);

signals:
    void routesChanged();
    void TArrayChanged();
    void SArrayChanged();
    void departureStationChanged();
    void arrivalStationChanged();
    void maxTransfersChanged();
    void hydraPreviousChanged();
    void hydraNextChanged();

private:
    QList<QRail::RouterEngine::Route *> m_routes;
    QMap<QUrl, QRail::RouterEngine::TrainProfile *> m_TArray;
    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > m_SArray;
    QUrl m_departureStation;
    QUrl m_arrivalStation;
    quint16 m_maxTransfers;
    QUrl m_hydraPrevious;
    QUrl m_hydraNext;
};
}
}

#endif // ROUTERJOURNEY_H
