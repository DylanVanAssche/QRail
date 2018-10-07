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
            .

SOURCES += \
    $$PWD/src/engines/alerts/alertsmessage.cpp \
    $$PWD/src/engines/router/routerplanner.cpp \
    $$PWD/src/engines/router/routerroute.cpp \
    $$PWD/src/engines/router/routertransfer.cpp \
    $$PWD/src/engines/router/routerrouteleg.cpp \
    $$PWD/src/engines/router/routerroutelegend.cpp \
    $$PWD/src/engines/router/routerstationstopprofile.cpp \
    $$PWD/src/engines/router/routertrainprofile.cpp \
    $$PWD/src/engines/liveboard/liveboardfactory.cpp \
    $$PWD/src/engines/liveboard/liveboardboard.cpp \
    $$PWD/src/engines/station/stationstation.cpp \
    $$PWD/src/engines/station/stationnullstation.cpp \
    $$PWD/src/engines/station/stationfactory.cpp \
    $$PWD/src/engines/vehicle/vehiclevehicle.cpp \
    $$PWD/src/engines/vehicle/vehiclestop.cpp \
    $$PWD/src/engines/vehicle/vehiclefactory.cpp \
    $$PWD/src/database/databasemanager.cpp \
    $$PWD/src/network/networkmanager.cpp \
    $$PWD/src/network/networkdispatcher.cpp \
    $$PWD/src/fragments/fragmentsfragment.cpp \
    $$PWD/src/fragments/fragmentspage.cpp \
    $$PWD/src/fragments/fragmentsfactory.cpp \
    $$PWD/src/fragments/fragmentsdispatcher.cpp \
    $$PWD/src/qrail.cpp \
    $$PWD/src/engines/vehicle/vehiclenullvehicle.cpp \
    $$PWD/src/engines/liveboard/liveboardnullboard.cpp \
    $$PWD/src/include/engines/router/routerjourney.cpp \
    $$PWD/src/engines/router/routernulljourney.cpp

HEADERS += \
    $$PWD/src/include/engines/alerts/alertsmessage.h \
    $$PWD/src/include/engines/router/routerplanner.h \
    $$PWD/src/include/engines/router/routerroute.h \
    $$PWD/src/include/engines/router/routertransfer.h \
    $$PWD/src/include/engines/router/routerrouteleg.h \
    $$PWD/src/include/engines/router/routerroutelegend.h \
    $$PWD/src/include/engines/router/routerstationstopprofile.h \
    $$PWD/src/include/engines/router/routertrainprofile.h \
    $$PWD/src/include/engines/router/routerjourney.h \
    $$PWD/src/include/engines/router/routernulljourney.h \
    $$PWD/src/include/engines/liveboard/liveboardfactory.h \
    $$PWD/src/include/engines/liveboard/liveboardboard.h \
    $$PWD/src/include/engines/liveboard/liveboardnullboard.h \
    $$PWD/src/include/engines/vehicle/vehiclevehicle.h \
    $$PWD/src/include/engines/vehicle/vehiclestop.h \
    $$PWD/src/include/engines/vehicle/vehiclefactory.h \
    $$PWD/src/include/engines/vehicle/vehiclenullvehicle.h \
    $$PWD/src/include/engines/station/stationstation.h \
    $$PWD/src/include/engines/station/stationnullstation.h \
    $$PWD/src/include/engines/station/stationfactory.h \
    $$PWD/src/include/database/databasemanager.h \
    $$PWD/src/include/network/networkmanager.h \
    $$PWD/src/include/network/networkdispatcher.h \
    $$PWD/src/include/fragments/fragmentsfragment.h \
    $$PWD/src/include/fragments/fragmentspage.h \
    $$PWD/src/include/fragments/fragmentsfactory.h \
    $$PWD/src/include/fragments/fragmentsdispatcher.h \
    $$PWD/qtcsv/include/qtcsv/stringdata.h \
    $$PWD/src/include/qrail.h

DISTFILES += \
    $$PWD/rpm/qrail.changes

OTHER_FILES += \
    $$PWD/rpm/qrail.spec

RESOURCES += \
    $$PWD/resources.qrc
