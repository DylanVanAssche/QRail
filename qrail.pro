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

TARGET = qrail
VERSION = 0.0.1

# Uncomment this setting if you want to build static library
#CONFIG += staticlib

# Library mode
CONFIG(staticlib): DEFINES += QRAIL_STATIC_LIB
DEFINES += QRAIL_LIB
TEMPLATE = lib
win32:TARGET_EXT = .dll

# Qt modules
QT += core \
    network \
    positioning \
    concurrent \
    sql

# QMake config
CONFIG += c++11

# Disable debug and warning messages while releasing for security reasons
CONFIG(release, debug|release):DEFINES += QT_NO_DEBUG_OUTPUT \
QT_NO_WARNING_OUTPUT

# Add external module QtCSV to read CSV files
include(qtcsv/qtcsv.pri)

# Include QRail library files
include(qrail.pri)

# Compiler configuration
!msvc {
    # Flags for GCC compilers
    CONFIG += warn_on
    QMAKE_CXXFLAGS_WARN_ON += -Werror -Wformat=2 -Wuninitialized -Winit-self \
            -Wswitch-enum -Wundef -Wpointer-arith \
            -Wdisabled-optimization -Wcast-align -Wcast-qual
}

# Installation configuration
unix {
    # Settings for "make install"
    copy_lib_headers.path = /usr/local/include/qrail/
    copy_lib_headers.files = $$PWD/include/qrail/*.h
    target.path = /usr/local/lib
    INSTALLS += target copy_lib_headers
}

# Print out information
message(=== QRail configuration ===)
message(Qt version: $$[QT_VERSION])
message(QRail version: $$VERSION)
message(QRail files will be created in folder: $$OUT_PWD)
message(=== \"make install\" settings ===)
message(Library files will be copied to folder: $$target.path)
message(Library headers will be copied to folder: $$copy_lib_headers.path)
