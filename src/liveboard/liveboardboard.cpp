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

#include "liveboard/liveboardboard.h"

/**
 * @file liveboarboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Liveboard::Board constructor: empty
 * @param QObject *parent
 * @package Liveboard
 * @public
 * Constructs a Liveboard::Board with the given parent.
 */
Liveboard::Board::Board(QObject *parent) : QObject(parent)
{

}

/**
 * @file liveboarboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Liveboard::Board constructor: full
 * @param const QList<CSA::VehicleStop *> &entries
 * @param CSA::Station *station
 * @param const QDateTime &from
 * @param const QDateTime &until
 * @param QObject *parent
 * @package Liveboard
 * @public
 * Constructs a Liveboard::Board with the given parent.
 */
Liveboard::Board::Board(const QList<CSA::VehicleStop *> &entries, CSA::Station *station, const QDateTime &from, const QDateTime &until, QObject *parent): QObject(parent)
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
 * @param CSA::VehicleStop *entry
 * @package Liveboard
 * @public
 * Adds an entry (CSA::VehicleStop *) to the Liveboard entries.
 * Using this method will automatically set the entries to the
 * appended list and trigger the entriesChanged signal.
 */
void Liveboard::Board::addEntry(CSA::VehicleStop *entry)
{
    QList<CSA::VehicleStop *> entries = this->entries();
    entries.append(entry);
    this->setEntries(entries);
}

// Getters & Setters
/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the station
 * @return CSA::Station *station
 * @package Liveboard
 * @public
 * Gets the station of the liveboard and returns it.
 */
CSA::Station *Liveboard::Board::station() const
{
    return m_station;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the station
 * @param CSA::Station *station
 * @package Liveboard
 * @public
 * Sets the station of the liveboard to the given CSA::Station *station.
 * Emits the stationChanged signal.
 */
void Liveboard::Board::setStation(CSA::Station *station)
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
QDateTime Liveboard::Board::until() const
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
void Liveboard::Board::setUntil(const QDateTime &until)
{
    m_until = until;
    emit this->untilChanged();
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the mode
 * @return const Liveboard::Board::Mode mode
 * @package Liveboard
 * @public
 * Gets the mode and returns it.
 */
Liveboard::Board::Mode Liveboard::Board::mode() const
{
    return m_mode;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the mode
 * @param const Liveboard::Board::Mode &mode
 * @package Liveboard
 * @public
 * Sets the mode of the liveboard to the given Liveboard::Board::Mode &mode.
 * Emits the modeChanged signal.
 */
void Liveboard::Board::setMode(const Liveboard::Board::Mode &mode)
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
QDateTime Liveboard::Board::from() const
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
void Liveboard::Board::setFrom(const QDateTime &from)
{
    m_from = from;
    emit this->fromChanged();
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Gets the entries
 * @return const QList<CSA::VehicleStop *> entries
 * @package Liveboard
 * @public
 * Gets the entries of the liveboard and returns it.
 */
QList<CSA::VehicleStop *> Liveboard::Board::entries() const
{
    return m_entries;
}

/**
 * @file liveboardboard.cpp
 * @author Dylan Van Assche
 * @date 21 Aug 2018
 * @brief Sets the entries
 * @param const QList<CSA::VehicleStop *> &entries
 * @package Liveboard
 * @public
 * Sets the entries of the liveboard to the given QList<CSA::VehicleStop *> &entries.
 * Emits the entriesChanged signal.
 */
void Liveboard::Board::setEntries(const QList<CSA::VehicleStop *> &entries)
{
    m_entries = entries;
    emit this->entriesChanged();
}
