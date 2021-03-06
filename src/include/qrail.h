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
#ifndef QRAIL_H
#define QRAIL_H

#include <QtCore/QFile>
#include <QtCore/QIODevice>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QString>
#include <QtCore/QtGlobal>

#define PAGE_CONTEXT_PATH "://context/page/context.json"
#define VEHICLE_CONTEXT_PATH "://context/vehicle/context.json"
#define LOCK_TIMEOUT 750
#define HTTP_TIMEOUT 180000 // 180 secs between pages maximum

//! Inits the QRail library resources.
void initQRail();
//! Retrieves the Linked Connections page context.
QJsonObject pageContext();
//! Retireves the Linked Connections vehicle context.
QJsonObject vehicleContext();

#ifdef QRAIL_STATIC_LIB
#define QRAIL_SHARED_EXPORT
#else
#if defined(QRAIL_LIB)
#define QRAIL_SHARED_EXPORT Q_DECL_EXPORT
#else
#define QRAIL_SHARED_EXPORT Q_DECL_IMPORT
#endif
#endif

#endif // QRAIL_H
