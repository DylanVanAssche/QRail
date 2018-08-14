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

#ifndef LINKEDCONNECTIONFACTORY_H
#define LINKEDCONNECTIONFACTORY_H

#include <QtCore/QObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QRegularExpression>
#include <QtCore/QDebug>
#include "fragmentsfragment.h"
#include "fragmentspage.h"
#include "../../network/networkmanager.h"
#define BASE_URL "https://graph.irail.be/sncb/connections"

// Factory pattern to generate Linked Connections fragments on the fly
namespace Fragments {
class Factory: public QObject
{
    Q_OBJECT
public:
    static Fragments::Factory *getInstance(QObject *parent = nullptr);
    void getPage(const QUrl &uri);
    void getPage(const QDateTime &departureTime);

signals:
    void pageReady(Fragments::Page *page);
    void getResource(const QUrl &uri);
    void error(const QString &message);

private slots:
    void processHTTPReply(QNetworkReply *reply);

private:
    static Fragments::Factory *m_instance;
    Network::Manager *m_http;
    void getPageByURIFromNetworkManager(const QUrl &uri);
    Fragments::Fragment *generateFragmentFromJSON(const QJsonObject &connection);
    bool validateData(const QJsonObject &data, const QStringList &properties);
    explicit Factory(QObject *parent);
    Network::Manager *http() const;
    void setHttp(Network::Manager *http);
};
}

#endif // LINKEDCONNECTIONFACTORY_H
