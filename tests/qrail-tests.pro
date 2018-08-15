# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = qrail-tests

CONFIG += sailfishapp \
        c++11

QT += core \
    network \
    positioning \
    concurrent \
    sql \
    testlib

CONFIG(debug, debug|release) {
    QRAIL_LOCATION = $$PWD/../build/debug
}
else {
    QRAIL_LOCATION = $$PWD/../build/release
}
LIBS += $$QRAIL_LOCATION/libqrail.a

## Header include path of the QRail library
INCLUDEPATH += $$PWD/../src/include \
            $$PWD/../qtcsv/include

SOURCES += src/qrail-tests.cpp \
    src/database/databasemanagertest.cpp \
    src/network/networkmanagertest.cpp \
    src/fragments/fragmentsfragmenttest.cpp \
    src/fragments/fragmentspagetest.cpp \
    src/fragments/fragmentsfactorytest.cpp \
    src/csa/csaplannertest.cpp

HEADERS += \
    src/database/databasemanagertest.h \
    src/network/networkmanagertest.h \
    src/fragments/fragmentsfragmenttest.h \
    src/fragments/fragmentspagetest.h \
    src/fragments/fragmentsfactorytest.h \
    src/csa/csaplannertest.h

DISTFILES += \
    rpm/qrail-tests.changes \
    rpm/qrail-tests.spec \
    qrail-tests.desktop

SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

# Don't remove this line, a bug in the Sailfish OS build engine corrupts the build if this line is removed!
CONFIG += sailfishapp_i18n
