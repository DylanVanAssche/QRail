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

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtConcurrent/QtConcurrent>

#include "engines/liveboard/liveboardboard.h"
#include "fragments/fragmentsfactory.h"
#include "fragments/fragmentspage.h"
#include "fragments/fragmentsfragment.h"
#include "engines/station/stationfactory.h"
#include "engines/vehicle/vehiclevehicle.h"
#include "engines/vehicle/vehiclestop.h"

#define SECONDS_TO_HOURS_MULTIPLIER 3600 // 3600 seconds = 1 hour
#define MINIMUM_PROGRESS_INCREMENT 1.0 // 1.0 = 1%

namespace LiveboardEngine {
class Factory : public QObject
{
    Q_OBJECT
public:
    static LiveboardEngine::Factory *getInstance();
    void getLiveboardByStationURI(
            const QUrl &uri,
            const LiveboardEngine::Board::Mode &mode = LiveboardEngine::Board::Mode::DEPARTURES
            );
    void getLiveboardByStationURI(
            const QUrl &uri,
            const QDateTime &from,
            const QDateTime &until,
            const LiveboardEngine::Board::Mode &mode = LiveboardEngine::Board::Mode::DEPARTURES
            );
    QDateTime from() const;
    QDateTime until() const;
    QUrl stationURI() const;
    LiveboardEngine::Board::Mode mode() const;
    LiveboardEngine::Board *liveboard() const;

signals:
    void liveboardReady(LiveboardEngine::Board *liveboard);
    void fromChanged();
    void untilChanged();
    void stationURIChanged();
    void modeChanged();
    void error(const QString &message);
    void pageReceived(const QUrl &uri);
    void pageRequested(const QUrl &uri);
    void pageProgress(const QUrl &pageURI, const qint16 &progress);

private slots:
    void pageReceived(Fragments::Page *page);

private:
    mutable QMutex liveboardAccessMutex;
    LiveboardEngine::Board *m_liveboard;
    QDateTime m_from;
    QDateTime m_until;
    QUrl m_stationURI;
    LiveboardEngine::Board::Mode m_mode;
    static LiveboardEngine::Factory *m_instance;
    Fragments::Factory *m_fragmentsFactory;
    StationEngine::Factory *m_stationFactory;
    Fragments::Factory *fragmentsFactory() const;
    StationEngine::Factory *stationFactory() const;
    void setStationFactory(StationEngine::Factory *stationFactory);
    void parsePage(Fragments::Page *page, const bool &finished);
    void setFragmentsFactory(Fragments::Factory *fragmentsFactory);
    void setMode(const LiveboardEngine::Board::Mode &mode);
    void setStationURI(const QUrl &stationURI);
    void setUntil(const QDateTime &until);
    void setFrom(const QDateTime &from);
    void setLiveboard(LiveboardEngine::Board *liveboard);
    explicit Factory(QObject *parent = nullptr);
};
}

#endif // LIVEBOARDFACTORY_H
