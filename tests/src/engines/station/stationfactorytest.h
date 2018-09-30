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
#ifndef STATIONFACTORYTEST_H
#define STATIONFACTORYTEST_H

#include <QtCore/QMetaType>
#include <QtCore/QObject>
#include <QtTest/QSignalSpy>
#include <QtTest/QtTest>

#include "engines/station/stationfactory.h"

namespace QRail {
namespace StationEngine {
class FactoryTest : public QObject
{
    Q_OBJECT
private slots:
    void initStationFactoryTest();
    void runStationFactoryTest();
    void cleanStationFactoryTest();

private:
    QRail::StationEngine::Factory *factory;
};
} // namespace StationEngine
} // namespace QRail

#endif // STATIONFACTORYTEST_H
