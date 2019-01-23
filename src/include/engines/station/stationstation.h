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

#include "qrail.h"

namespace QRail {
namespace StationEngine {
/**
 * @class Station
 * @brief An StationEngine::Station contains station information.
 * A station contains all the information about a stop.
 */
class QRAIL_SHARED_EXPORT Station : public QObject
{
    Q_OBJECT
public:
    //! The days of the week
    enum class Day {
        MONDAY,
        TUESDAY,
        WEDNESDAY,
        THURSDAY,
        FRIDAY,
        SATURDAY,
        SUNDAY
    };
    //! Constructs a Station object.
    /*!
        \param parent QObject parent-child memory management.
     */
    explicit Station(QObject *parent = nullptr);
    //! Constructs a Station object without facilities and no platforms.
    /*!
        \param uri The URI of the station.
        \param name The name of the station.
        \param country The country where you can find this station.
        \param position The GPS position of it.
        \param averageStopTimes The average amount of stops in this station.
        \param officialTransferTimes The official time needed to transfer in this station.
        \param parent QObject parent-child memory management.
     */
    explicit Station(
        const QUrl &uri,
        const QMap<QLocale::Language, QString> &name,
        const QLocale::Country &country,
        const QGeoCoordinate &position,
        const qreal &averageStopTimes,
        const quint32 &officialTransferTimes,
        QObject *parent = nullptr);
    //! Constructs a Station object with facilities and no platforms.
    /*!
        \param uri The URI of the station.
        \param name The name of the station.
        \param country The country where you can find this station.
        \param position The GPS position of it.

        \param address The address of the station.
        \param hasTicketVendingMachine True if a ticket vending machine is available.
        \param hasLuggageLockers True if luggage lockers are available.
        \param hasFreeParking True if you can park your car here for free.
        \param hasTaxi True if you can take a taxi here.
        \param hasBicycleSpots True if the station provides bicycle spots.
        \param hasBlueBike True if you can use the Blue Bike service here.
        \param hasBus True if a bus stops here.
        \param hasTram True if a tram stops here.
        \param hasMetro True if a metro stops here.
        \param hasWheelchairAvailable True if wheelchairs are available.
        \param hasRamp True if a ramp is available for wheelchair users.
        \param disabledParkingSpots True if disabled parking spots are available.
        \param hasElevatedPlatform True if the platform is elevated.
        \param hasEscalatorUp True if you can take the escalator up.
        \param hasEscalatorDown True if you can take the escalator down.
        \param hasElevatorPlatform True if the platform has an elevator.
        \param hasAudioInductionLoop True if there's an audio induction loop.
        \param openingHours A list of opening hours of the station services.
        \param averageStopTimes The average amount of stops in this station.
        \param officialTransferTimes The official time needed to transfer in this station.
        \param parent QObject parent-child memory management.
     */
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
        const bool &hasAudioInductionLoop,
        const QMap<StationEngine::Station::Day, QPair<QTime, QTime>> &openingHours,
        const qreal &averageStopTimes,
        const quint32 &officialTransferTimes,
        QObject *parent = nullptr);
    //! Constructs a Station object without facilities, but with platforms.
    /*!
        \param uri The URI of the station.
        \param name The name of the station.
        \param country The country where you can find this station.
        \param position The GPS position of it.
        \param averageStopTimes The average amount of stops in this station.
        \param officialTransferTimes The official time needed to transfer in this station.
        \param platforms The platforms associated with this station.
        \param parent QObject parent-child memory management.
     */
    explicit Station(
        const QUrl &uri,
        const QMap<QLocale::Language, QString> &name,
        const QLocale::Country &country,
        const QGeoCoordinate &position,
        const qreal &averageStopTimes,
        const quint32 &officialTransferTimes,
        const QMap<QUrl, QString> &platforms,
        QObject *parent = nullptr);
    //! Constructs a Station object with facilities, but with platforms.
    /*!
        \param uri The URI of the station.
        \param name The name of the station.
        \param country The country where you can find this station.
        \param position The GPS position of it.

        \param address The address of the station.
        \param hasTicketVendingMachine True if a ticket vending machine is available.
        \param hasLuggageLockers True if luggage lockers are available.
        \param hasFreeParking True if you can park your car here for free.
        \param hasTaxi True if you can take a taxi here.
        \param hasBicycleSpots True if the station provides bicycle spots.
        \param hasBlueBike True if you can use the Blue Bike service here.
        \param hasBus True if a bus stops here.
        \param hasTram True if a tram stops here.
        \param hasMetro True if a metro stops here.
        \param hasWheelchairAvailable True if wheelchairs are available.
        \param hasRamp True if a ramp is available for wheelchair users.
        \param disabledParkingSpots True if disabled parking spots are available.
        \param hasElevatedPlatform True if the platform is elevated.
        \param hasEscalatorUp True if you can take the escalator up.
        \param hasEscalatorDown True if you can take the escalator down.
        \param hasElevatorPlatform True if the platform has an elevator.
        \param hasAudioInductionLoop True if there's an audio induction loop.
        \param openingHours A list of opening hours of the station services.
        \param averageStopTimes The average amount of stops in this station.
        \param officialTransferTimes The official time needed to transfer in this station.
        \param platforms The platforms associated with this station.
        \param parent QObject parent-child memory management.
     */
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
        const bool &hasAudioInductionLoop,
        const QMap<StationEngine::Station::Day, QPair<QTime, QTime>> &openingHours,
        const qreal &averageStopTimes,
        const quint32 &officialTransferTimes,
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
    bool hasAudioInductionLoop() const;
    void setHasAudioInductionLoop(const bool &hasAudioInductionLoop);
    QMap<StationEngine::Station::Day, QPair<QTime, QTime> > openingHours() const;
    void setOpeningHours(const QMap<StationEngine::Station::Day, QPair<QTime, QTime> > &openingHours);
    qreal averageStopTimes() const;
    void setAverageStopTimes(const qreal &averageStopTimes);
    quint32 officialTransferTimes() const;
    void setOfficialTransferTimes(const quint32 &officialTransferTimes);
    QMap<QUrl, QString> platforms() const;
    void setPlatforms(const QMap<QUrl, QString> &platforms);

signals:
    //! Emitted if the URI has been changed.
    void uriChanged();
    //! Emitted if the name has been changed.
    void nameChanged();
    //! Emitted if the country has been changed.
    void countryChanged();
    //! Emitted if the position has been changed.
    void positionChanged();
    //! Emitted if the address has been changed.
    void addressChanged();
    //! Emitted if the ticket vending state has been changed.
    void hasTicketVendingMachineChanged();
    //! Emitted if the luggage locker state has been changed.
    void hasLuggageLockersChanged();
    //! Emitted if the free parking state has been changed.
    void hasFreeParkingChanged();
    //! Emitted if the taxi state has been changed.
    void hasTaxiChanged();
    //! Emitted if the bicyle spots state has been changed.
    void hasBicylceSpotsChanged();
    //! Emitted if the Blue Bike state has been changed.
    void hasBlueBikeChanged();
    //! Emitted if the bus state has been changed.
    void hasBusChanged();
    //! Emitted if the tram state has been changed.
    void hasTramChanged();
    //! Emitted if the metro state has been changed.
    void hasMetroChanged();
    //! Emitted if the wheelchair available state has been changed.
    void hasWheelchairAvailableChanged();
    //! Emitted if the ramp state has been changed.
    void hasRampChanged();
    //! Emitted if the disabled parking spots state has been changed.
    void disabledParkingSpotsChanged();
    //! Emitted if the elevated platform state has been changed.
    void hasElevatedPlatformChanged();
    //! Emitted if the escalator up state has been changed.
    void hasEscalatorUpChanged();
    //! Emitted if the escalator down state has been changed.
    void hasEscalatorDownChanged();
    //! Emitted if the audio induction loop state has been changed.
    void hasAudioInductionLoopChanged();
    //! Emitted if the opening hours are changed.
    void openingHoursChanged();
    //! Emitted if the average stop times has been changed.
    void averageStopTimesChanged();
    //! Emitted if the official transfer times are changed.
    void officialTransferTimesChanged();
    //! Emitted if the platforms are changed.
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
    bool m_hasAudioInductionLoop;
    QMap<StationEngine::Station::Day, QPair<QTime, QTime>> m_openingHours;
    qreal m_averageStopTimes;
    quint32 m_officialTransferTimes;
    QMap<QUrl, QString> m_platforms;

    Q_ENUM(Day)
};
}
}

#endif // CSASTATION_H
