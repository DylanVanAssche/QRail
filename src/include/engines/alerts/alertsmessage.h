/******************************************************************************
 * Copyright (C) 2018 by Dylan Van Assche                                     *
 *                                                                            *
 * This file is part of QRail.                                               *
 *                                                                            *
 *   QRail is free software: you can redistribute it and/or modify it        *
 *   under the terms of the GNU Lesser General Public License as published    *
 *   by the Free Software Foundation, either version 3 of the License, or     *
 *   (at your option) any later version.                                      *
 *                                                                            *
 *   QRail is distributed in the hope that it will be useful,                *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of           *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            *
 *   GNU Lesser General Public License for more details.                      *
 *                                                                            *
 *   You should have received a copy of the GNU Lesser General Public         *
 *   License along with QRail.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************/

#ifndef CSAMESSAGE_H
#define CSAMESSAGE_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

#include "qrail.h"

namespace QRail {
namespace AlertsEngine {
class QRAIL_SHARED_EXPORT Message : public QObject
{
    Q_OBJECT
public:
    //! QRail::AlertsEngine::Message constructor: full.
    /*!
        \param header the header of the Message.
        \param description the description of the Message.
        \param lead the lead of the Message.
        \param link the link of the Message.
        \param parent QObject parent-child memory management.
        \public
        Constructs a Message to store the information about
        a disturbance, remark or anything else that might be
        usefull for the user.
     */
    explicit Message(
        const QString &header,
        const QString &description,
        const QString &lead,
        const QUrl &link,
        QObject *parent = nullptr
    );
    //! QRail::AlertsEngine::Message constructor: minimum.
    /*!
        \param header the header of the Message.
        \param description the description of the Message.
        \param parent QObject parent-child memory management.
        \public
        Constructs a Message to store the information about
        a disturbance, remark or anything else that might be
        usefull for the user.
     */
    explicit Message(
        const QString &header,
        const QString &description,
        QObject *parent = nullptr
    );
    //! Gets the header.
    /*!
        \return The header of the Message.
        \public
        Gets the header of the Message and returns it.
     */
    QString header() const;
    //! Sets the header.
    /*!
        \param header the header of the Message.
        \public
        Sets the header of the Message to the given QString &header.
     */
    void setHeader(const QString &header);
    //! Gets the description.
    /*!
        \return The description of the Message.
        \public
        Gets the description of the Message and returns it.
     */
    QString description() const;
    //! Sets the description.
    /*!
        \param description the header of the Message.
        \public
        Sets the description of the Message to the given QString &header.
     */
    void setDescription(const QString &description);
    //! Gets the lead.
    /*!
        \return The lead of the Message.
        \public
        Gets the lead of the Message and returns it.
     */
    QString lead() const;
    //! Sets the lead.
    /*!
        \param lead the lead of the Message.
        \public
        Sets the lead of the Message to the given QString &lead.
     */
    void setLead(const QString &lead);
    //! Gets the link.
    /*!
        \return The URL link of the Message.
        \public
        Gets the URL of the Message and returns it.
     */
    QUrl link() const;
    //! Sets the link.
    /*!
        \param link the link of the Message.
        \public
        Sets the link of the Message to the given QUrl &link.
     */
    void setLink(const QUrl &link);

signals:
    //! Emitted when the header has been changed.
    void headerChanged();
    //! Emitted when the description has been changed.
    void descriptionChanged();
    //! Emitted when the lead has been changed.
    void leadChanged();
    //! Emitted when the link has been changed.
    void linkChanged();

private:
    QString m_header;
    QString m_description;
    QString m_lead;
    QUrl m_link;
};
}
}

#endif // CSAMESSAGE_H
