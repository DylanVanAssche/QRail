/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of QRail.                                               *
 *                                                                            *
 *   QRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   QRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with QRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#ifndef LINKEDCONNECTIONPAGETEST_H
#define LINKEDCONNECTIONPAGETEST_H

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include "fragments/fragmentspage.h"

namespace Fragments {
class PageTest: public QObject
{
    Q_OBJECT
private slots:
    void initLinkedConnectionPageTest();
    void runLinkedConnectionPageTest();
    void cleanLinkedConnectionPageTest();

private:
    Fragments::Page *page;
};
}

#endif // LINKEDCONNECTIONPAGETEST_H