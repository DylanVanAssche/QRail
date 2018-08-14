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

#ifndef LINKEDCONNECTIONPAGE_H
#define LINKEDCONNECTIONPAGE_H

#include <QtCore/QObject>
#include <QtCore/QUrl>
#include <QtCore/QDateTime>
#include <QtCore/QList>
#include "fragmentsfragment.h"

namespace Fragments {
class Page : public QObject
{
    Q_OBJECT
public:
    explicit Page(QObject *parent = nullptr);
    explicit Page(
            const QUrl &uri,
            const QDateTime &timestamp,
            const QUrl &hydraNext,
            const QUrl &hydraPrevious,
            const QList<Fragments::Fragment *> &fragments,
            QObject *parent = nullptr
            );
    QUrl uri() const;
    void setURI(const QUrl &uri);
    QDateTime timestamp() const;
    void setTimestamp(const QDateTime &timestamp);
    QUrl hydraNext() const;
    void setHydraNext(const QUrl &hydraNext);
    QUrl hydraPrevious() const;
    void setHydraPrevious(const QUrl &hydraPrevious);
    QList<Fragments::Fragment *> fragments() const;
    void setFragments(const QList<Fragments::Fragment *> &fragments);

signals:
    void uriChanged();
    void timestampChanged();
    void hydraNextChanged();
    void hydraPreviousChanged();
    void fragmentsChanged();

private:
    QUrl m_uri;
    QDateTime m_timestamp;
    QUrl m_hydraNext;
    QUrl m_hydraPrevious;
    QList<Fragments::Fragment *> m_fragments;
};
}

#endif // LINKEDCONNECTIONPAGE_H
