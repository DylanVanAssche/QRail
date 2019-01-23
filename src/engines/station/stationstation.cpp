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

StationEngine::Station::Station(QObject *parent): QObject(parent)
{

}

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

QUrl StationEngine::Station::uri() const
{
    return m_uri;
}

void StationEngine::Station::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

QMap<QLocale::Language, QString> StationEngine::Station::name() const
{
    return m_name;
}

void StationEngine::Station::setName(const QMap<QLocale::Language, QString> &name)
{
    m_name = name;
    emit this->nameChanged();
}

QLocale::Country StationEngine::Station::country() const
{
    return m_country;
}

void StationEngine::Station::setCountry(const QLocale::Country &country)
{
    m_country = country;
    emit this->countryChanged();
}

QGeoCoordinate StationEngine::Station::position() const
{
    return m_position;
}

void StationEngine::Station::setPosition(const QGeoCoordinate &position)
{
    m_position = position;
    emit this->positionChanged();
}

QGeoAddress StationEngine::Station::address() const
{
    return m_address;
}

void StationEngine::Station::setAddress(const QGeoAddress &address)
{
    m_address = address;
    emit this->addressChanged();
}

bool StationEngine::Station::hasTicketVendingMachine() const
{
    return m_hasTicketVendingMachine;
}

void StationEngine::Station::setHasTicketVendingMachine(const bool &hasTicketVendingMachine)
{
    m_hasTicketVendingMachine = hasTicketVendingMachine;
    emit this->hasTicketVendingMachineChanged();
}

bool StationEngine::Station::hasLuggageLockers() const
{
    return m_hasLuggageLockers;
}

void StationEngine::Station::setHasLuggageLockers(const bool &hasLuggageLockers)
{
    m_hasLuggageLockers = hasLuggageLockers;
    emit this->hasLuggageLockersChanged();
}

bool StationEngine::Station::hasFreeParking() const
{
    return m_hasFreeParking;
}

void StationEngine::Station::setHasFreeParking(const bool &hasFreeParking)
{
    m_hasFreeParking = hasFreeParking;
    emit this->hasFreeParkingChanged();
}

bool StationEngine::Station::hasTaxi() const
{
    return m_hasTaxi;
}

void StationEngine::Station::setHasTaxi(const bool &hasTaxi)
{
    m_hasTaxi = hasTaxi;
    emit this->hasTaxiChanged();
}

bool StationEngine::Station::hasBicycleSpots() const
{
    return m_hasBicycleSpots;
}

void StationEngine::Station::setHasBicycleSpots(const bool &hasBicycleSpots)
{
    m_hasBicycleSpots = hasBicycleSpots;
    emit this->hasBicylceSpotsChanged();
}

bool StationEngine::Station::hasBlueBike() const
{
    return m_hasBlueBike;
}

void StationEngine::Station::setHasBlueBike(const bool &hasBlueBike)
{
    m_hasBlueBike = hasBlueBike;
    emit this->hasBlueBikeChanged();
}

bool StationEngine::Station::hasBus() const
{
    return m_hasBus;
}

void StationEngine::Station::setHasBus(const bool &hasBus)
{
    m_hasBus = hasBus;
    emit this->hasBusChanged();
}

bool StationEngine::Station::hasTram() const
{
    return m_hasTram;
}

void StationEngine::Station::setHasTram(const bool &hasTram)
{
    m_hasTram = hasTram;
    emit this->hasTramChanged();
}

bool StationEngine::Station::hasMetro() const
{
    return m_hasMetro;
}

void StationEngine::Station::setHasMetro(const bool &hasMetro)
{
    m_hasMetro = hasMetro;
    emit this->hasMetroChanged();
}

bool StationEngine::Station::hasWheelchairAvailable() const
{
    return m_hasWheelchairAvailable;
}

void StationEngine::Station::setHasWheelchairAvailable(const bool &hasWheelchairAvailable)
{
    m_hasWheelchairAvailable = hasWheelchairAvailable;
    emit this->hasWheelchairAvailableChanged();
}

bool StationEngine::Station::hasRamp() const
{
    return m_hasRamp;
}

void StationEngine::Station::setHasRamp(const bool &hasRamp)
{
    m_hasRamp = hasRamp;
    emit this->hasRampChanged();
}

qint16 StationEngine::Station::disabledParkingSpots() const
{
    return m_disabledParkingSpots;
}

void StationEngine::Station::setDisabledParkingSpots(const qint16 &disabledParkingSpots)
{
    m_disabledParkingSpots = disabledParkingSpots;
    emit this->disabledParkingSpotsChanged();
}

bool StationEngine::Station::hasElevatedPlatform() const
{
    return m_hasElevatedPlatform;
}

void StationEngine::Station::setHasElevatedPlatform(const bool &hasElevatedPlatform)
{
    m_hasElevatedPlatform = hasElevatedPlatform;
    emit this->hasElevatedPlatformChanged();
}

bool StationEngine::Station::hasEscalatorUp() const
{
    return m_hasEscalatorUp;
}

void StationEngine::Station::setHasEscalatorUp(const bool &hasEscalatorUp)
{
    m_hasEscalatorUp = hasEscalatorUp;
    emit this->hasEscalatorUpChanged();
}

bool StationEngine::Station::hasEscalatorDown() const
{
    return m_hasEscalatorDown;
}

void StationEngine::Station::setHasEscalatorDown(const bool &hasEscalatorDown)
{
    m_hasEscalatorDown = hasEscalatorDown;
    emit this->hasEscalatorDownChanged();
}

bool StationEngine::Station::hasElevatorPlatform() const
{
    return m_hasElevatorPlatform;
}

void StationEngine::Station::setHasElevatorPlatform(const bool &hasElevatorPlatform)
{
    m_hasElevatorPlatform = hasElevatorPlatform;
    emit this->hasElevatedPlatformChanged();
}

bool StationEngine::Station::hasAudioInductionLoop() const
{
    return m_hasAudioInductionLoop;
}

void StationEngine::Station::setHasAudioInductionLoop(const bool &hasAudioInductionLoop)
{
    m_hasAudioInductionLoop = hasAudioInductionLoop;
    emit this->hasAudioInductionLoopChanged();
}

QMap<StationEngine::Station::Day, QPair<QTime, QTime> > StationEngine::Station::openingHours() const
{
    return m_openingHours;
}

void StationEngine::Station::setOpeningHours(const QMap<Station::Day, QPair<QTime, QTime> >
                                             &openingHours)
{
    m_openingHours = openingHours;
    emit this->openingHoursChanged();
}

qreal StationEngine::Station::averageStopTimes() const
{
    return m_averageStopTimes;
}

void StationEngine::Station::setAverageStopTimes(const qreal &averageStopTimes)
{
    m_averageStopTimes = averageStopTimes;
    emit this->averageStopTimesChanged();
}

quint32 QRail::StationEngine::Station::officialTransferTimes() const
{
    return m_officialTransferTimes;
}

void QRail::StationEngine::Station::setOfficialTransferTimes(const quint32 &officialTransferTimes)
{
    m_officialTransferTimes = officialTransferTimes;
    emit this->officialTransferTimesChanged();
}

QMap<QUrl, QString> StationEngine::Station::platforms() const
{
    return m_platforms;
}

void StationEngine::Station::setPlatforms(const QMap<QUrl, QString> &platforms)
{
    m_platforms = platforms;
    emit this->platformsChanged();
}
