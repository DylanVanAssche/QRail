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
#ifndef LINKEDCONNECTIONFRAGMENTTEST_H
#define LINKEDCONNECTIONFRAGMENTTEST_H

#include "fragments/fragmentsfragment.h"
#include <QObject>
#include <QSignalSpy>
#include <QtTest/QtTest>

namespace QRail {
namespace Fragments {
class FragmentTest : public QObject {
  Q_OBJECT
private slots:
  void initLinkedConnectionFragment();
  void runLinkedConnectionFragment();
  void cleanLinkedConnectionFragment();

private:
  QRail::Fragments::Fragment *lcWithoutDelay;
  QRail::Fragments::Fragment *lcWithDelay;
};
} // namespace Fragments
} // namespace QRail

#endif // LINKEDCONNECTIONFRAGMENTTEST_H
