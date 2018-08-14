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

#include "csamessage.h"

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Message constructor: full
 * @param const QString &header
 * @param const QString &description
 * @param const QString &lead
 * @param const QUrl &link
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a Message to store the information about
 * a disturbance, remark or anything else that might be
 * usefull for the user.
 */
CSA::Message::Message(const QString &header, const QString &description, const QString &lead, const QUrl &link, QObject *parent): QObject(parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_header = header;
    m_description = description;
    m_lead = lead;
    m_link = link;
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Message constructor: minimum
 * @param const QString &header
 * @param const QString &description
 * @param QObject *parent
 * @package CSA
 * @public
 * Constructs a Message to store the information about
 * a disturbance, remark or anything else that might be
 * usefull for the user.
 */
CSA::Message::Message(const QString &header, const QString &description, QObject *parent)
{
    // Clean up when parent dies
    this->setParent(parent);

    // Use private members to avoid signal firing on construction
    m_header = header;
    m_description = description;
    m_lead = QString();
    m_link = QUrl();
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the header
 * @return const QString header
 * @package CSA
 * @public
 * Gets the header of the message and returns it.
 */
QString CSA::Message::header() const
{
    return m_header;
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the header
 * @param const QString &header
 * @package CSA
 * @public
 * Sets the header of the message to the given QString &header
 */
void CSA::Message::setHeader(const QString &header)
{
    m_header = header;
    emit this->headerChanged();
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the description
 * @return const QString description
 * @package CSA
 * @public
 * Gets the description of the message and returns it.
 */
QString CSA::Message::description() const
{
    return m_description;
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the description
 * @param const QString &description
 * @package CSA
 * @public
 * Sets the description of the message to the given QString &description
 */
void CSA::Message::setDescription(const QString &description)
{
    m_description = description;
    emit this->descriptionChanged();
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the lead
 * @return const QString lead
 * @package CSA
 * @public
 * Gets the lead of the message and returns it.
 */
QString CSA::Message::lead() const
{
    return m_lead;
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the lead
 * @param const QString &lead
 * @package CSA
 * @public
 * Sets the lead of the message to the given QString &lead
 */
void CSA::Message::setLead(const QString &lead)
{
    m_lead = lead;
    emit this->leadChanged();
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the link
 * @return const QUrl link
 * @package CSA
 * @public
 * Gets the link of the message and returns it.
 */
QUrl CSA::Message::link() const
{
    return m_link;
}

/**
 * @file csamessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the link
 * @param const QUrl &link
 * @package CSA
 * @public
 * Sets the link of the message to the given QUrl &link
 */
void CSA::Message::setLink(const QUrl &link)
{
    m_link = link;
    emit this->linkChanged();
}
