#ifndef ROUTERJOURNEY_H
#define ROUTERJOURNEY_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
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
    explicit Journey(const QList<QRail::RouterEngine::Route *> routes,
                     const QDateTime departureTime,
                     const QDateTime arrivalTime,
                     const QUrl hydraNext,
                     const QUrl hydraPrevious,
                     QObject *parent = nullptr);
    QList<QRail::RouterEngine::Route *> routes() const;
    void setRoutes(const QList<QRail::RouterEngine::Route *> &routes);
    QUrl hydraNext() const;
    void setHydraNext(const QUrl &hydraNext);
    QUrl hydraPrevious() const;
    void setHydraPrevious(const QUrl &hydraPrevious);
    QDateTime departureTime() const;
    void setDepartureTime(const QDateTime &departureTime);
    QDateTime arrivalTime() const;
    void setArrivalTime(const QDateTime &arrivalTime);
    QMap<QUrl, qint16> T_EarliestArrivalTime() const;
    void setT_EarliestArrivalTime(const QMap<QUrl, qint16> &T_EarliestArrivalTime);
    QMap<QUrl, QDateTime> S_EarliestArrivalTime() const;
    void setS_EarliestArrivalTime(const QMap<QUrl, QDateTime> &S_EarliestArrivalTime);
    QUrl departureStationURI() const;
    void setDepartureStationURI(const QUrl &departureStationURI);
    QUrl arrivalStationURI() const;
    void setArrivalStationURI(const QUrl &arrivalStationURI);
    QMap<QUrl, QRail::RouterEngine::TrainProfile *> TArray() const;
    void setTArray(const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray);
    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > SArray() const;
    void setSArray(const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray);
    qint16 maxTransfers() const;
    void setMaxTransfers(const qint16 &maxTransfers);

private:
    QList<QRail::RouterEngine::Route *> m_routes;
    QDateTime m_departureTime;
    QDateTime m_arrivalTime;
    qint16 m_maxTransfers;
    QUrl m_hydraNext;
    QUrl m_hydraPrevious;
    QUrl m_departureStationURI;
    QUrl m_arrivalStationURI;
    QMap<QUrl, qint16> m_T_EarliestArrivalTime;
    QMap<QUrl, QDateTime> m_S_EarliestArrivalTime;
    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > m_SArray;
    QMap<QUrl, QRail::RouterEngine::TrainProfile *> m_TArray;
};
}
}

#endif // ROUTERJOURNEY_H
