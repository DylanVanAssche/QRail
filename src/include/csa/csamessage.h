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

#ifndef CSAMESSAGE_H
#define CSAMESSAGE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

namespace CSA {
class Message : public QObject
{
    Q_OBJECT
public:
    explicit Message(
            const QString &header,
            const QString &description,
            const QString &lead,
            const QUrl &link,
            QObject *parent = nullptr
            );
    explicit Message(
            const QString &header,
            const QString &description,
            QObject *parent = nullptr
            );
    QString header() const;
    void setHeader(const QString &header);
    QString description() const;
    void setDescription(const QString &description);
    QString lead() const;
    void setLead(const QString &lead);
    QUrl link() const;
    void setLink(const QUrl &link);

signals:
    void headerChanged();
    void descriptionChanged();
    void leadChanged();
    void linkChanged();

private:
    QString m_header;
    QString m_description;
    QString m_lead;
    QUrl m_link;
};
}

#endif // CSAMESSAGE_H
