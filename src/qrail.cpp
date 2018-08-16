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

/**
 * @file qrail.cpp
 * @author Dylan Van Assche
 * @date 16 Aug 2018
 * @brief Init the QRail library
 * @public
 * @warning Do not run any code without calling this function first! QRail won't work without it.
 * Init the QRail library assets.
 */
void initQRail() {
    /*
     * Init resources since we're running in library mode.
     * Without this, the resources aren't visible to QRail.
     */
    Q_INIT_RESOURCE(resources);
}
