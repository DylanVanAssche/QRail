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
#ifndef LIVEBOARDFACTORYTEST_H
#define LIVEBOARDFACTORYTEST_H

#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtTest/QSignalSpy>
#include <QtTest/QtTest>

#include "engines/liveboard/liveboardfactory.h"

namespace QRail {
namespace LiveboardEngine {
class FactoryTest : public QObject
{
public slots:
    void liveboardReceived(QRail::LiveboardEngine::Board *board);
    void liveboardStreamReceived(QRail::VehicleEngine::Vehicle *vehicle);

private slots:
    void initLiveboardFactoryTest();
    void runLiveboardFactoryTest();
    void cleanLiveboardFactoryTest();

private:
    QRail::LiveboardEngine::Factory *factory;
    QRail::LiveboardEngine::Board *liveboard;
};
} // namespace LiveboardEngine
} // namespace QRail

#endif // LIVEBOARDFACTORYTEST_H
