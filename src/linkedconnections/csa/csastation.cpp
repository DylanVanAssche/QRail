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

#include "csastation.h"

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Station constructor: empty
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::Station with the given parent.
 */
CSA::Station::Station(QObject *parent)
{
    // Clean up when parent dies
    this->setParent(parent);
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Station constructor: minimal
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const qreal &averageStopTimes
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::Station without facilities.
 */
CSA::Station::Station(const QUrl &uri, const QMap<QLocale::Language, QString> &name, const QLocale::Country &country, const QGeoCoordinate &position, const qreal &averageStopTimes, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    // Unknown fields are set to a default value to avoid undefined references
    m_uri = uri;
    m_name = name;
    m_country = country;
    m_position = position;
    m_address = QGeoAddress();
    m_hasTicketVendingMachine = false;
    m_hasLuggageLockers = false;
    m_hasFreeParking = false;
    m_hasTaxi = false;
    m_hasBicycleSpots = false;
    m_hasBlueBike = false;
    m_hasBus = false;
    m_hasTram = false;
    m_hasMetro = false;
    m_hasWheelchairAvailable = false;
    m_hasRamp = false;
    m_disabledParkingSpots = 0;
    m_hasElevatedPlatform = false;
    m_hasEscalatorUp = false;
    m_hasEscalatorDown = false;
    m_hasElevatorPlatform = false;
    m_hasHearingAidSignal = false;
    m_openingHours = QMap<CSA::Station::Day, QPair<QTime, QTime> >();
    m_averageStopTimes = averageStopTimes;
    m_platforms = QMap<QUrl, QString>();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Station constructor: full
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const QGeoAddress &address
 * @param const bool &hasTicketVendingMachine
 * @param const bool &hasLuggageLockers
 * @param const bool &hasFreeParking
 * @param const bool &hasTaxi
 * @param const bool &hasBicyclSpots
 * @param const bool &hasBus
 * @param const bool &hasTram
 * @param const bool &hasMetro
 * @param const bool &hasWheelchairAvailable
 * @param const bool &hasRamp
 * @param const qint16 &disabledParkingSpots
 * @param const bool &hasElevatedPlatform
 * @param const bool &hasEscalatorUp
 * @param const bool &hasEscalatorDown
 * @param const bool &hasElevatorPlatform
 * @param const bool &hasHearingAidSignal
 * @param const QMap<CSA::Station::Day, QPair<QTime, QTime>> &openingHours
 * @param const qreal &averageStopTimes
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::Station with facilities.
 */
CSA::Station::Station(const QUrl &uri, const QMap<QLocale::Language, QString> &name, const QLocale::Country &country, const QGeoCoordinate &position, const QGeoAddress &address, const bool &hasTicketVendingMachine, const bool &hasLuggageLockers, const bool &hasFreeParking, const bool &hasTaxi, const bool &hasBicycleSpots, const bool &hasBlueBike, const bool &hasBus, const bool &hasTram, const bool &hasMetro, const bool &hasWheelchairAvailable, const bool &hasRamp, const qint16 &disabledParkingSpots, const bool &hasElevatedPlatform, const bool &hasEscalatorUp, const bool &hasEscalatorDown, const bool &hasElevatorPlatform, const bool &hasHearingAidSignal, const QMap<CSA::Station::Day, QPair<QTime, QTime> > &openingHours, const qreal &averageStopTimes, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_uri = uri;
    m_name = name;
    m_country = country;
    m_position = position;
    m_address = address;
    m_hasTicketVendingMachine = hasTicketVendingMachine;
    m_hasLuggageLockers = hasLuggageLockers;
    m_hasFreeParking = hasFreeParking;
    m_hasTaxi = hasTaxi;
    m_hasBicycleSpots = hasBicycleSpots;
    m_hasBlueBike = hasBlueBike;
    m_hasBus = hasBus;
    m_hasTram = hasTram;
    m_hasMetro = hasMetro;
    m_hasWheelchairAvailable = hasWheelchairAvailable;
    m_hasRamp = hasRamp;
    m_disabledParkingSpots = disabledParkingSpots;
    m_hasElevatedPlatform = hasElevatedPlatform;
    m_hasEscalatorUp = hasEscalatorUp;
    m_hasEscalatorDown = hasEscalatorDown;
    m_hasElevatorPlatform = hasElevatorPlatform;
    m_hasHearingAidSignal = hasHearingAidSignal;
    m_openingHours = openingHours;
    m_averageStopTimes = averageStopTimes;
    m_platforms = QMap<QUrl, QString>();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Station constructor: minimal
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const qreal &averageStopTimes
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::Station without facilities.
 */
CSA::Station::Station(const QUrl &uri, const QMap<QLocale::Language, QString> &name, const QLocale::Country &country, const QGeoCoordinate &position, const qreal &averageStopTimes, const QMap<QUrl, QString> &platforms, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    // Unknown fields are set to a default value to avoid undefined references
    m_uri = uri;
    m_name = name;
    m_country = country;
    m_position = position;
    m_address = QGeoAddress();
    m_hasTicketVendingMachine = false;
    m_hasLuggageLockers = false;
    m_hasFreeParking = false;
    m_hasTaxi = false;
    m_hasBicycleSpots = false;
    m_hasBlueBike = false;
    m_hasBus = false;
    m_hasTram = false;
    m_hasMetro = false;
    m_hasWheelchairAvailable = false;
    m_hasRamp = false;
    m_disabledParkingSpots = 0;
    m_hasElevatedPlatform = false;
    m_hasEscalatorUp = false;
    m_hasEscalatorDown = false;
    m_hasElevatorPlatform = false;
    m_hasHearingAidSignal = false;
    m_openingHours = QMap<CSA::Station::Day, QPair<QTime, QTime> >();
    m_averageStopTimes = averageStopTimes;
    m_platforms = platforms;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief CSA::Station constructor: full
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const QGeoAddress &address
 * @param const bool &hasTicketVendingMachine
 * @param const bool &hasLuggageLockers
 * @param const bool &hasFreeParking
 * @param const bool &hasTaxi
 * @param const bool &hasBicyclSpots
 * @param const bool &hasBus
 * @param const bool &hasTram
 * @param const bool &hasMetro
 * @param const bool &hasWheelchairAvailable
 * @param const bool &hasRamp
 * @param const qint16 &disabledParkingSpots
 * @param const bool &hasElevatedPlatform
 * @param const bool &hasEscalatorUp
 * @param const bool &hasEscalatorDown
 * @param const bool &hasElevatorPlatform
 * @param const bool &hasHearingAidSignal
 * @param const QMap<CSA::Station::Day, QPair<QTime, QTime>> &openingHours
 * @param const QList<QPair<QUrl, QString> > &platforms
 * @param const qreal &averageStopTimes
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a CSA::Station with facilities and platforms.
 */
CSA::Station::Station(const QUrl &uri, const QMap<QLocale::Language, QString> &name, const QLocale::Country &country, const QGeoCoordinate &position, const QGeoAddress &address, const bool &hasTicketVendingMachine, const bool &hasLuggageLockers, const bool &hasFreeParking, const bool &hasTaxi, const bool &hasBicycleSpots, const bool &hasBlueBike, const bool &hasBus, const bool &hasTram, const bool &hasMetro, const bool &hasWheelchairAvailable, const bool &hasRamp, const qint16 &disabledParkingSpots, const bool &hasElevatedPlatform, const bool &hasEscalatorUp, const bool &hasEscalatorDown, const bool &hasElevatorPlatform, const bool &hasHearingAidSignal, const QMap<CSA::Station::Day, QPair<QTime, QTime> > &openingHours, const qreal &averageStopTimes, const QMap<QUrl, QString> &platforms, QObject *parent) : QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_uri = uri;
    m_name = name;
    m_country = country;
    m_position = position;
    m_address = address;
    m_hasTicketVendingMachine = hasTicketVendingMachine;
    m_hasLuggageLockers = hasLuggageLockers;
    m_hasFreeParking = hasFreeParking;
    m_hasTaxi = hasTaxi;
    m_hasBicycleSpots = hasBicycleSpots;
    m_hasBlueBike = hasBlueBike;
    m_hasBus = hasBus;
    m_hasTram = hasTram;
    m_hasMetro = hasMetro;
    m_hasWheelchairAvailable = hasWheelchairAvailable;
    m_hasRamp = hasRamp;
    m_disabledParkingSpots = disabledParkingSpots;
    m_hasElevatedPlatform = hasElevatedPlatform;
    m_hasEscalatorUp = hasEscalatorUp;
    m_hasEscalatorDown = hasEscalatorDown;
    m_hasElevatorPlatform = hasElevatorPlatform;
    m_hasHearingAidSignal = hasHearingAidSignal;
    m_openingHours = openingHours;
    m_averageStopTimes = averageStopTimes;
    m_platforms = platforms;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @package CSA
 * @return QUrl URI
 * @public
 * Gets the URI of the station and returns it.
 */
QUrl CSA::Station::uri() const
{
    return m_uri;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the URI
 * @package CSA
 * @param const QUrl &uri
 * @public
 * Sets the URI of the station to the given QUrl &url.
 */
void CSA::Station::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the name
 * @package CSA
 * @return const QMap<QLocale::Language, QString> name
 * @public
 * Gets the name of the station and returns it.
 */
QMap<QLocale::Language, QString> CSA::Station::name() const
{
    return m_name;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the name
 * @package CSA
 * @param const QMap<QLocale::Language, QString> &name
 * @public
 * Sets the name of the station to the given QMap<QLocale::Language, QString> &name.
 */
void CSA::Station::setName(const QMap<QLocale::Language, QString> &name)
{
    m_name = name;
    emit this->nameChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the country
 * @package CSA
 * @return const QLocale::Country country
 * @public
 * Gets the country of the station and returns it.
 */
QLocale::Country CSA::Station::country() const
{
    return m_country;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the country
 * @package CSA
 * @param const QLocale::Country &country
 * @public
 * Sets the country of the station to the given QLocale::Country &country.
 */
void CSA::Station::setCountry(const QLocale::Country &country)
{
    m_country = country;
    emit this->countryChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the position
 * @package CSA
 * @return const QGeoCoordinate position
 * @public
 * Gets the position of the station and returns it.
 */
QGeoCoordinate CSA::Station::position() const
{
    return m_position;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the position
 * @package CSA
 * @param const QGeoCoordinate &position
 * @public
 * Sets the position of the station to the given QGeoCoordinate &position.
 */
void CSA::Station::setPosition(const QGeoCoordinate &position)
{
    m_position = position;
    emit this->positionChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the address
 * @package CSA
 * @return const QGeoAddress address
 * @public
 * Gets the address of the station and returns it.
 */
QGeoAddress CSA::Station::address() const
{
    return m_address;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the address
 * @package CSA
 * @param const QGeoAddress &address
 * @public
 * Sets the address of the station to the given QGeoAddress &address.
 */
void CSA::Station::setAddress(const QGeoAddress &address)
{
    m_address = address;
    emit this->addressChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasTicketVendingMachine
 * @package CSA
 * @return const bool hasTicketVendingMachine
 * @public
 * Gets the hasTicketVendingMachine and returns it.
 */
bool CSA::Station::hasTicketVendingMachine() const
{
    return m_hasTicketVendingMachine;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasTicketVendingMachine
 * @package CSA
 * @param const bool &hasTicketVendingMachine
 * @public
 * Sets the hasTicketVendingMachine to the given bool &hasTicketVendingMachine.
 */
void CSA::Station::setHasTicketVendingMachine(const bool &hasTicketVendingMachine)
{
    m_hasTicketVendingMachine = hasTicketVendingMachine;
    emit this->hasTicketVendingMachineChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasLuggageLockers
 * @package CSA
 * @return const bool hasLuggageLockers
 * @public
 * Gets the hasLuggageLockers and returns it.
 */
bool CSA::Station::hasLuggageLockers() const
{
    return m_hasLuggageLockers;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasLuggageLockers
 * @package CSA
 * @param const bool &hasLuggageLockers
 * @public
 * Sets the hasLuggageLockers to the given bool &hasLuggageLockers.
 */
void CSA::Station::setHasLuggageLockers(const bool &hasLuggageLockers)
{
    m_hasLuggageLockers = hasLuggageLockers;
    emit this->hasLuggageLockersChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasFreeParking
 * @package CSA
 * @return const bool hasFreeParking
 * @public
 * Gets the hasFreeParking and returns it.
 */
bool CSA::Station::hasFreeParking() const
{
    return m_hasFreeParking;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasFreeParking
 * @package CSA
 * @param const bool &hasFreeParking
 * @public
 * Sets the hasFreeParking to the given bool &hasFreeParking.
 */
void CSA::Station::setHasFreeParking(const bool &hasFreeParking)
{
    m_hasFreeParking = hasFreeParking;
    emit this->hasFreeParkingChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasTaxi
 * @package CSA
 * @return const bool hasTaxi
 * @public
 * Gets the hasTaxi and returns it.
 */
bool CSA::Station::hasTaxi() const
{
    return m_hasTaxi;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasTaxi
 * @package CSA
 * @param const bool &hasTaxi
 * @public
 * Sets the hasTaxi to the given bool &hasTaxi.
 */
void CSA::Station::setHasTaxi(const bool &hasTaxi)
{
    m_hasTaxi = hasTaxi;
    emit this->hasTaxiChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasBicycleSpots
 * @package CSA
 * @return const bool hasBicycleSpots
 * @public
 * Gets the hasBicycleSpots and returns it.
 */
bool CSA::Station::hasBicycleSpots() const
{
    return m_hasBicycleSpots;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasBicycleSpots
 * @package CSA
 * @param const bool &hasBicycleSpots
 * @public
 * Sets the hasBicycleSpots to the given bool &hasBicycleSpots.
 */
void CSA::Station::setHasBicycleSpots(const bool &hasBicycleSpots)
{
    m_hasBicycleSpots = hasBicycleSpots;
    emit this->hasBicylceSpotsChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasBlueBike
 * @package CSA
 * @return const bool hasBlueBike
 * @public
 * Gets the hasBlueBike and returns it.
 */
bool CSA::Station::hasBlueBike() const
{
    return m_hasBlueBike;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasBlueBike
 * @package CSA
 * @param const bool &hasBlueBike
 * @public
 * Sets the hasBlueBike to the given bool &hasBlueBike.
 */
void CSA::Station::setHasBlueBike(const bool &hasBlueBike)
{
    m_hasBlueBike = hasBlueBike;
    emit this->hasBlueBikeChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasBus
 * @package CSA
 * @return const bool hasBus
 * @public
 * Gets the hasBus and returns it.
 */
bool CSA::Station::hasBus() const
{
    return m_hasBus;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasBus
 * @package CSA
 * @param const bool &hasBus
 * @public
 * Sets the hasBus to the given QUrl &url.
 */
void CSA::Station::setHasBus(const bool &hasBus)
{
    m_hasBus = hasBus;
    emit this->hasBusChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasTram
 * @package CSA
 * @return const bool hasTram
 * @public
 * Gets the hasTram and returns it.
 */
bool CSA::Station::hasTram() const
{
    return m_hasTram;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasTram
 * @package CSA
 * @param const bool &hasTram
 * @public
 * Sets the hasTram to the given QUrl &url.
 */
void CSA::Station::setHasTram(const bool &hasTram)
{
    m_hasTram = hasTram;
    emit this->hasTramChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasMetro
 * @package CSA
 * @return const bool hasMetro
 * @public
 * Gets the hasMetro and returns it.
 */
bool CSA::Station::hasMetro() const
{
    return m_hasMetro;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasMetro
 * @package CSA
 * @param const bool &hasMetro
 * @public
 * Sets the hasMetro to the given bool &hasMetro.
 */
void CSA::Station::setHasMetro(const bool &hasMetro)
{
    m_hasMetro = hasMetro;
    emit this->hasMetroChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @package CSA
 * @return const bool hasWheelchairAvailable
 * @public
 * Gets the hasWheelchairAvailable and returns it.
 */
bool CSA::Station::hasWheelchairAvailable() const
{
    return m_hasWheelchairAvailable;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasWheelchairAvailable
 * @package CSA
 * @param const bool &hasWheelchairAvailable
 * @public
 * Sets the hasWheelchairAvailable to the given bool &hasWheelchairAvailable.
 */
void CSA::Station::setHasWheelchairAvailable(const bool &hasWheelchairAvailable)
{
    m_hasWheelchairAvailable = hasWheelchairAvailable;
    emit this->hasWheelchairAvailableChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasRamp
 * @package CSA
 * @return const bool hasRamp
 * @public
 * Gets the hasRamp and returns it.
 */
bool CSA::Station::hasRamp() const
{
    return m_hasRamp;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasRamp
 * @package CSA
 * @param const bool &hasRamp
 * @public
 * Sets the hasRamp to the given bool &hasRamp.
 */
void CSA::Station::setHasRamp(const bool &hasRamp)
{
    m_hasRamp = hasRamp;
    emit this->hasRampChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the number of disabled parking spots
 * @package CSA
 * @return const qint16 disabledParkingSpots
 * @public
 * Gets the disabledParkingSpots and returns it.
 */
qint16 CSA::Station::disabledParkingSpots() const
{
    return m_disabledParkingSpots;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the number of disabled parking spots
 * @package CSA
 * @param const qint16 &disabledParkingSpots
 * @public
 * Sets the disabledParkingSpots to the given qint16 &disabledParkingSpots.
 */
void CSA::Station::setDisabledParkingSpots(const qint16 &disabledParkingSpots)
{
    m_disabledParkingSpots = disabledParkingSpots;
    emit this->disabledParkingSpotsChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasElevatedPlatform
 * @package CSA
 * @return bool hasElevatedPlatform
 * @public
 * Gets the hasElevatedPlatform and returns it.
 */
bool CSA::Station::hasElevatedPlatform() const
{
    return m_hasElevatedPlatform;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasElevatedPlatform
 * @package CSA
 * @param const bool &hasElevatedPlatform
 * @public
 * Sets the hasElevatedPlatform to the given bool &hasElevatedPlatform.
 */
void CSA::Station::setHasElevatedPlatform(const bool &hasElevatedPlatform)
{
    m_hasElevatedPlatform = hasElevatedPlatform;
    emit this->hasElevatedPlatformChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasEscalatorUp
 * @package CSA
 * @return bool hasEscalatorUp
 * @public
 * Gets the hasEscalatorUp and returns it.
 */
bool CSA::Station::hasEscalatorUp() const
{
    return m_hasEscalatorUp;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasEscalatorUp
 * @package CSA
 * @param const bool &hasEscalatorUp
 * @public
 * Sets the hasEscalatorUp to the given bool &hasEscalatorUp.
 * Emit the hasEscalatorUpChanged signal.
 */
void CSA::Station::setHasEscalatorUp(const bool &hasEscalatorUp)
{
    m_hasEscalatorUp = hasEscalatorUp;
    emit this->hasEscalatorUpChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasEscalatorDown
 * @package CSA
 * @return bool hasEscalatorDown
 * @public
 * Gets the hasEscalatorDown and returns it.
 */
bool CSA::Station::hasEscalatorDown() const
{
    return m_hasEscalatorDown;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasEscalatorDown
 * @package CSA
 * @param const bool &hasEscalatorDown
 * @public
 * Sets the hasEscalatorDown to the given bool &hasEscalatorDown.
 * Emit the hasEscalatorDownChanged signal.
 */
void CSA::Station::setHasEscalatorDown(const bool &hasEscalatorDown)
{
    m_hasEscalatorDown = hasEscalatorDown;
    emit this->hasEscalatorDownChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasElevatorPlatform
 * @package CSA
 * @return bool hasElevatorPlatform
 * @public
 * Gets the hasElevatorPlatform and returns it.
 */
bool CSA::Station::hasElevatorPlatform() const
{
    return m_hasElevatorPlatform;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasElevatorPlatform
 * @package CSA
 * @param const bool &hasElevatorPlatform
 * @public
 * Sets the hasElevatorPlatform to the given bool &hasElevatorPlatform.
 */
void CSA::Station::setHasElevatorPlatform(const bool &hasElevatorPlatform)
{
    m_hasElevatorPlatform = hasElevatorPlatform;
    emit this->hasElevatedPlatformChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasHearingAidSignal
 * @package CSA
 * @return bool hasHearingAidSignal
 * @public
 * Gets the hasHearingAidSignal and returns it.
 */
bool CSA::Station::hasHearingAidSignal() const
{
    return m_hasHearingAidSignal;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasHearingAidSignal
 * @package CSA
 * @param const bool &hasHearingAidSignal
 * @public
 * Sets the hasHearingAidSignal to the given bool &hasHearingAidSignal.
 * Emit the hasHearingAidSignalChanged signal.
 */
void CSA::Station::setHasHearingAidSignal(const bool &hasHearingAidSignal)
{
    m_hasHearingAidSignal = hasHearingAidSignal;
    emit this->hasHearingAidSignalChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @package CSA
 * @return QMap<Station::Day, QPair<QTime, QTime> > openingHours
 * @public
 * Gets the openingHours and returns it.
 */
QMap<CSA::Station::Day, QPair<QTime, QTime> > CSA::Station::openingHours() const
{
    return m_openingHours;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the openingHours
 * @package CSA
 * @param const QMap<Station::Day, QPair<QTime, QTime> > &openingHours
 * @public
 * Sets the openinghours to the given QMap<Station::Day, QPair<QTime, QTime> > &openingHours.
 * Emits the openingHoursChanged signal.
 */
void CSA::Station::setOpeningHours(const QMap<Station::Day, QPair<QTime, QTime> > &openingHours)
{
    m_openingHours = openingHours;
    emit this->openingHoursChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the average stop times
 * @package CSA
 * @return qreal averageStopTimes
 * @public
 * Gets the average stop times and returns it.
 */
qreal CSA::Station::averageStopTimes() const
{
    return m_averageStopTimes;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the average stop times
 * @package CSA
 * @param const qreal &averageStopTimes
 * @public
 * Sets the average stop times to the given qreal &averageStopTimes.
 * Emits the averageStopTimesChanged signal.
 */
void CSA::Station::setAverageStopTimes(const qreal &averageStopTimes)
{
    m_averageStopTimes = averageStopTimes;
    emit this->averageStopTimesChanged();
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platforms for the station
 * @package CSA
 * @return const QMap<QUrl, QString> platforms
 * @public
 * Gets the platforms for the station and returns it.
 */
QMap<QUrl, QString> CSA::Station::platforms() const
{
    return m_platforms;
}

/**
 * @file csastation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets platforms for the station
 * @package CSA
 * @param const QMap<QUrl, QString> &platforms
 * @public
 * Sets the platforms for the station to the given QMap<QUrl, QString> &platforms.
 * Emits the platformsChanged signal.
 */
void CSA::Station::setPlatforms(const QMap<QUrl, QString> &platforms)
{
    m_platforms = platforms;
    emit this->platformsChanged();
}
