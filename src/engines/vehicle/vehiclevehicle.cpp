/*
 * Copyright (C) 2018 by Dylan Van Assche
 *
 * This file is part of QRail.
 *
 *   QRail is free software: you can redistribute it and/or modify it
 *   under the terms of the GNU Lesser General Public License as published
 *   by the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   QRail is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public
 *   License along with QRail.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "engines/vehicle/vehiclevehicle.h"
using namespace QRail;

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief QRail::VehicleEngine::Vehicle constructor empty
 * @param QObject *parent
 * @package VehicleEngine
 * @public
 * Constructs a QRail::VehicleEngine::Vehicle to store information about the vehicle itself.
 */
QRail::VehicleEngine::Vehicle::Vehicle(QObject *parent): QObject(parent)
{

}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief QRail::VehicleEngine::Vehicle constructor full
 * @param const QUrl &uri
 * @param const QUrl &tripURI
 * @param const QString &headsign
 * @param QObject *parent
 * @package VehicleEngine
 * @public
 * Constructs a QRail::VehicleEngine::Vehicle to store information about the vehicle itself without intermediary stops.
 */
QRail::VehicleEngine::Vehicle::Vehicle(const QUrl &uri, const QUrl &tripURI, const QString &headsign, QObject *parent): QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_uri = uri;
    m_tripURI = tripURI;
    m_headsign = headsign;
    m_intermediaryStops = QList<QRail::VehicleEngine::Stop *>();
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief QRail::VehicleEngine::Vehicle constructor full
 * @param const QUrl &uri
 * @param const QUrl &tripURI
 * @param const QString &headsign
 * @param const QList<QRail::VehicleEngine::Stop *> &intermediaryStops
 * @param QObject *parent
 * @package VehicleEngine
 * @public
 * Constructs a QRail::VehicleEngine::Vehicle to store information about the vehicle itself with intermediary stops.
 */
QRail::VehicleEngine::Vehicle::Vehicle(const QUrl &uri, const QUrl &tripURI, const QString &headsign, const QList<QRail::VehicleEngine::Stop *> &intermediaryStops, QObject *parent) : QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_uri = uri;
    m_tripURI = tripURI;
    m_headsign = headsign;
    m_intermediaryStops = intermediaryStops;
}

// Getters & Setters
/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the URI of the vehicle
 * @return const QUrl uri
 * @package VehicleEngine
 * @public
 * Gets the URI of the vehicle and returns it.
 */
QUrl QRail::VehicleEngine::Vehicle::uri() const
{
    return m_uri;
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the URI of the vehicle
 * @param const QUrl &uri
 * @package VehicleEngine
 * @public
 * Sets the URI of the vehicle to the given QUrl &uri
 * Emit the uriChanged signal.
 */
void QRail::VehicleEngine::Vehicle::setUri(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 24 Aug 2018
 * @brief Gets the URI of the current trip
 * @return const QUrl tripURI
 * @package VehicleEngine
 * @public
 * Gets the URI of the current trip and returns it.
 */
QUrl QRail::VehicleEngine::Vehicle::tripURI() const
{
    return m_tripURI;
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 24 Aug 2018
 * @brief Sets the URI of the current trip
 * @param const QUrl &tripURI
 * @package VehicleEngine
 * @public
 * Sets the URI of the current trip to the given QUrl &tripURI
 * Emit the tripURIChanged signal.
 */
void QRail::VehicleEngine::Vehicle::setTripURI(const QUrl &tripURI)
{
    m_tripURI = tripURI;
    emit this->tripURIChanged();
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the headsign of the vehicle
 * @return const QString &headsign
 * @package VehicleEngine
 * @public
 * Gets the headsign of the vehicle and returns it.
 */
QString QRail::VehicleEngine::Vehicle::headsign() const
{
    return m_headsign;
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the headsign of the vehicle
 * @param const QString &headsign
 * @package VehicleEngine
 * @public
 * Sets the headsign of the vehicle to the given QString &headsign
 * Emit the headsignChanged signal.
 */
void QRail::VehicleEngine::Vehicle::setHeadsign(const QString &headsign)
{
    m_headsign = headsign;
    emit this->headsignChanged();
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 24 Aug 2018
 * @brief Gets the intermediary stops of the vehicle
 * @return QList<QRail::VehicleEngine::Stop *> &intermediaryStops
 * @package VehicleEngine
 * @public
 * Gets the intermediary stops of the vehicle and returns it.
 */
QList<QRail::VehicleEngine::Stop *> QRail::VehicleEngine::Vehicle::intermediaryStops() const
{
    return m_intermediaryStops;
}

/**
 * @file vehiclevehicle.cpp
 * @author Dylan Van Assche
 * @date 24 Aug 2018
 * @brief Sets the intermediary stops of the vehicle
 * @param const QList<QRail::VehicleEngine::Stop *> &intermediaryStops
 * @package VehicleEngine
 * @public
 * Sets the intermediary stops of the vehicle to the given QList<QRail::VehicleEngine::Stop *> &intermediaryStops.
 * Emit the intermediaryStopsChanged signal.
 */
void QRail::VehicleEngine::Vehicle::setIntermediaryStops(const QList<QRail::VehicleEngine::Stop *> &intermediaryStops)
{
    m_intermediaryStops = intermediaryStops;
    emit this->intermediaryStopsChanged();
}
