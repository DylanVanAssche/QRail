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

#ifndef LIVEBOARDBOARD_H
#define LIVEBOARDBOARD_H

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QDateTime>
#include <algorithm> // std::sort C++

#include "engines/station/stationstation.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "fragments/fragmentspage.h"

namespace QRail {
namespace LiveboardEngine {
/**
 * @class Board
 * @brief An LiveboardEngine::Board contains a liveboard.
 * A liveboard holds all the departing or arriving vehicles (depending on the mode) in a certain station.
 */
class Board : public QObject
{
    Q_OBJECT
public:
    //! The different modes in which the Board can operate.
    enum class Mode {
        ARRIVALS,
        DEPARTURES,
    };
    //! QRail::LiveboardEngine::Board constructor: empty.
    /*!
        \param parent QObject parent-child memory management.
        \public
        Constructs a QRail::LiveboardEngine::Board with the given parent.
     */
    explicit Board(QObject *parent = nullptr);
    //! QRail::LiveboardEngine::Board constructor: full.
    /*!
        \param entries a list of entries.
        \param station the associated station.
        \param from the departure time.
        \param until the arrival time.
        \param parent QObject parent-child memory management.
        \public
        Constructs a QRail::LiveboardEngine::Board with the given parent.
     */
    explicit Board(const QList<QRail::VehicleEngine::Vehicle *> &entries,
                   StationEngine::Station *station,
                   const QDateTime &from,
                   const QDateTime &until,
                   QObject *parent = nullptr);
    //! Adds an entry to the Board.
    /*!
        \param entry a QRail::VehicleEngine::Vehicle entry.
        \public
        Adds an entry (QRail::VehicleEngine::Vehicle *) to the Liveboard entries.<br>
        Using this method will automatically set the entries to the
        appended list and trigger the entriesChanged signal.
     */
    void addEntry(QRail::VehicleEngine::Vehicle *entry);
    //! Gets the entries of the Board.
    /*!
        \return entries a QList<QRail::VehicleEngine::Vehicle *> of the Board entries.
        \public
        Gets the list of entries and returns them.
     */
    QList<QRail::VehicleEngine::Vehicle *> entries() const;
    //! Sets the entries of the Board.
    /*!
        \param entries a QList<QRail::VehicleEngine::Vehicle *> of the Board entries.
        \public
        Sets the list of entries to the given QList<QRail::VehicleEngine::Vehicle *entries.
     */
    void setEntries(const QList<QRail::VehicleEngine::Vehicle *> &entries);
    //! Gets the station of the Board.
    /*!
        \return station StationEngine::Station object with information about the associated station.
        \public
        Gets the station of the Board and returns it.
     */
    StationEngine::Station *station() const;
    //! Sets the station of the Board.
    /*!
        \param station StationEngine::Station object with information about the associated station.
        \public
        Sets the station of the liveboard to the given StationEngine::Station *station.
     */
    void setStation(StationEngine::Station *station);
    //! Gets from.
    /*!
        \return from the departure timestamp in UTC.
        \public
        Gets the departure time and returns it.
     */
    QDateTime from() const;
    //! Sets from.
    /*!
        \param from the departure timestamp in UTC.
        \public
        Sets the departure timestamp in UTC to the given const QDateTime &from.
     */
    void setFrom(const QDateTime &from);
    //! Gets until.
    /*!
        \return until the arrival timestamp in UTC.
        \public
        Gets the arrival time and returns it.
     */
    QDateTime until() const;
    //! Sets until.
    /*!
        \param until the arrival timestamp in UTC.
        \public
        Sets the arrival timestamp in UTC to the given const QDateTime &until.
     */
    void setUntil(const QDateTime &until);
    //! Gets the mode.
    /*!
        \return mode Board::Mode state.
        \public
        Gets the current Board::Mode mode for this Board and returns it.
     */
    QRail::LiveboardEngine::Board::Mode mode() const;
    //! Sets the mode.
    /*!
        \param mode Board::Mode state.
        \public
        Sets the current Board::Mode mode for this Board to the given const QRail::LiveboardEngine::Board::Mode &mode.
     */
    void setMode(const QRail::LiveboardEngine::Board::Mode &mode);
    //! Gets hydraPrevious.
    /*!
        \return hydraPrevious the previous hydra URL.
        \public
        Gets the previous hydra URL and returns it.
     */
    QUrl hydraPrevious() const;
    //! Sets hydraPrevious.
    /*!
        \param hydraPrevious the previous hydra URL.
        \public
        Sets the previous hydra URL and returns it.
     */
    void setHydraPrevious(const QUrl &hydraPrevious);
    //! Gets hydraNext.
    /*!
        \return hydraNext the next hydra URL.
        \public
        Gets the next hydra URL and returns it.
     */
    QUrl hydraNext() const;
    //! Sets hydraNext.
    /*!
        \param hydraNext the next hydra URL.
        \public
        Sets the next hydra URL and returns it.
     */
    void setHydraNext(const QUrl &hydraNext);

signals:
    //! Emitted when the entries are changed.
    void entriesChanged();
    //! Emitted when the station has been changed.
    void stationChanged();
    //! Emitted when from has been changed.
    void fromChanged();
    //! Emitted when until has been changed.
    void untilChanged();
    //! Emitted when the mode has been changed.
    void modeChanged();
    //! Emitted when the hydraPrevious has been changed.
    void hydraPreviousChanged();
    //! Emitted when the hydraNext has been changed.
    void hydraNextChanged();

private:
    QList<QRail::VehicleEngine::Vehicle *> m_entries;
    QRail::StationEngine::Station *m_station;
    QDateTime m_from;
    QDateTime m_until;
    QRail::LiveboardEngine::Board::Mode m_mode;
    QUrl m_hydraPrevious;
    QUrl m_hydraNext;

    Q_ENUM(Mode)
};
} // namespace LiveboardEngine
} // namespace QRail

#endif // LIVEBOARDBOARD_H
