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
VERSION = 0.0.1
TEMPLATE = app

# Qt modules
QT += testlib
QT -= gui

# QMake config
CONFIG += console \
        testcase
CONFIG -= app_bundle

# Compiler configuration
!msvc {
    # Flags for GCC compilers
    CONFIG += warn_on
    QMAKE_CXXFLAGS_WARN_ON += -Werror -Wformat=2 -Wuninitialized -Winit-self \
            -Wmissing-include-dirs -Wswitch-enum -Wundef -Wpointer-arith \
            -Wdisabled-optimization -Wcast-align -Wcast-qual
}

# Sets the location of the QRail library for the linker
# DEFAULT: The same location as the qrail-tests binary
QRAIL_LOCATION = $$OUT_PWD
LIBS += -L$$QRAIL_LOCATION -lqrail

# Header include path of the QRail library
INCLUDEPATH += $$PWD/../include

SOURCES += \
    $$PWD/qrail-tests.cpp \
    $$PWD/database/databasemanagertest.cpp \
    $$PWD/network/networkmanagertest.cpp \
    $$PWD/fragments/fragmentsfragmenttest.cpp \
    $$PWD/fragments/fragmentspagetest.cpp \
    $$PWD/fragments/fragmentsfactorytest.cpp \
    $$PWD/csa/csaplannertest.cpp

HEADERS += \
    $$PWD/database/databasemanagertest.h \
    $$PWD/network/networkmanagertest.h \
    $$PWD/fragments/fragmentsfragmenttest.h \
    $$PWD/fragments/fragmentspagetest.h \
    $$PWD/fragments/fragmentsfactorytest.h \
    $$PWD/csa/csaplannertest.h

# Print out configuration information
message(=== QRail tests configuration ===)
message(Qt version: $$[QT_VERSION])
message(qrail-tests binary will be created in folder: $$OUT_PWD)
message(Expected location of the QRail library: $$QRAIL_LOCATION)
