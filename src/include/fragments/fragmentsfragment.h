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
class Fragment : public QObject
{
    Q_OBJECT
public:
    enum class GTFSTypes {
        REGULAR,
        NOTAVAILABLE,
        MUSTPHONE,
        MUSTCOORDINATEWITHDRIVER,
        UNKNOWN
    };
    explicit Fragment(QObject *parent = nullptr);
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
