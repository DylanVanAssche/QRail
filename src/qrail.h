/*
*   This file is part of BeRail.
*
*   BeRail is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   BeRail is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with BeRail.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef QRAIL_H
#define QRAIL_H

#include <QtCore/QtGlobal>

#ifdef QRAIL_STATIC_LIB
#   define QRAIL_SHARED_EXPORT
#else
#   if defined(QRAIL_LIB)
#       define QRAIL_SHARED_EXPORT Q_DECL_EXPORT
#   else
#       define QRAIL_SHARED_EXPORT Q_DECL_IMPORT
#   endif
#endif

#endif // QRAIL_H
