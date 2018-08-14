/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of BeRail.                                               *
 *                                                                            *
 *   BeRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   BeRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with BeRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#ifndef CSAVEHICLE_H
#define CSAVEHICLE_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QMetaType>

namespace CSA {
class Vehicle : public QObject
{
    Q_OBJECT
public:
    enum class OccupancyLevel
    {
        UNSUPPORTED,
        UNKNOWN,
        LOW,
        MEDIUM,
        HIGH
    };
    explicit Vehicle(
            const QUrl &uri,
            const QUrl &id,
            const QString &headsign,
            QObject *parent = nullptr
            );
    QUrl uri() const;
    void setUri(const QUrl &uri);
    QUrl id() const;
    void setId(const QUrl &id);
    QString headsign() const;
    void setHeadsign(const QString &headsign);

signals:
    void uriChanged();
    void idChanged();
    void headsignChanged();

private:
    QUrl m_uri;
    QUrl m_id;
    QString m_headsign;

    Q_ENUM(OccupancyLevel)
};
}

#endif // CSAVEHICLE_H
