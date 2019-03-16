#ifndef ROUTERJOURNEY_H
#define ROUTERJOURNEY_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>

#include "engines/router/routerroute.h"
#include "engines/router/routerstationstopprofile.h"
#include "engines/router/routertrainprofile.h"
#include "engines/router/routersnapshotjourney.h"
#include "fragments/fragmentspage.h"

namespace QRail {
namespace RouterEngine {
//! A RouterEngine::Journey contains a complete journey between 2 stops.
/*!
    \class Journey
    A Journey holds all the necessary information about a journey for the connection between 2 stops.
    A journey can keep track of multiple routes to get from A to B.
 */
class Journey : public QObject
{
    Q_OBJECT
public:
    //! QRail::RouterEngine::Journey constructor: empty.
    /*!
        \param parent QObject parent-child memory management.
        \public
        Constructs a QRail::RouterEngine::Journey with the given parent.
     */
    explicit Journey(QObject *parent = nullptr);
    //! QRail::RouterEngine::Journey constructor: empty.
    /*!
        \param routes A list of possible routes to get from A to B.
        \param departureTime The requested departure time.
        \param arrivalTime The requested arrival time.
        \param hydraNext The URI to the previous page.
        \param hydraPrevious The URI to the next page.
        \param parent QObject parent-child memory management.
        \public
        Constructs a QRail::RouterEngine::Journey with the given parent.
     */
    explicit Journey(const QList<QRail::RouterEngine::Route *> routes,
                     const QDateTime departureTime,
                     const QDateTime arrivalTime,
                     const QUrl hydraNext,
                     const QUrl hydraPrevious,
                     QObject *parent = nullptr);
    //! Gets the possible routes of the Journey.
    /*!
        \return routes A list with all possibles routes for this journey.
        \public
        Gets the list of routes for the Journey and returns it.
     */
    QList<QRail::RouterEngine::Route *> routes() const;
    //! Sets the possible routes of the Journey.
    /*!
        \param routes A list with all possibles routes for this journey.
        \public
        Sets the list of routes for the Journey.
     */
    void setRoutes(const QList<QRail::RouterEngine::Route *> &routes);
    //! Gets the next hydra URI of the Journey.
    /*!
        \return hydraNext The URI with the next page URI.
        \public
        Gets the next hydra URI of the Journey and returns it.
     */
    QUrl hydraNext() const;
    //! Sets the next hydra URI of the Journey.
    /*!
        \param hydraNext The URI with the next page URI.
        \public
        Sets the next hydra URI of the Journey.
     */
    void setHydraNext(const QUrl &hydraNext);
    //! Gets the previous hydra URI of the Journey.
    /*!
        \return hydraPrevious The URI with the next page URI.
        \public
        Gets the previous hydra URI of the Journey and returns it.
     */
    QUrl hydraPrevious() const;
    //! Sets the previous hydra URI of the Journey.
    /*!
        \param hydraPrevious The URI with the next page URI.
        \public
        Sets the previous hydra URI of the Journey.
     */
    void setHydraPrevious(const QUrl &hydraPrevious);
    //! Gets the departure time of the Journey.
    /*!
        \return departureTime The requested departure time.
        \public
        Gets the departure time of the Journey and returns it.
     */
    QDateTime departureTime() const;
    //! Sets the departure time of the Journey.
    /*!
        \param departureTime The requested departure time.
        \public
        Sets the departure time of the Journey.
     */
    void setDepartureTime(const QDateTime &departureTime);
    //! Gets the arrival time of the Journey.
    /*!
        \return arrivalTime The requested arrival time.
        \public
        Gets the arrival time of the Journey and returns it.
     */
    QDateTime arrivalTime() const;
    //! Sets the arrival time of the Journey.
    /*!
        \param arrivalTime The requested arrival time.
        \public
        Sets the arrival time of the Journey.
     */
    void setArrivalTime(const QDateTime &arrivalTime);
    //! Gets the T array for the Earliest Arrival CSA of the Journey.
    /*!
        \return T_EarliestArrivalTime T array for Earliest Arrival CSA.
        \public
        Gets the T array for the Earliest Arrival CSA of the Journey and returns it.
     */
    QMap<QUrl, qint16> T_EarliestArrivalTime() const;
    //! Sets the T array for the Earliest Arrival CSA of the Journey.
    /*!
        \param T_EarliestArrivalTime T array for Earliest Arrival CSA.
        \public
        Sets the T array for the Earliest Arrival CSA of the Journey.
     */
    void setT_EarliestArrivalTime(const QMap<QUrl, qint16> &T_EarliestArrivalTime);
    //! Gets the S array for the Earliest Arrival CSA of the Journey.
    /*!
        \return S_EarliestArrivalTime S array for Earliest Arrival CSA.
        \public
        Gets the S array for the Earliest Arrival CSA of the Journey and returns it.
     */
    QMap<QUrl, QDateTime> S_EarliestArrivalTime() const;
    //! Sets the S array for the Earliest Arrival CSA of the Journey.
    /*!
        \param S_EarliestArrivalTime S array for Earliest Arrival CSA.
        \public
        Sets the S array for the Earliest Arrival CSA of the Journey.
     */
    void setS_EarliestArrivalTime(const QMap<QUrl, QDateTime> &S_EarliestArrivalTime);
    //! Gets the departure station URI of the Journey.
    /*!
        \return departureStationURI The URI of the departure station.
        \public
        Gets the departure station URI of the Journey and returns it.
     */
    QUrl departureStationURI() const;
    //! Sets the departure station URI of the Journey.
    /*!
        \param departureStationURI The URI of the departure station.
        \public
        Sets the departure station URI of the Journey.
     */
    void setDepartureStationURI(const QUrl &departureStationURI);
    //! Gets the arrival station URI of the Journey.
    /*!
        \return arrivalStationURI The URI of the arrival station.
        \public
        Gets the arrival station URI of the Journey and returns it.
     */
    QUrl arrivalStationURI() const;
    //! Sets the arrival station URI of the Journey.
    /*!
        \param arrivalStationURI The URI of the arrival station.
        \public
        Sets the arrival station URI of the Journey.
     */
    void setArrivalStationURI(const QUrl &arrivalStationURI);
    //! Gets the T array for the Profile CSA of the Journey.
    /*!
        \return TArray T array for Profile CSA.
        \public
        Gets the T array for the Profile CSA of the Journey and returns it.
     */
    QMap<QUrl, QRail::RouterEngine::TrainProfile *> TArray() const;
    //! Sets the T array for the Profile CSA of the Journey.
    /*!
        \return TArray T array for Profile CSA.
        \public
        Sets the T array for the Profile CSA of the Journey.
     */
    void setTArray(const QMap<QUrl, QRail::RouterEngine::TrainProfile *> &TArray);
    //! Gets the S array for the Profile CSA of the Journey.
    /*!
        \return SArray T array for Profile CSA.
        \public
        Gets the S array for the Profile CSA of the Journey and returns it.
     */
    QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > SArray() const;
    //! Sets the S array for the Profile CSA of the Journey.
    /*!
        \param SArray T array for Profile CSA.
        \public
        Sets the S array for the Profile CSA of the Journey.
     */
    void setSArray(const QMap<QUrl, QList<QRail::RouterEngine::StationStopProfile *> > &SArray);
    //! Gets the maximum amount of transfers for the Journey.
    /*!
        \return station StationEngine::Station object with information about the associated station.
        \note The real amount of transfers is always lower than or equal to this one.
        \public
        Gets the maximum amount of transfers for the Journey and returns it.
     */
    qint16 maxTransfers() const;
    //! Sets the maximum amount of transfers for the Journey.
    /*!
        \return station StationEngine::Station object with information about the associated station.
        \note The real amount of transfers is always lower than or equal to this one.
        \public
        Sets the maximum amount of transfers for the Journey.
     */
    void setMaxTransfers(const qint16 &maxTransfers);

    //! Add a SnapshotJourney object to this Journey
    void addSnapshotJourney(QRail::RouterEngine::SnapshotJourney *snapshotJourney);
    //! Restore Journey before page
    void restoreBeforePage(QUrl pageURI);

private:
    QList<QRail::RouterEngine::SnapshotJourney *> m_snapshotJourneys;
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
