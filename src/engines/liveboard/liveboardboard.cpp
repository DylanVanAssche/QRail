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
#include "engines/liveboard/liveboardboard.h"
using namespace QRail;

/**
 * @file liveboarboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief QRail::LiveboardEngine::Board constructor: empty
 * @param QObject *parent
 * @package Liveboard
 * @public
 * Constructs a QRail::LiveboardEngine::Board with the given parent.
 */
QRail::LiveboardEngine::Board::Board(QObject *parent) : QObject(parent)
{

}

/**
 * @file liveboarboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief QRail::LiveboardEngine::Board constructor: full
 * @param const QList<QRail::VehicleEngine::Vehicle *> &entries
 * @param StationEngine::Station *station
 * @param const QDateTime &from
 * @param const QDateTime &until
 * @param QObject *parent
 * @package Liveboard
 * @public
 * Constructs a QRail::LiveboardEngine::Board with the given parent.
 */
QRail::LiveboardEngine::Board::Board(const QList<QRail::VehicleEngine::Vehicle *> &entries, StationEngine::Station *station, const QDateTime &from, const QDateTime &until, QObject *parent): QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_entries = entries;
    m_station = station;
    m_from = from;
    m_until = until;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Add an entry
 * @param QRail::VehicleEngine::Vehicle *entry
 * @package Liveboard
 * @public
 * Adds an entry (QRail::VehicleEngine::Vehicle *) to the Liveboard entries.
 * Using this method will automatically set the entries to the
 * appended list and trigger the entriesChanged signal.
 */
void QRail::LiveboardEngine::Board::addEntry(QRail::VehicleEngine::Vehicle *entry)
{
    QList<QRail::VehicleEngine::Vehicle *> entries = this->entries();
    entries.append(entry);
    this->setEntries(entries);
}

// Getters & Setters
/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the station
 * @return StationEngine::Station *station
 * @package Liveboard
 * @public
 * Gets the station of the liveboard and returns it.
 */
StationEngine::Station *QRail::LiveboardEngine::Board::station() const
{
    return m_station;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the station
 * @param StationEngine::Station *station
 * @package Liveboard
 * @public
 * Sets the station of the liveboard to the given StationEngine::Station *station.
 * Emits the stationChanged signal.
 */
void QRail::LiveboardEngine::Board::setStation(StationEngine::Station *station)
{
    m_station = station;
    emit this->stationChanged();
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the until time
 * @return const QDateTime until
 * @package Liveboard
 * @public
 * Gets the until time and returns it.
 */
QDateTime QRail::LiveboardEngine::Board::until() const
{
    return m_until;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the until time
 * @param const QDateTime &until
 * @package Liveboard
 * @public
 * Sets the until time to the given QDateTime &until.
 * Emits the untilChanged signal.
 */
void QRail::LiveboardEngine::Board::setUntil(const QDateTime &until)
{
    m_until = until;
    emit this->untilChanged();
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the mode
 * @return const QRail::LiveboardEngine::Board::Mode mode
 * @package Liveboard
 * @public
 * Gets the mode and returns it.
 */
QRail::LiveboardEngine::Board::Mode QRail::LiveboardEngine::Board::mode() const
{
    return m_mode;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the mode
 * @param const QRail::LiveboardEngine::Board::Mode &mode
 * @package Liveboard
 * @public
 * Sets the mode of the liveboard to the given QRail::LiveboardEngine::Board::Mode &mode.
 * Emits the modeChanged signal.
 */
void QRail::LiveboardEngine::Board::setMode(const QRail::LiveboardEngine::Board::Mode &mode)
{
    m_mode = mode;
    emit this->modeChanged();
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the from time
 * @return const QDateTime from
 * @package Liveboard
 * @public
 * Gets the from time and returns it.
 */
QDateTime QRail::LiveboardEngine::Board::from() const
{
    return m_from;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the from time
 * @param const QDateTime &from
 * @package Liveboard
 * @public
 * Sets the from time to the given QDateTime &from.
 * Emits the fromChanged signal.
 */
void QRail::LiveboardEngine::Board::setFrom(const QDateTime &from)
{
    m_from = from;
    emit this->fromChanged();
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the entries
 * @return const QList<QRail::VehicleEngine::Vehicle *> entries
 * @package Liveboard
 * @public
 * Gets the entries of the liveboard and returns it.
 */
QList<QRail::VehicleEngine::Vehicle *> QRail::LiveboardEngine::Board::entries() const
{
    return m_entries;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the entries
 * @param const QList<QRail::VehicleEngine::Vehicle *> &entries
 * @package Liveboard
 * @public
 * Sets the entries of the liveboard to the given QList<QRail::VehicleEngine::Vehicle *> &entries.
 * Emits the entriesChanged signal.
 */
void QRail::LiveboardEngine::Board::setEntries(const QList<QRail::VehicleEngine::Vehicle *> &entries)
{
    m_entries = entries;
    emit this->entriesChanged();
}
