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
#include "engines/alerts/alertsmessage.h"
using namespace QRail;

QRail::AlertsEngine::Message::Message(const QString &header, const QString &description,
                                      const QString &lead, const QUrl &link, QObject *parent): QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_header = header;
    m_description = description;
    m_lead = lead;
    m_link = link;
}

QRail::AlertsEngine::Message::Message(const QString &header, const QString &description,
                                      QObject *parent): QObject(parent)
{
    // Use private members to avoid signal firing on construction
    m_header = header;
    m_description = description;
    m_lead = QString();
    m_link = QUrl();
}

QString QRail::AlertsEngine::Message::header() const
{
    return m_header;
}

void QRail::AlertsEngine::Message::setHeader(const QString &header)
{
    m_header = header;
    emit this->headerChanged();
}

QString QRail::AlertsEngine::Message::description() const
{
    return m_description;
}

/**
 * @file alertsmessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the description
 * @param const QString &description
 * @package AlertsEngine
 * @public
 * Sets the description of the message to the given QString &description
 */
void QRail::AlertsEngine::Message::setDescription(const QString &description)
{
    m_description = description;
    emit this->descriptionChanged();
}

/**
 * @file alertsmessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the lead
 * @return const QString lead
 * @package AlertsEngine
 * @public
 * Gets the lead of the message and returns it.
 */
QString QRail::AlertsEngine::Message::lead() const
{
    return m_lead;
}

/**
 * @file alertsmessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the lead
 * @param const QString &lead
 * @package AlertsEngine
 * @public
 * Sets the lead of the message to the given QString &lead
 */
void QRail::AlertsEngine::Message::setLead(const QString &lead)
{
    m_lead = lead;
    emit this->leadChanged();
}

/**
 * @file alertsmessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the link
 * @return const QUrl link
 * @package AlertsEngine
 * @public
 * Gets the link of the message and returns it.
 */
QUrl QRail::AlertsEngine::Message::link() const
{
    return m_link;
}

/**
 * @file alertsmessage.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the link
 * @param const QUrl &link
 * @package AlertsEngine
 * @public
 * Sets the link of the message to the given QUrl &link
 */
void QRail::AlertsEngine::Message::setLink(const QUrl &link)
{
    m_link = link;
    emit this->linkChanged();
}
