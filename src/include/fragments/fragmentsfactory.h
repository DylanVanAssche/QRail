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
#ifndef LINKEDCONNECTIONFACTORY_H
#define LINKEDCONNECTIONFACTORY_H

#include <QtCore/QDebug>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonParseError>
#include <QtCore/QObject>
#include <QtCore/QRegularExpression>

#include "fragments/fragmentsdispatcher.h"
#include "fragments/fragmentsfragment.h"
#include "fragments/fragmentspage.h"
#include "fragments/fragmentscache.h"
#include "network/networkmanager.h"
#include "network/networkeventsource.h"
#include "qrail.h"

#define BASE_URL "https://graph.irail.be/sncb/connections"
#define REAL_TIME_URL "https://graph.irail.be/sncb/events"
#define GTFS_REGULAR "gtfs:Regular"
#define GTFS_NOT_AVAILABLE "gtfs:NotAvailable"
#define GTFS_MUST_PHONE "gtfs:MustPhone"
#define GTFS_MUST_COORDINATE_WITH_DRIVER "gtfs:MustCoordinateWithDriver"

//#define VERBOSE_HTTP_STATUS // Show HTTP results

// Factory pattern to generate Linked Connections fragments on the fly
namespace QRail {
namespace Fragments {
//! An Fragments::Factory allows you to generate Fragments::Station objects.
/**
 * \class Factory
 * The factory design pattern allows you to create Station objects in an easy way. Several modes are available to fetch your Station.
 */
class Factory : public QObject
{
    Q_OBJECT
public:
    //! Gets a QRail::Fragments::Factory instance.
    /*!
        \return QRail::Fragments::Factory *factory
        \public
        Constructs a QRail::Fragments::Factory if none exists and returns the
        instance.
     */
    static QRail::Fragments::Factory *getInstance();
    //! Fetches a Linked Connections page.
    /*!
        \param uri The URI of the page you want to fetch.
        \param caller The caller of this method.
        \note The caller is needed since the dispatcher will send you a special event using the Qt event system.
     */
    void getPage(const QUrl &uri, QObject *caller);
    //! Fetches a Linked Connections page.
    /*!
        \param departureTime The timestamp of the page (departure time).
               The page will contain at least this timestamp and the next connections that are following on this timestamp.
        \param caller The caller of this method.
        \note The caller is needed since the dispatcher will send you a special event using the Qt event system.
     */
    void getPage(const QDateTime &departureTime, QObject *caller);
    QRail::Fragments::Dispatcher *dispatcher() const;

protected:
    //! Dispatcher protected method, only here as a reference.
    virtual void customEvent(QEvent *event);

signals:
    //! Emitted when a page has been become ready.
    void pageReady(QRail::Fragments::Page *page);
    //! Emitted when a resource is fetched from the Network::Manager.
    void getResource(const QUrl &uri, QObject *caller);
    //! Emitted when an error occurred during processing.
    void error(const QString &message);
    //! Emitted when a connection has been updated.
    void connectionChanged(const QUrl &uri);

private slots:
    void handleEventSource(QString message);

private:
    QRail::Network::EventSource *m_eventSource;
    QRail::Fragments::Cache *m_pageCache;
    QRail::Fragments::Fragment::GTFSTypes parseGTFSType(QString type);
    static QRail::Fragments::Factory *m_instance;
    QRail::Network::Manager *m_http;
    QRail::Fragments::Dispatcher *m_dispatcher;
    void getPageByURIFromNetworkManager(const QUrl &uri);
    QRail::Fragments::Fragment *generateFragmentFromJSON(const QJsonObject &data);
    void processHTTPReply(QNetworkReply *reply);
    QRail::Network::Manager *http() const;
    void setHttp(QRail::Network::Manager *http);
    void setDispatcher(QRail::Fragments::Dispatcher *dispatcher);
    explicit Factory(QObject *parent = nullptr);
};
} // namespace Fragments
} // namespace QRail

#endif // LINKEDCONNECTIONFACTORY_H
