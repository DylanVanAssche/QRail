#
#   This file is part of QRail.
#
#   QRail is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
#
#   QRail is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with QRail.  If not, see <http://www.gnu.org/licenses/>.
#

TARGET = qrail-tests

# QMake config
CONFIG += sailfishapp \
        c++11

# Qt modules
QT += core \
    network \
    positioning \
    concurrent \
    sql \
    testlib

# QRail library build location
CONFIG(debug, debug|release) {
    QRAIL_LOCATION = $$PWD/../build/debug
}
else {
    QRAIL_LOCATION = $$PWD/../build/release
}
LIBS += $$QRAIL_LOCATION/libqrail.a

## Headers include path of the QRail library
INCLUDEPATH += $$PWD/../src/include \
            $$PWD/../qtcsv/include

SOURCES += src/qrail-tests.cpp \
    src/database/databasemanagertest.cpp \
    src/network/networkmanagertest.cpp \
    src/fragments/fragmentsfragmenttest.cpp \
    src/fragments/fragmentspagetest.cpp \
    src/fragments/fragmentsfactorytest.cpp \
    src/engines/router/routerplannertest.cpp \
    src/engines/liveboard/liveboardfactorytest.cpp \
    src/engines/vehicle/vehiclefactorytest.cpp

HEADERS += \
    src/database/databasemanagertest.h \
    src/network/networkmanagertest.h \
    src/fragments/fragmentsfragmenttest.h \
    src/fragments/fragmentspagetest.h \
    src/fragments/fragmentsfactorytest.h \
    src/engines/router/routerplannertest.h \
    src/engines/liveboard/liveboardfactorytest.h \
    src/engines/vehicle/vehiclefactorytest.h

DISTFILES += \
    rpm/qrail-tests.spec \
    qrail-tests.desktop

# Due a bug in the spec file, these lines can't be removed
SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172
CONFIG += sailfishapp_i18n
