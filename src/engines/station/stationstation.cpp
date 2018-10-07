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
#include "engines/station/stationstation.h"
using namespace QRail;

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: empty
 * @param QObject *parent
 * @package StationEngine
 * @public
 * Constructs a StationEngine::Station with the given parent.
 */
StationEngine::Station::Station(QObject *parent): QObject(parent)
{

}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: minimal
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const qreal &averageStopTimes
 * @param const quint32 &officialTransferTimes
 * @param QObject *parent
 * @package StationEngine
 * @public
 * Constructs a StationEngine::Station without facilities.
 */
StationEngine::Station::Station(const QUrl &uri,
                                const QMap<QLocale::Language,
                                QString> &name,
                                const QLocale::Country &country,
                                const QGeoCoordinate &position,
                                const qreal &averageStopTimes,
                                const quint32 &officialTransferTimes,
                                QObject *parent) : QObject(parent)
{
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
    m_hasAudioInductionLoop = false;
    m_openingHours = QMap<StationEngine::Station::Day, QPair<QTime, QTime> >();
    m_averageStopTimes = averageStopTimes;
    m_officialTransferTimes = officialTransferTimes;
    m_platforms = QMap<QUrl, QString>();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: full
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
 * @param const bool &hasAudioInductionLoop
 * @param const QMap<StationEngine::Station::Day, QPair<QTime, QTime>> &openingHours
 * @param const qreal &averageStopTimes
 * @param const quint32 &officialTransferTimes
 * @param QObject *parent
 * @package StationEngine
 * @public
 * Constructs a StationEngine::Station with facilities.
 */
StationEngine::Station::Station(const QUrl &uri,
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
                                const QMap<StationEngine::Station::Day, QPair<QTime, QTime> > &openingHours,
                                const qreal &averageStopTimes,
                                const quint32 &officialTransferTimes,
                                QObject *parent) : QObject(parent)
{
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
    m_hasAudioInductionLoop = hasAudioInductionLoop;
    m_openingHours = openingHours;
    m_averageStopTimes = averageStopTimes;
    m_officialTransferTimes = officialTransferTimes;
    m_platforms = QMap<QUrl, QString>();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: minimal
 * @param const QUrl &uri
 * @param const QMap<QLocale::Language, QString> &name
 * @param const QLocale::Country &country
 * @param const QGeoCoordinate &position
 * @param const qreal &averageStopTimes
 * @param const quint32 &officialTransferTimes
 * @param QObject *parent
 * @package StationEngine
 * @public
 * Constructs a StationEngine::Station without facilities.
 */
StationEngine::Station::Station(const QUrl &uri,
                                const QMap<QLocale::Language, QString> &name,
                                const QLocale::Country &country,
                                const QGeoCoordinate &position,
                                const qreal &averageStopTimes,
                                const quint32 &officialTransferTimes,
                                const QMap<QUrl, QString> &platforms,
                                QObject *parent) : QObject(parent)
{
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
    m_hasAudioInductionLoop = false;
    m_openingHours = QMap<StationEngine::Station::Day, QPair<QTime, QTime> >();
    m_averageStopTimes = averageStopTimes;
    m_officialTransferTimes = officialTransferTimes;
    m_platforms = platforms;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Station constructor: full
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
 * @param const bool &hasAudioInductionLoop
 * @param const QMap<StationEngine::Station::Day, QPair<QTime, QTime>> &openingHours
 * @param const QList<QPair<QUrl, QString> > &platforms
 * @param const qreal &averageStopTimes
 * @param const quint32 &officialTransferTimes
 * @param QObject *parent
 * @package StationEngine
 * @public
 * Constructs a StationEngine::Station with facilities and platforms.
 */
StationEngine::Station::Station(const QUrl &uri,
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
                                const QMap<StationEngine::Station::Day, QPair<QTime, QTime> > &openingHours,
                                const qreal &averageStopTimes,
                                const quint32 &officialTransferTimes,
                                const QMap<QUrl, QString> &platforms,
                                QObject *parent) : QObject(parent)
{
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
    m_hasAudioInductionLoop = hasAudioInductionLoop;
    m_openingHours = openingHours;
    m_averageStopTimes = averageStopTimes;
    m_officialTransferTimes = officialTransferTimes;
    m_platforms = platforms;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @package StationEngine
 * @return QUrl URI
 * @public
 * Gets the URI of the station and returns it.
 */
QUrl StationEngine::Station::uri() const
{
    return m_uri;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the URI
 * @package StationEngine
 * @param const QUrl &uri
 * @public
 * Sets the URI of the station to the given QUrl &url.
 */
void StationEngine::Station::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the name
 * @package StationEngine
 * @return const QMap<QLocale::Language, QString> name
 * @public
 * Gets the name of the station and returns it.
 */
QMap<QLocale::Language, QString> StationEngine::Station::name() const
{
    return m_name;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the name
 * @package StationEngine
 * @param const QMap<QLocale::Language, QString> &name
 * @public
 * Sets the name of the station to the given QMap<QLocale::Language, QString> &name.
 */
void StationEngine::Station::setName(const QMap<QLocale::Language, QString> &name)
{
    m_name = name;
    emit this->nameChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the country
 * @package StationEngine
 * @return const QLocale::Country country
 * @public
 * Gets the country of the station and returns it.
 */
QLocale::Country StationEngine::Station::country() const
{
    return m_country;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the country
 * @package StationEngine
 * @param const QLocale::Country &country
 * @public
 * Sets the country of the station to the given QLocale::Country &country.
 */
void StationEngine::Station::setCountry(const QLocale::Country &country)
{
    m_country = country;
    emit this->countryChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the position
 * @package StationEngine
 * @return const QGeoCoordinate position
 * @public
 * Gets the position of the station and returns it.
 */
QGeoCoordinate StationEngine::Station::position() const
{
    return m_position;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the position
 * @package StationEngine
 * @param const QGeoCoordinate &position
 * @public
 * Sets the position of the station to the given QGeoCoordinate &position.
 */
void StationEngine::Station::setPosition(const QGeoCoordinate &position)
{
    m_position = position;
    emit this->positionChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the address
 * @package StationEngine
 * @return const QGeoAddress address
 * @public
 * Gets the address of the station and returns it.
 */
QGeoAddress StationEngine::Station::address() const
{
    return m_address;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the address
 * @package StationEngine
 * @param const QGeoAddress &address
 * @public
 * Sets the address of the station to the given QGeoAddress &address.
 */
void StationEngine::Station::setAddress(const QGeoAddress &address)
{
    m_address = address;
    emit this->addressChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasTicketVendingMachine
 * @package StationEngine
 * @return const bool hasTicketVendingMachine
 * @public
 * Gets the hasTicketVendingMachine and returns it.
 */
bool StationEngine::Station::hasTicketVendingMachine() const
{
    return m_hasTicketVendingMachine;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasTicketVendingMachine
 * @package StationEngine
 * @param const bool &hasTicketVendingMachine
 * @public
 * Sets the hasTicketVendingMachine to the given bool &hasTicketVendingMachine.
 */
void StationEngine::Station::setHasTicketVendingMachine(const bool &hasTicketVendingMachine)
{
    m_hasTicketVendingMachine = hasTicketVendingMachine;
    emit this->hasTicketVendingMachineChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasLuggageLockers
 * @package StationEngine
 * @return const bool hasLuggageLockers
 * @public
 * Gets the hasLuggageLockers and returns it.
 */
bool StationEngine::Station::hasLuggageLockers() const
{
    return m_hasLuggageLockers;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasLuggageLockers
 * @package StationEngine
 * @param const bool &hasLuggageLockers
 * @public
 * Sets the hasLuggageLockers to the given bool &hasLuggageLockers.
 */
void StationEngine::Station::setHasLuggageLockers(const bool &hasLuggageLockers)
{
    m_hasLuggageLockers = hasLuggageLockers;
    emit this->hasLuggageLockersChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasFreeParking
 * @package StationEngine
 * @return const bool hasFreeParking
 * @public
 * Gets the hasFreeParking and returns it.
 */
bool StationEngine::Station::hasFreeParking() const
{
    return m_hasFreeParking;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasFreeParking
 * @package StationEngine
 * @param const bool &hasFreeParking
 * @public
 * Sets the hasFreeParking to the given bool &hasFreeParking.
 */
void StationEngine::Station::setHasFreeParking(const bool &hasFreeParking)
{
    m_hasFreeParking = hasFreeParking;
    emit this->hasFreeParkingChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasTaxi
 * @package StationEngine
 * @return const bool hasTaxi
 * @public
 * Gets the hasTaxi and returns it.
 */
bool StationEngine::Station::hasTaxi() const
{
    return m_hasTaxi;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasTaxi
 * @package StationEngine
 * @param const bool &hasTaxi
 * @public
 * Sets the hasTaxi to the given bool &hasTaxi.
 */
void StationEngine::Station::setHasTaxi(const bool &hasTaxi)
{
    m_hasTaxi = hasTaxi;
    emit this->hasTaxiChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasBicycleSpots
 * @package StationEngine
 * @return const bool hasBicycleSpots
 * @public
 * Gets the hasBicycleSpots and returns it.
 */
bool StationEngine::Station::hasBicycleSpots() const
{
    return m_hasBicycleSpots;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasBicycleSpots
 * @package StationEngine
 * @param const bool &hasBicycleSpots
 * @public
 * Sets the hasBicycleSpots to the given bool &hasBicycleSpots.
 */
void StationEngine::Station::setHasBicycleSpots(const bool &hasBicycleSpots)
{
    m_hasBicycleSpots = hasBicycleSpots;
    emit this->hasBicylceSpotsChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasBlueBike
 * @package StationEngine
 * @return const bool hasBlueBike
 * @public
 * Gets the hasBlueBike and returns it.
 */
bool StationEngine::Station::hasBlueBike() const
{
    return m_hasBlueBike;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasBlueBike
 * @package StationEngine
 * @param const bool &hasBlueBike
 * @public
 * Sets the hasBlueBike to the given bool &hasBlueBike.
 */
void StationEngine::Station::setHasBlueBike(const bool &hasBlueBike)
{
    m_hasBlueBike = hasBlueBike;
    emit this->hasBlueBikeChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasBus
 * @package StationEngine
 * @return const bool hasBus
 * @public
 * Gets the hasBus and returns it.
 */
bool StationEngine::Station::hasBus() const
{
    return m_hasBus;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasBus
 * @package StationEngine
 * @param const bool &hasBus
 * @public
 * Sets the hasBus to the given QUrl &url.
 */
void StationEngine::Station::setHasBus(const bool &hasBus)
{
    m_hasBus = hasBus;
    emit this->hasBusChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasTram
 * @package StationEngine
 * @return const bool hasTram
 * @public
 * Gets the hasTram and returns it.
 */
bool StationEngine::Station::hasTram() const
{
    return m_hasTram;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasTram
 * @package StationEngine
 * @param const bool &hasTram
 * @public
 * Sets the hasTram to the given QUrl &url.
 */
void StationEngine::Station::setHasTram(const bool &hasTram)
{
    m_hasTram = hasTram;
    emit this->hasTramChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasMetro
 * @package StationEngine
 * @return const bool hasMetro
 * @public
 * Gets the hasMetro and returns it.
 */
bool StationEngine::Station::hasMetro() const
{
    return m_hasMetro;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasMetro
 * @package StationEngine
 * @param const bool &hasMetro
 * @public
 * Sets the hasMetro to the given bool &hasMetro.
 */
void StationEngine::Station::setHasMetro(const bool &hasMetro)
{
    m_hasMetro = hasMetro;
    emit this->hasMetroChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @package StationEngine
 * @return const bool hasWheelchairAvailable
 * @public
 * Gets the hasWheelchairAvailable and returns it.
 */
bool StationEngine::Station::hasWheelchairAvailable() const
{
    return m_hasWheelchairAvailable;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasWheelchairAvailable
 * @package StationEngine
 * @param const bool &hasWheelchairAvailable
 * @public
 * Sets the hasWheelchairAvailable to the given bool &hasWheelchairAvailable.
 */
void StationEngine::Station::setHasWheelchairAvailable(const bool &hasWheelchairAvailable)
{
    m_hasWheelchairAvailable = hasWheelchairAvailable;
    emit this->hasWheelchairAvailableChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasRamp
 * @package StationEngine
 * @return const bool hasRamp
 * @public
 * Gets the hasRamp and returns it.
 */
bool StationEngine::Station::hasRamp() const
{
    return m_hasRamp;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasRamp
 * @package StationEngine
 * @param const bool &hasRamp
 * @public
 * Sets the hasRamp to the given bool &hasRamp.
 */
void StationEngine::Station::setHasRamp(const bool &hasRamp)
{
    m_hasRamp = hasRamp;
    emit this->hasRampChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the number of disabled parking spots
 * @package StationEngine
 * @return const qint16 disabledParkingSpots
 * @public
 * Gets the disabledParkingSpots and returns it.
 */
qint16 StationEngine::Station::disabledParkingSpots() const
{
    return m_disabledParkingSpots;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the number of disabled parking spots
 * @package StationEngine
 * @param const qint16 &disabledParkingSpots
 * @public
 * Sets the disabledParkingSpots to the given qint16 &disabledParkingSpots.
 */
void StationEngine::Station::setDisabledParkingSpots(const qint16 &disabledParkingSpots)
{
    m_disabledParkingSpots = disabledParkingSpots;
    emit this->disabledParkingSpotsChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasElevatedPlatform
 * @package StationEngine
 * @return bool hasElevatedPlatform
 * @public
 * Gets the hasElevatedPlatform and returns it.
 */
bool StationEngine::Station::hasElevatedPlatform() const
{
    return m_hasElevatedPlatform;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasElevatedPlatform
 * @package StationEngine
 * @param const bool &hasElevatedPlatform
 * @public
 * Sets the hasElevatedPlatform to the given bool &hasElevatedPlatform.
 */
void StationEngine::Station::setHasElevatedPlatform(const bool &hasElevatedPlatform)
{
    m_hasElevatedPlatform = hasElevatedPlatform;
    emit this->hasElevatedPlatformChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasEscalatorUp
 * @package StationEngine
 * @return bool hasEscalatorUp
 * @public
 * Gets the hasEscalatorUp and returns it.
 */
bool StationEngine::Station::hasEscalatorUp() const
{
    return m_hasEscalatorUp;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasEscalatorUp
 * @package StationEngine
 * @param const bool &hasEscalatorUp
 * @public
 * Sets the hasEscalatorUp to the given bool &hasEscalatorUp.
 * Emit the hasEscalatorUpChanged signal.
 */
void StationEngine::Station::setHasEscalatorUp(const bool &hasEscalatorUp)
{
    m_hasEscalatorUp = hasEscalatorUp;
    emit this->hasEscalatorUpChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasEscalatorDown
 * @package StationEngine
 * @return bool hasEscalatorDown
 * @public
 * Gets the hasEscalatorDown and returns it.
 */
bool StationEngine::Station::hasEscalatorDown() const
{
    return m_hasEscalatorDown;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasEscalatorDown
 * @package StationEngine
 * @param const bool &hasEscalatorDown
 * @public
 * Sets the hasEscalatorDown to the given bool &hasEscalatorDown.
 * Emit the hasEscalatorDownChanged signal.
 */
void StationEngine::Station::setHasEscalatorDown(const bool &hasEscalatorDown)
{
    m_hasEscalatorDown = hasEscalatorDown;
    emit this->hasEscalatorDownChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasElevatorPlatform
 * @package StationEngine
 * @return bool hasElevatorPlatform
 * @public
 * Gets the hasElevatorPlatform and returns it.
 */
bool StationEngine::Station::hasElevatorPlatform() const
{
    return m_hasElevatorPlatform;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasElevatorPlatform
 * @package StationEngine
 * @param const bool &hasElevatorPlatform
 * @public
 * Sets the hasElevatorPlatform to the given bool &hasElevatorPlatform.
 */
void StationEngine::Station::setHasElevatorPlatform(const bool &hasElevatorPlatform)
{
    m_hasElevatorPlatform = hasElevatorPlatform;
    emit this->hasElevatedPlatformChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the hasAudioInductionLoop
 * @package StationEngine
 * @return bool hasAudioInductionLoop
 * @public
 * Gets the hasAudioInductionLoop and returns it.
 */
bool StationEngine::Station::hasAudioInductionLoop() const
{
    return m_hasAudioInductionLoop;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the hasAudioInductionLoop
 * @package StationEngine
 * @param const bool &hasAudioInductionLoop
 * @public
 * Sets the hasAudioInductionLoop to the given bool &hasAudioInductionLoop.
 * Emit the hasAudioInductionLoopChanged signal.
 */
void StationEngine::Station::setHasAudioInductionLoop(const bool &hasAudioInductionLoop)
{
    m_hasAudioInductionLoop = hasAudioInductionLoop;
    emit this->hasAudioInductionLoopChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI
 * @package StationEngine
 * @return QMap<Station::Day, QPair<QTime, QTime> > openingHours
 * @public
 * Gets the openingHours and returns it.
 */
QMap<StationEngine::Station::Day, QPair<QTime, QTime> > StationEngine::Station::openingHours() const
{
    return m_openingHours;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the openingHours
 * @package StationEngine
 * @param const QMap<Station::Day, QPair<QTime, QTime> > &openingHours
 * @public
 * Sets the openinghours to the given QMap<Station::Day, QPair<QTime, QTime> > &openingHours.
 * Emits the openingHoursChanged signal.
 */
void StationEngine::Station::setOpeningHours(const QMap<Station::Day, QPair<QTime, QTime> >
                                             &openingHours)
{
    m_openingHours = openingHours;
    emit this->openingHoursChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the average stop times
 * @package StationEngine
 * @return qreal averageStopTimes
 * @public
 * Gets the average stop times and returns it.
 */
qreal StationEngine::Station::averageStopTimes() const
{
    return m_averageStopTimes;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the average stop times
 * @package StationEngine
 * @param const qreal &averageStopTimes
 * @public
 * Sets the average stop times to the given qreal &averageStopTimes.
 * Emits the averageStopTimesChanged signal.
 */
void StationEngine::Station::setAverageStopTimes(const qreal &averageStopTimes)
{
    m_averageStopTimes = averageStopTimes;
    emit this->averageStopTimesChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Gets the official transfer times
 * @package StationEngine
 * @return quint32 officialTransferTimes
 * @public
 * Gets the official transfer times and returns it.
 */
quint32 QRail::StationEngine::Station::officialTransferTimes() const
{
    return m_officialTransferTimes;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 07 Oct 2018
 * @brief Sets the official transfer times
 * @package StationEngine
 * @param const quint32 &officialTransferTimes
 * @public
 * Sets the official transfer times to the given quint32 &officialTransferTimes.
 * Emits the officialTransferTimesChanged signal.
 */
void QRail::StationEngine::Station::setOfficialTransferTimes(const quint32 &officialTransferTimes)
{
    m_officialTransferTimes = officialTransferTimes;
    emit this->officialTransferTimesChanged();
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the platforms for the station
 * @package StationEngine
 * @return const QMap<QUrl, QString> platforms
 * @public
 * Gets the platforms for the station and returns it.
 */
QMap<QUrl, QString> StationEngine::Station::platforms() const
{
    return m_platforms;
}

/**
 * @file stationstation.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets platforms for the station
 * @package StationEngine
 * @param const QMap<QUrl, QString> &platforms
 * @public
 * Sets the platforms for the station to the given QMap<QUrl, QString> &platforms.
 * Emits the platformsChanged signal.
 */
void StationEngine::Station::setPlatforms(const QMap<QUrl, QString> &platforms)
{
    m_platforms = platforms;
    emit this->platformsChanged();
}
