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

QRail::Fragments::Page::Page(QObject *parent) : QObject(parent)
{

}

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


QUrl QRail::Fragments::Page::uri() const
{
    return m_uri;
}

void QRail::Fragments::Page::setURI(const QUrl &uri)
{
    m_uri = uri;
    emit this->uriChanged();
}

QDateTime QRail::Fragments::Page::timestamp() const
{
    return m_timestamp;
}

void QRail::Fragments::Page::setTimestamp(const QDateTime &timestamp)
{
    m_timestamp = timestamp;
    emit this->timestampChanged();
}

QUrl QRail::Fragments::Page::hydraNext() const
{
    return m_hydraNext;
}

void QRail::Fragments::Page::setHydraNext(const QUrl &hydraNext)
{
    m_hydraNext = hydraNext;
    emit this->hydraNextChanged();
}

QUrl QRail::Fragments::Page::hydraPrevious() const
{
    return m_hydraPrevious;
}

void QRail::Fragments::Page::setHydraPrevious(const QUrl &hydraPrevious)
{
    m_hydraPrevious = hydraPrevious;
    emit this->hydraPreviousChanged();
}

QList<QRail::Fragments::Fragment *> QRail::Fragments::Page::fragments() const
{
    return m_fragments;
}

void QRail::Fragments::Page::setFragments(const QList<QRail::Fragments::Fragment *> &fragments)
{
    m_fragments = fragments;
    emit this->fragmentsChanged();
}
