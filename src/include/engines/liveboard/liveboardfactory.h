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
#ifndef LIVEBOARDFACTORY_H
#define LIVEBOARDFACTORY_H

#include <QtCore/QDateTime>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtCore/QtGlobal>
#include <QtCore/QTimer>

#include "engines/liveboard/liveboardboard.h"
#include "engines/liveboard/liveboardnullboard.h"
#include "engines/station/stationfactory.h"
#include "engines/vehicle/vehiclestop.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "fragments/fragmentsfactory.h"
#include "fragments/fragmentsfragment.h"
#include "fragments/fragmentspage.h"

#define SECONDS_TO_HOURS_MULTIPLIER 3600 // 3600 seconds = 1 hour
#define MINIMUM_PROGRESS_INCREMENT 1.0   // 1.0 = 1%

namespace QRail {
namespace LiveboardEngine {
/**
 * @class Factory
 * @brief An LiveboardEngine::Factory allows you to generate LiveboardEngine::Board objects.
 * The factory design pattern allows you to create Board objects in an easy way. Several modes are available to fetch your Board.
 */
class Factory : public QObject
{
    Q_OBJECT
public:
    //! Gets a QRail::LiveboardEngine::Factory instance.
    /*!
        \param QObject *parent = nullptr
        \return QRail::LiveboardEngine::Factory *factory
        \public
        Constructs a QRail::LiveboardEngine::Factory if none exists and returns the
        instance.
     */
    static QRail::LiveboardEngine::Factory *getInstance();
    //! Retrieves a liveboard by a station URI.
    /*!
        \param const QUrl &url
        \param const QRail::LiveboardEngine::Board::Mode &mode
        \overload
        \public
        Retrieves a QRail::LiveboardEngine::Board for a station given by it's URI.
        The QRail::LiveboardEngine::Board::Mode &mode parameter determines if the the
        liveboard should contain all the arrivals, departures of the station.
        Calling this method will retrieve a QRail::LiveboardEngine::Board for the
        current time until the current time + 0.5 hour.
     */
    void getLiveboardByStationURI(const QUrl &uri,
                                  const QRail::LiveboardEngine::Board::Mode &mode = QRail::LiveboardEngine::Board::Mode::DEPARTURES);
    //! Retrieves a liveboard by a station URI and a time range.
    /*!
        \param const QUrl &url
        \param const QDateTime &from
        \param const QDateTime &until
        \param const QRail::LiveboardEngine::Board::Mode &mode
        \package Liveboard
        \overload
        \public
        Retrieves a QRail::LiveboardEngine::Board for a station given by it's URI.
        The QRail::LiveboardEngine::Board::Mode &mode parameter determines if the the
        liveboard should contain all the arrivals, departures of the station.
     */
    void getLiveboardByStationURI(const QUrl &uri, const QDateTime &from,
                                  const QDateTime &until,
                                  const QRail::LiveboardEngine::Board::Mode &mode = QRail::LiveboardEngine::Board::Mode::DEPARTURES);
    //! Extend the liveboard with next results.
    /*!
        \param board a QRail::LiveboardEngine::Board instance which we want to extend.
        \public
        You can extend a generated Board with next results by supplying the board to this method.
        The method will search until at least 1 next result has been found.
     */
    void getNextResultsForLiveboard(QRail::LiveboardEngine::Board *board);
    //! Extend the liveboard with previous results.
    /*!
        \param board a QRail::LiveboardEngine::Board instance which we want to extend.
        \public
        You can extend a generated Board with previous results by supplying the board to this method.
        The method will search until at least 1 previous result has been found.
     */
    void getPreviousResultsForLiveboard(QRail::LiveboardEngine::Board *board);
    //! Cancel the current operation.
    /*!
        \public
        If the user wants to cancel the current operation,
        this method must be called before performing a new action.
     */
    void abortCurrentOperation();
    //! Gets from.
    /*!
        \return the departure timestamp in UTC.
        \public
        Gets the departure time and returns it.
     */
    QDateTime from() const;
    //! Gets until.
    /*!
        \return the arrival timestamp in UTC.
        \public
        Gets the arrival time and returns it.
     */
    QDateTime until() const;
    //! Gets the station URI.
    /*!
        \return The URI of the station
        \public
        Gets the URI of the station and returns it.
     */
    QUrl stationURI() const;
    //! Gets the current mode.
    /*!
        \return The mode of the factory
        \public
        Gets the current mode of the factory and returns it.
     */
    QRail::LiveboardEngine::Board::Mode mode() const;

protected:
    virtual void customEvent(QEvent *event);

signals:
    void fromChanged();
    void untilChanged();
    void stationURIChanged();
    void modeChanged();
    void error(const QString &message);
    void requested(const QUrl &uri);
    void processing(const QUrl &uri);
    void finished(QRail::LiveboardEngine::Board *liveboard);
    void stream(QRail::VehicleEngine::Vehicle *entry);

private slots:
    void unlockLiveboard();
    void handleTimeout();
    void handleFragmentFactoryError();

private:
    enum class Direction {
        PREVIOUS,
        NEXT,
    };
    QTimer *progressTimeoutTimer;
    mutable QMutex liveboardProcessingMutex;
    mutable QMutex liveboardAccessMutex;
    mutable QMutex liveboardExtendingMutex;
    QList<QRail::Fragments::Page *> m_usedPages;
    QRail::LiveboardEngine::Board *m_liveboard;
    QDateTime m_from;
    QDateTime m_until;
    QUrl m_stationURI;
    QRail::LiveboardEngine::Board::Mode m_mode;
    static QRail::LiveboardEngine::Factory *m_instance;
    QRail::Fragments::Factory *m_fragmentsFactory;
    StationEngine::Factory *m_stationFactory;
    QRail::Fragments::Factory *fragmentsFactory() const;
    StationEngine::Factory *stationFactory() const;
    bool m_isExtending;
    bool m_abortRequested;
    QRail::LiveboardEngine::Factory::Direction m_extendingDirection;
    void processPage(QRail::Fragments::Page *page);
    void setStationFactory(StationEngine::Factory *stationFactory);
    void parsePage(QRail::Fragments::Page *page, bool &finished);
    void setFragmentsFactory(QRail::Fragments::Factory *fragmentsFactory);
    void setMode(const QRail::LiveboardEngine::Board::Mode &mode);
    void setStationURI(const QUrl &stationURI);
    void setUntil(const QDateTime &until);
    void setFrom(const QDateTime &from);
    void setLiveboard(QRail::LiveboardEngine::Board *liveboard);
    void initUsedPages();
    void addUsedPage(QRail::Fragments::Page *page);
    void deleteUsedPages();
    bool isExtending() const;
    void setIsExtending(bool isExtending);
    bool isAbortRequested() const;
    void setAbortRequested(bool abortRequested);
    QRail::LiveboardEngine::Board *liveboard() const;
    explicit Factory(QObject *parent = nullptr);
};
} // namespace LiveboardEngine
} // namespace QRail

#endif // LIVEBOARDFACTORY_H
