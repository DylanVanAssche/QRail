/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of BeRail.                                               *
 *                                                                            *
 *   BeRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   BeRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with BeRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#ifndef CSASTATION_H
#define CSASTATION_H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QLocale>
#include <QtCore/QMap>
#include <QtCore/QTime>
#include <QtCore/QPair>
#include <QtCore/QMetaType>
#include <QtPositioning/QGeoCoordinate>
#include <QtPositioning/QGeoAddress>

namespace CSA {
class Station : public QObject
{
    Q_OBJECT
public:
    enum class Day {
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };
    // Empty station for NullStation
    explicit Station(QObject *parent = nullptr);
    // Without facilities
    explicit Station(
            const QUrl &uri,
            const QMap<QLocale::Language, QString> &name,
            const QLocale::Country &country,
            const QGeoCoordinate &position,
            const qreal &averageStopTimes,
            QObject *parent = nullptr);
    // With facilities
    explicit Station(
            const QUrl &uri,
            const QMap<QLocale::Language, QString> &name,
            const QLocale::Country &country,
            const QGeoCoordinate &position,
            const QGeoAddress &address,
            const bool &hasTicketVendingMachine,
            const bool &hasLuggageLockers,
            const bool &hasFreeParking,
            const bool &hasTaxi,
            const bool &hasBicycleSpots,
            const bool &hasBlueBike,
            const bool &hasBus,
            const bool &hasTram,
            const bool &hasMetro,
            const bool &hasWheelchairAvailable,
            const bool &hasRamp,
            const qint16 &disabledParkingSpots,
            const bool &hasElevatedPlatform,
            const bool &hasEscalatorUp,
            const bool &hasEscalatorDown,
            const bool &hasElevatorPlatform,
            const bool &hasHearingAidSignal,
            const QMap<CSA::Station::Day, QPair<QTime, QTime>> &openingHours,
            const qreal &averageStopTimes,
            QObject *parent = nullptr);
    // Without facilities and platforms
    explicit Station(
            const QUrl &uri,
            const QMap<QLocale::Language, QString> &name,
            const QLocale::Country &country,
            const QGeoCoordinate &position,
            const qreal &averageStopTimes,
            const QMap<QUrl, QString> &platforms,
            QObject *parent = nullptr);
    // With facilities and platforms
    explicit Station(
            const QUrl &uri,
            const QMap<QLocale::Language, QString> &name,
            const QLocale::Country &country,
            const QGeoCoordinate &position,
            const QGeoAddress &address,
            const bool &hasTicketVendingMachine,
            const bool &hasLuggageLockers,
            const bool &hasFreeParking,
            const bool &hasTaxi,
            const bool &hasBicycleSpots,
            const bool &hasBlueBike,
            const bool &hasBus,
            const bool &hasTram,
            const bool &hasMetro,
            const bool &hasWheelchairAvailable,
            const bool &hasRamp,
            const qint16 &disabledParkingSpots,
            const bool &hasElevatedPlatform,
            const bool &hasEscalatorUp,
            const bool &hasEscalatorDown,
            const bool &hasElevatorPlatform,
            const bool &hasHearingAidSignal,
            const QMap<CSA::Station::Day, QPair<QTime, QTime>> &openingHours,
            const qreal &averageStopTimes,
            const QMap<QUrl, QString> &platforms,
            QObject *parent = nullptr);
    QUrl uri() const;
    void setUri(const QUrl &uri);
    QMap<QLocale::Language, QString> name() const;
    void setName(const QMap<QLocale::Language, QString> &name);
    QLocale::Country country() const;
    void setCountry(const QLocale::Country &country);
    QGeoCoordinate position() const;
    void setPosition(const QGeoCoordinate &position);
    QGeoAddress address() const;
    void setAddress(const QGeoAddress &address);
    bool hasTicketVendingMachine() const;
    void setHasTicketVendingMachine(const bool &hasTicketVendingMachine);
    bool hasLuggageLockers() const;
    void setHasLuggageLockers(const bool &hasLuggageLockers);
    bool hasFreeParking() const;
    void setHasFreeParking(const bool &hasFreeParking);
    bool hasTaxi() const;
    void setHasTaxi(const bool &hasTaxi);
    bool hasBicycleSpots() const;
    void setHasBicycleSpots(const bool &hasBicycleSpots);
    bool hasBlueBike() const;
    void setHasBlueBike(const bool &hasBlueBike);
    bool hasBus() const;
    void setHasBus(const bool &hasBus);
    bool hasTram() const;
    void setHasTram(const bool &hasTram);
    bool hasMetro() const;
    void setHasMetro(const bool &hasMetro);
    bool hasWheelchairAvailable() const;
    void setHasWheelchairAvailable(const bool &hasWheelchairAvailable);
    bool hasRamp() const;
    void setHasRamp(const bool &hasRamp);
    qint16 disabledParkingSpots() const;
    void setDisabledParkingSpots(const qint16 &disabledParkingSpots);
    bool hasElevatedPlatform() const;
    void setHasElevatedPlatform(const bool &hasElevatedPlatform);
    bool hasEscalatorUp() const;
    void setHasEscalatorUp(const bool &hasEscalatorUp);
    bool hasEscalatorDown() const;
    void setHasEscalatorDown(const bool &hasEscalatorDown);
    bool hasElevatorPlatform() const;
    void setHasElevatorPlatform(const bool &hasElevatorPlatform);
    bool hasHearingAidSignal() const;
    void setHasHearingAidSignal(const bool &hasHearingAidSignal);
    QMap<CSA::Station::Day, QPair<QTime, QTime> > openingHours() const;
    void setOpeningHours(const QMap<CSA::Station::Day, QPair<QTime, QTime> > &openingHours);
    qreal averageStopTimes() const;
    void setAverageStopTimes(const qreal &averageStopTimes);
    QMap<QUrl, QString> platforms() const;
    void setPlatforms(const QMap<QUrl, QString> &platforms);

signals:
    void uriChanged();
    void nameChanged();
    void countryChanged();
    void positionChanged();
    void addressChanged();
    void hasTicketVendingMachineChanged();
    void hasLuggageLockersChanged();
    void hasFreeParkingChanged();
    void hasTaxiChanged();
    void hasBicylceSpotsChanged();
    void hasBlueBikeChanged();
    void hasBusChanged();
    void hasTramChanged();
    void hasMetroChanged();
    void hasWheelchairAvailableChanged();
    void hasRampChanged();
    void disabledParkingSpotsChanged();
    void hasElevatedPlatformChanged();
    void hasEscalatorUpChanged();
    void hasEscalatorDownChanged();
    void hasHearingAidSignalChanged();
    void openingHoursChanged();
    void averageStopTimesChanged();
    void platformsChanged();

private:
    QUrl m_uri;
    QMap<QLocale::Language, QString> m_name;
    QLocale::Country m_country;
    QGeoCoordinate m_position;
    QGeoAddress m_address;
    bool m_hasTicketVendingMachine;
    bool m_hasLuggageLockers;
    bool m_hasFreeParking;
    bool m_hasTaxi;
    bool m_hasBicycleSpots;
    bool m_hasBlueBike;
    bool m_hasBus;
    bool m_hasTram;
    bool m_hasMetro;
    bool m_hasWheelchairAvailable;
    bool m_hasRamp;
    qint16 m_disabledParkingSpots;
    bool m_hasElevatedPlatform;
    bool m_hasEscalatorUp;
    bool m_hasEscalatorDown;
    bool m_hasElevatorPlatform;
    bool m_hasHearingAidSignal;
    QMap<CSA::Station::Day, QPair<QTime, QTime>> m_openingHours;
    qreal m_averageStopTimes;
    QMap<QUrl, QString> m_platforms;

    Q_ENUM(Day)
};
}

#endif // CSASTATION_H
