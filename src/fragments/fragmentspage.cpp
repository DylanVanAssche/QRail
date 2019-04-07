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
#include "fragments/fragmentspage.h"
using namespace QRail;

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Page constructor: empty
 * @param QObject *parent = nullptr
 * @package Fragments
 * @public
 * Constructs a Page to store the information about
 * a Linked Connection page for the Connection Scan Algorithm (CSA).
 */
QRail::Fragments::Page::Page(QObject *parent) : QObject(parent)
{

}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Page constructor: page with the next and previous URIs
 * @param const QUrl &uri
 * @param const QDateTime &timestamp
 * @param const QUrl &hydraNext
 * @param const QUrl &hydraPrevious
 * @param QObject *parent = nullptr
 * @package Fragments
 * @public
 * Constructs a Page to store the information about
 * a Linked Connection page for the Connection Scan Algorithm (CSA).
 */
QRail::Fragments::Page::Page(const QUrl &uri, const QDateTime &timestamp, const QUrl &hydraNext,
                             const QUrl &hydraPrevious, const QList<Fragment *> &fragments, QObject *parent): QObject(parent)
{
    // Enforce Page as the parent of all it's children, Qt will delete them when the Page is destroyed
    foreach (QRail::Fragments::Fragment *frag, fragments) {
        frag->setParent(this);
    }

    // Avoid signal triggering on contruction
    m_uri = uri;
    m_timestamp = timestamp;
    m_hydraNext = hydraNext;
    m_hydraPrevious = hydraPrevious;
    m_fragments = fragments;
}

Fragments::Page::~Page()
{
    foreach(auto f, m_fragments) {
        if(f) {
            f->deleteLater();
        }
    }
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Gets the URI of the linked connection page
 * @return const QUrl URI
 * @package Fragments
 * @public
 * Retrieves the URI of the linked connection page and returns it.
 */
QUrl QRail::Fragments::Page::uri() const
{
    return m_uri;
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Sets the URI of the linked connection page
 * @param const QUrl &URI
 * @package Fragments
 * @public
 * Sets the URI of the linked connection page to the given QUrl &uri.
 * Emits the uriChanged signal.
 */
void QRail::Fragments::Page::setURI(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Gets the timestamp of the linked connection page
 * @return const QDateTime timestamp
 * @package Fragments
 * @public
 * Retrieves the timestamp of the linked connection page and returns it.
 */
QDateTime QRail::Fragments::Page::timestamp() const
{
    return m_timestamp;
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Sets the timestamp of the linked connection fragment
 * @param const QDateTime &timestamp
 * @package Fragments
 * @public
 * Sets the timestamp of the linked connection fragment to the given QDateTime &timestamp.
 * Emits the timestampChanged signal.
 */
void QRail::Fragments::Page::setTimestamp(const QDateTime &timestamp)
{
    m_timestamp = timestamp;
    emit this->timestampChanged();
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Gets the URI of the next linked connection page
 * @return const QUrl URI
 * @package Fragments
 * @public
 * Retrieves the URI of the next linked connection page and returns it.
 */
QUrl QRail::Fragments::Page::hydraNext() const
{
    return m_hydraNext;
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Sets the URI of the linked connection next page
 * @param const QUrl &hydraPrevious
 * @package Fragments
 * @public
 * Sets the URI of the linked connection next page to the given QUrl &hydraPrevious.
 * Emits the hydraPreviousChanged signal.
 */
void QRail::Fragments::Page::setHydraNext(const QUrl &hydraNext)
{
    m_hydraNext = hydraNext;
    emit this->hydraNextChanged();
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Gets the URI of the previous linked connection page
 * @return const QUrl URI
 * @package Fragments
 * @public
 * Retrieves the URI of the previous linked connection page and returns it.
 */
QUrl QRail::Fragments::Page::hydraPrevious() const
{
    return m_hydraPrevious;
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 24 Jul 2018
 * @brief Sets the URI of the linked connection previous page
 * @param const QUrl &hydraPrevious
 * @package Fragments
 * @public
 * Sets the URI of the linked connection previous page to the given QUrl &hydraPrevious.
 * Emits the hydraPreviousChanged signal.
 */
void QRail::Fragments::Page::setHydraPrevious(const QUrl &hydraPrevious)
{
    m_hydraPrevious = hydraPrevious;
    emit this->hydraPreviousChanged();
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief Gets the list of fragments associated with this page
 * @return const QList<Fragment *> fragments
 * @package Fragments
 * @public
 * Gets the list of fragments associated with this page and return it.
 */
QList<QRail::Fragments::Fragment *> QRail::Fragments::Page::fragments() const
{
    return m_fragments;
}

/**
 * @file fragmentspage.cpp
 * @author Dylan Van Assche
 * @date 27 Jul 2018
 * @brief  Sets the list of fragments associated with this page
 * @param const QList<Fragment *> &fragments
 * @package Fragments
 * @public
 * Sets the list of fragments associated with this page to the given QList<Fragment *> &fragments.
 * Emits the fragmentsChanged signal.
 */
void QRail::Fragments::Page::setFragments(const QList<QRail::Fragments::Fragment *> &fragments)
{
    m_fragments = fragments;
    emit this->fragmentsChanged();
}
