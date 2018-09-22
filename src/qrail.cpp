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
#include "qrail.h"

// Helpers
/**
 * @file qrail.cpp
 * @author Dylan Van Assche
 * @date 31 Aug 2018
 * @brief Reads a JSON file
 * @param const QString &path
 * @return QJsonObject json
 * @public
 * Reads a JSON file, parses it and returns it as a QJsonObject.
 */
QJsonObject context(const QString &contextPath)
{
    // Read JSON data
    QFile file;
    QString data;
    file.setFileName(contextPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();

    // Convert to object and return it
    QJsonDocument document = QJsonDocument::fromJson(data.toUtf8());
    return document.object();
}

/**
 * @file qrail.cpp
 * @author Dylan Van Assche
 * @date 16 Aug 2018
 * @brief Init the QRail library
 * @public
 * @warning Do not run any code without calling this function first! QRail won't
 * work without it. Init the QRail library assets.
 */
void initQRail()
{
    /*
    * Init resources since we're running in library mode.
    * Without this, the resources aren't visible to QRail.
    */
    Q_INIT_RESOURCE(resources);
}

// Getters & Setters
/**
 * @file qrail.cpp
 * @author Dylan Van Assche
 * @date 31 Aug 2018
 * @brief Reads the LC page context in JSON
 * @return QJsonObject pageContext
 * @public
 * Reads the LC page context JSON file and returns it.
 */
QJsonObject pageContext()
{
    return context(PAGE_CONTEXT_PATH);
}

/**
 * @file qrail.cpp
 * @author Dylan Van Assche
 * @date 31 Aug 2018
 * @brief Reads the vehicle context in JSON
 * @return QJsonObject vehicleContext
 * @public
 * Reads the iRail vehicle context JSON file and returns it.
 */
QJsonObject vehicleContext()
{
    return context(VEHICLE_CONTEXT_PATH);
}
