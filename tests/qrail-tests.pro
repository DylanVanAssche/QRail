
TARGET = qrail-tests
SAILFISHAPP_ICONS = 86x86 108x108 128x128 172x172

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

# Set QRail library location (static linked due Jolla Harbour restrictions)
CONFIG(debug, debug|release) {
    QRAIL_LOCATION = $$PWD/../build/debug
}
else {
    QRAIL_LOCATION = $$PWD/../build/release
}
LIBS += $$QRAIL_LOCATION/libqrail.a
INCLUDEPATH += $$PWD/../src/include \
            $$PWD/../qtcsv/include

SOURCES += \
    src/qrail-tests.cpp \
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

# Print out configuration information
message(=== QRail tests configuration ===)
message(Qt version: $$[QT_VERSION])
message(QRail static library path: $$QRAIL_LOCATION/libqrail.a)
message(QRail include path: $$INCLUDEPATH)
