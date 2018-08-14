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

CONFIG *= qt
QT *= core

# Public header files are kept in a separate 'include' folder
INCLUDEPATH += $$PWD/src/include \
            $$PWD/qtcsv/include/qtcsv \
            $$PWD

SOURCES += \
    $$PWD/src/database/databasemanager.cpp \
    $$PWD/src/network/networkmanager.cpp \
    $$PWD/src/csa/csaplanner.cpp \
    $$PWD/src/csa/csaroute.cpp \
    $$PWD/src/csa/csavehicle.cpp \
    $$PWD/src/csa/csavehiclestop.cpp \
    $$PWD/src/csa/csastation.cpp \
    $$PWD/src/csa/csanullstation.cpp \
    $$PWD/src/csa/csastationfactory.cpp \
    $$PWD/src/csa/csatransfer.cpp \
    $$PWD/src/csa/csamessage.cpp \
    $$PWD/src/fragments/fragmentsfragment.cpp \
    $$PWD/src/fragments/fragmentspage.cpp \
    $$PWD/src/fragments/fragmentsfactory.cpp \
    $$PWD/src/csa/csarouteleg.cpp \
    $$PWD/src/csa/csaroutelegend.cpp \
    $$PWD/src/csa/csastationstopprofile.cpp \
    $$PWD/src/csa/csatrainprofile.cpp

HEADERS += \
    $$PWD/src/qrail.h \
    $$PWD/src/include/database/databasemanager.h \
    $$PWD/src/include/network/networkmanager.h \
    $$PWD/src/include/csa/csaplanner.h \
    $$PWD/src/include/csa/csaroute.h \
    $$PWD/src/include/csa/csavehicle.h \
    $$PWD/src/include/csa/csavehiclestop.h \
    $$PWD/src/include/csa/csastation.h \
    $$PWD/src/include/csa/csanullstation.h \
    $$PWD/src/include/csa/csastationfactory.h \
    $$PWD/src/include/csa/csatransfer.h \
    $$PWD/src/include/csa/csamessage.h \
    $$PWD/src/csa/csarouteleg.h \
    $$PWD/src/csa/csaroutelegend.h \
    $$PWD/src/csa/csastationstopprofile.h \
    $$PWD/src/csa/csatrainprofile.h \
    $$PWD/src/include/fragments/fragmentsfragment.h \
    $$PWD/src/include/fragments/fragmentspage.h \
    $$PWD/src/include/fragments/fragmentsfactory.h \
    $$PWD/qtcsv/include/qtcsv/stringdata.h \

DISTFILES += \
    $$PWD/rpm/qrail.changes

OTHER_FILES += \
    $$PWD/rpm/qrail.spec
