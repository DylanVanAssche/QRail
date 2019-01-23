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
#include "engines/station/stationnullstation.h"
using namespace QRail;
StationEngine::NullStation *StationEngine::NullStation::m_instance = nullptr;

StationEngine::NullStation::NullStation(const QUrl &uri,
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
                                        QObject *parent) :
    Station(uri, name, country, position, address, hasTicketVendingMachine, hasLuggageLockers,
            hasFreeParking, hasTaxi, hasBicycleSpots, hasBlueBike, hasBus, hasTram, hasMetro,
            hasWheelchairAvailable, hasRamp, disabledParkingSpots, hasElevatedPlatform, hasEscalatorDown,
            hasEscalatorUp, hasElevatorPlatform, hasAudioInductionLoop, openingHours, averageStopTimes,
            officialTransferTimes, parent)
{

}

StationEngine::NullStation *StationEngine::NullStation::getInstance()
{
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new NullStation";
        m_instance = new StationEngine::NullStation(
            QUrl(),
            QMap<QLocale::Language, QString>(),
            QLocale::Country::Belgium,
            QGeoCoordinate(),
            QGeoAddress(),
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            false,
            0,
            false,
            false,
            false,
            false,
            false,
            QMap<StationEngine::Station::Day, QPair<QTime, QTime> >(),
            0.0,
            0
        );
    }
    return m_instance;
}
