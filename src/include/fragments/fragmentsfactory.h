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
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QMutex>
#include <QtCore/QMutexLocker>
#include <QtCore/QSharedPointer>

#include "fragments/fragmentsfragment.h"
#include "fragments/fragmentspage.h"
#include "fragments/fragmentscache.h"
#include "network/networkmanager.h"
#include "network/networkeventsource.h"
#include "qrail.h"

#define BASE_URL "http://lc.dylanvanassche.be/sncb/connections"
#define REAL_TIME_URL_POLL "http://lc.dylanvanassche.be/sncb/events"
#define REAL_TIME_URL_SSE "http://lc.dylanvanassche.be/sncb/events/sse"
#define GTFS_REGULAR "gtfs:Regular"
#define GTFS_NOT_AVAILABLE "gtfs:NotAvailable"
#define GTFS_MUST_PHONE "gtfs:MustPhone"
#define GTFS_MUST_COORDINATE_WITH_DRIVER "gtfs:MustCoordinateWithDriver"

#define VERBOSE_HTTP_STATUS // Show HTTP results

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
    static QRail::Fragments::Factory *getInstance(QRail::Network::EventSource::Subscription subscriptionType);
    //! Fetches a Linked Connections page.
    /*!
        \param uri The URI of the page you want to fetch.
        \param caller The caller of this method.
     */
    void getPage(const QUrl &uri);
    //! Fetches a Linked Connections page.
    /*!
        \param departureTime The timestamp of the page (departure time).
               The page will contain at least this timestamp and the next connections that are following on this timestamp.
        \param caller The caller of this method.
     */
    void getPage(const QDateTime &departureTime);
    //! Mutex access to page cache
    QRail::Fragments::Cache* pageCache() const;
    void setPageCache(QRail::Fragments::Cache* pageCache);

signals:
    //! Emitted when a page has been become ready.
    void pageReady(QSharedPointer<QRail::Fragments::Page> page);
    //! Emitted when a resource is fetched from the Network::Manager.
    void getResource(const QUrl &uri);
    //! Emitted when an error occurred during processing.
    void error(const QString &message);
    //! Emitted when a connection has been updated.
    void connectionChanged(const QUrl &uri);
    //! Emitted when a fragment has been updated
    void fragmentUpdated(QSharedPointer<QRail::Fragments::Fragment> fragment);
    //! Emitted when a page has been updated
    void pageUpdated(QUrl pageURI);
    //! Emitted when a page and fragment are updated
    void fragmentAndPageUpdated(QSharedPointer<QRail::Fragments::Fragment> fragment, QUrl page);
    //! Emitted when an update has been successfully processed
    void updateProcessed(qint64 timestamp);
    void updateReceived(qint64 timestamp);

private slots:
    void handleEventSource(QString message);
    void processHTTPReply();

private:
    mutable QMutex m_cache_mutex;
    QRail::Network::EventSource *m_eventSource;
    QRail::Fragments::Cache* m_pageCache;
    QRail::Fragments::Fragment::GTFSTypes parseGTFSType(QString type);
    static QRail::Fragments::Factory *m_instance;
    QRail::Network::Manager *m_http;
    QSharedPointer<QNetworkReply> m_reply;
    void getPageByURIFromNetworkManager(const QUrl &uri);
    QSharedPointer<QRail::Fragments::Fragment> generateFragmentFromJSON(const QJsonObject &data);
    explicit Factory(QRail::Network::EventSource::Subscription subscriptionType, QObject *parent = nullptr);
    QRail::Network::EventSource::Subscription m_subscriptionType;
};
} // namespace Fragments
} // namespace QRail

#endif // LINKEDCONNECTIONFACTORY_H
