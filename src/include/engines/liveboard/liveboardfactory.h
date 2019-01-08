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
class Factory : public QObject
{
    Q_OBJECT
public:
    enum class Direction {
        PREVIOUS,
        NEXT,
    };
    static QRail::LiveboardEngine::Factory *getInstance();
    void
    getLiveboardByStationURI(const QUrl &uri,
                             const QRail::LiveboardEngine::Board::Mode &mode =
                                 QRail::LiveboardEngine::Board::Mode::DEPARTURES);
    void
    getLiveboardByStationURI(const QUrl &uri, const QDateTime &from,
                             const QDateTime &until,
                             const QRail::LiveboardEngine::Board::Mode &mode =
                                 QRail::LiveboardEngine::Board::Mode::DEPARTURES);
    void getNextResultsForLiveboard(QRail::LiveboardEngine::Board *board);
    void getPreviousResultsForLiveboard(QRail::LiveboardEngine::Board *board);
    void abortCurrentOperation();
    QDateTime from() const;
    QDateTime until() const;
    QUrl stationURI() const;
    QRail::LiveboardEngine::Board::Mode mode() const;
    QRail::LiveboardEngine::Board *liveboard() const;

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
    explicit Factory(QObject *parent = nullptr);
};
} // namespace LiveboardEngine
} // namespace QRail

#endif // LIVEBOARDFACTORY_H
