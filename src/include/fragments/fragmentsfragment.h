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

#ifndef LINKEDCONNECTIONFRAGMENT_H
#define LINKEDCONNECTIONFRAGMENT_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>
#include <QtCore/QString>

namespace QRail {
namespace Fragments {
//! A Fragments::Fragment contains a connection.
/*!
    \class Fragment
    A Fragments holds a connection between 2 stations.
 */
class Fragment : public QObject
{
    Q_OBJECT
public:
    //! The defined GTFS types by the GTFS specification.
    enum class GTFSTypes {
        REGULAR = 0,
        NOTAVAILABLE = 1,
        MUSTPHONE = 2,
        MUSTCOORDINATEWITHDRIVER = 3,
        UNKNOWN = 4
    };
    //! Constructs a Fragment.
    /*!
     * \param parent QObject parent-child memory management.
     * \public
     */
    explicit Fragment(QObject *parent = nullptr);
    //! Constructs a Fragment.
    /*!
     * \param uri The URI of the fragment.
     * \param departureStationURI The URI of the departure station of the connection.
     * \param arrivalStationURI The URI of the arrival station of the connection.
     * \param departureTime The departure time of the connection.
     * \param arrivalTime The arrival time of the connection.
     * \param departureDelay The departure delay of the connection.
     * \param arrivalDelay The arrival delay of the connection.
     * \param tripURI The URI of the trip of the vehicle.
     * \param routeURI The URI of the route of the vehicle.
     * \param direction The direction of the vehicle.
     * \param pickupType The type of pickup in the departure station.
     * \param dropOffType The type of drop off in the arrival station.
     * \param parent QObject parent-child memory management.
     * \public
     */
    explicit Fragment(
        const QUrl &uri,
        const QUrl &departureStationURI,
        const QUrl &arrivalStationURI,
        const QDateTime &departureTime,
        const QDateTime &arrivalTime,
        const qint16 &departureDelay,
        const qint16 &arrivalDelay,
        const QUrl &tripURI,
        const QUrl &routeURI,
        const QString &direction,
        const GTFSTypes &pickupType,
        const GTFSTypes &dropOffType,
        QObject *parent = nullptr
    );
    QUrl uri() const;
    void setURI(const QUrl &uri);
    QUrl departureStationURI() const;
    void setDepartureStationURI(const QUrl &departureStationURI);
    QUrl arrivalStationURI() const;
    void setArrivalStationURI(const QUrl &arrivalStationURI);
    QDateTime departureTime() const;
    void setDepartureTime(const QDateTime &departureTime);
    QDateTime arrivalTime() const;
    void setArrivalTime(const QDateTime &arrivalTime);
    qint16 departureDelay() const;
    void setDepartureDelay(const qint16 &departureDelay);
    qint16 arrivalDelay() const;
    void setArrivalDelay(const qint16 &arrivalDelay);
    QUrl tripURI() const;
    void setTripURI(const QUrl &tripURI);
    QUrl routeURI() const;
    void setRouteURI(const QUrl &routeURI);
    QString direction() const;
    void setDirection(const QString &direction);
    GTFSTypes pickupType() const;
    void setPickupType(const GTFSTypes &pickupType);
    GTFSTypes dropOffType() const;
    void setDropOffType(const GTFSTypes &dropOffType);

signals:
    void uriChanged();
    void departureStationURIChanged();
    void arrivalStationURIChanged();
    void departureTimeChanged();
    void arrivalTimeChanged();
    void departureDelayChanged();
    void arrivalDelayChanged();
    void tripURIChanged();
    void routeURIChanged();
    void directionChanged();
    void pickupTypeChanged();
    void dropOffTypeChanged();

private:
    QUrl m_uri;
    QUrl m_departureStationURI;
    QUrl m_arrivalStationURI;
    QDateTime m_departureTime;
    QDateTime m_arrivalTime;
    qint16 m_departureDelay;
    qint16 m_arrivalDelay;
    QUrl m_tripURI;
    QUrl m_routeURI;
    QString m_direction;
    GTFSTypes m_pickupType;
    GTFSTypes m_dropOffType;
};
}
}

#endif // LINKEDCONNECTIONFRAGMENT_H
