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
#ifndef CSAPLANNERTEST_H
#define CSAPLANNERTEST_H

#include "engines/router/routerplanner.h"
#include <QtCore/QDateTime>
#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>
#include <QtTest/QSignalSpy>
#include <QtTest/QtTest>
#define REPEAT_COUNT 10

namespace QRail {
namespace RouterEngine {
class PlannerTest : public QObject
{
    Q_OBJECT
private slots:
    void initCSAPlannerTest();
    void runCSAPlannerTest();
    void cleanCSAPlannerTest();

public slots:
    void processRoutesFinished(QRail::RouterEngine::Journey *journey);
    void processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route);
    void processing(const QUrl &pageURI);
    void requested(const QUrl &pageURI);

private:
    QRail::RouterEngine::Planner *planner;
    QRail::RouterEngine::Journey *journey;
};
} // namespace RouterEngine
} // namespace QRail

#endif // CSAPLANNERTEST_H
