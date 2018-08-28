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

#ifndef LINKEDCONNECTIONFACTORY_H
#define LINKEDCONNECTIONFACTORY_H

#include <QtCore/QObject>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonParseError>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtCore/QRegularExpression>
#include <QtCore/QDebug>

#include "fragments/fragmentsfragment.h"
#include "fragments/fragmentspage.h"
#include "fragments/fragmentsdispatcher.h"
#include "network/networkmanager.h"

#define BASE_URL "https://graph.irail.be/sncb/connections"

// Factory pattern to generate Linked Connections fragments on the fly
namespace Fragments {
class Factory: public QObject
{
    Q_OBJECT
public:
    static Fragments::Factory *getInstance();
    void getPage(const QUrl &uri, QObject *caller);
    void getPage(const QDateTime &departureTime, QObject *caller);
    Fragments::Dispatcher *dispatcher() const;

protected:
    virtual void customEvent(QEvent *event);

signals:
    void pageReady(Fragments::Page *page);
    void getResource(const QUrl &uri, QObject *caller);
    void error(const QString &message);

private:
    static Fragments::Factory *m_instance;
    Network::Manager *m_http;
    Fragments::Dispatcher *m_dispatcher;
    void getPageByURIFromNetworkManager(const QUrl &uri);
    Fragments::Fragment *generateFragmentFromJSON(const QJsonObject &data);
    bool validateData(const QJsonObject &data, const QStringList &properties);
    void processHTTPReply(QNetworkReply *reply);
    Network::Manager *http() const;
    void setHttp(Network::Manager *http);
    void setDispatcher(Fragments::Dispatcher *dispatcher);
    explicit Factory(QObject *parent = nullptr);
};
}

#endif // LINKEDCONNECTIONFACTORY_H
