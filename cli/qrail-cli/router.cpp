#include "router.h"

router::router(QString departureStation, QString arrivalStation, QString departureTime, QString maxTransfers, QString mode, bool verbose, QObject *parent) : QObject(parent)
{
    // Setup QRail
    initQRail();
    // Launch query
    if(mode == "polling") {
        qDebug() << "Polling subscription set";
        planner = QRail::RouterEngine::Planner::getInstance(QRail::Network::EventSource::Subscription::POLLING);
    }
    else if(mode == "pushing") {
        qDebug() << "SSE subscription set";
        planner = QRail::RouterEngine::Planner::getInstance(QRail::Network::EventSource::Subscription::SSE);
    }
    else {
        qDebug() << "Reference mode: rollback CSA disabled";
        planner = QRail::RouterEngine::Planner::getInstance(QRail::Network::EventSource::Subscription::NONE);
    }

    // Let the Qt meta object system know how it should handle our custom QObjects
    qRegisterMetaType<QList<QSharedPointer<QRail::RouterEngine::Route> > >("QList<QRail::RouterEngine::Route*>");

    m_from = departureStation;
    m_to = arrivalStation;
    m_departureTime = departureTime;
    m_maxTransfers = maxTransfers;
    m_verbose = verbose;
    m_ready = false;
}

void router::route()
{
    m_started_at = QDateTime::currentDateTime();

    // Connect the signals
    if(m_verbose) {
        connect(planner, SIGNAL(stream(QSharedPointer<QRail::RouterEngine::Route>)), this, SLOT(processRoutesStream(QSharedPointer<QRail::RouterEngine::Route>)));
    }
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), this, SLOT(processRoutesFinished(QRail::RouterEngine::Journey*)));
    connect(planner, SIGNAL(updateReceived(qint64)), this, SLOT(updateReceived()));

    // Start CSA
    planner->getConnections(
        QUrl(m_from), // From: departure station
        QUrl(m_to), // To: arrival station
        QDateTime::fromString(m_departureTime, Qt::ISODate), // Departure time (UTC)
        m_maxTransfers.toInt() // Max transfers
    );
}

void router::startReroute()
{
    qInfo() << "Start reroute";
    m_started_at = QDateTime::currentDateTime();
}

void router::updateReceived() {
    qInfo() << "Update received ";
}
void router::processRoutesFinished(QRail::RouterEngine::Journey *journey)
{
    // Start benchmarking updates when initial routing is complete
    if(!m_ready) {
        connect(planner, SIGNAL(startReroute()), this, SLOT(startReroute()));
        m_ready = true;
    }
    qDebug() << "CSA found" << journey->routes().size() << "possible routes";
    qint64 diff = m_started_at.msecsTo(QDateTime::currentDateTime());
    qDebug() << "All routes processed:" << diff << "ms";
}

void router::processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route)
{
    // Log the complete trip to the console and verify it
    qint64 diff = m_started_at.msecsTo(QDateTime::currentDateTime());
    qDebug() << diff << "ms";
    foreach (QSharedPointer<QRail::RouterEngine::Transfer> transfer, route->transfers()) {
        if (transfer->type() == QRail::RouterEngine::Transfer::Type::TRANSFER) {
            qDebug() << "TRANSFER:"
                     << "Changing vehicle at"
                     << transfer->departureLeg()->departure()->station()->name().value(QLocale::Language::Dutch)
                     << transfer->arrivalLeg()->arrival()->time().toUTC().toString("hh:mm")
                     << "delay:" << transfer->arrivalLeg()->arrival()->delay()/60 << "min"
                     << "-->"
                     << transfer->departureLeg()->departure()->time().toUTC().toString("hh:mm")
                     << "delay:" << transfer->departureLeg()->departure()->delay()/60 << "min"
                     << "vehicles:"
                     << transfer->arrivalLeg()->vehicleInformation()->uri().toString()
                     << "-->"
                     << transfer->departureLeg()->vehicleInformation()->uri().toString();
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
            qDebug() << "DEPARTURE:"
                     << transfer->time().toUTC().toString("hh:mm") << "UTC"
                     << "delay:" << ((transfer->delayedTime().toSecsSinceEpoch() - transfer->time().toSecsSinceEpoch())/60) << "min"
                     << transfer->station()->name().value(QLocale::Language::Dutch)
                     << "vehicle:"
                     << transfer->departureLeg()->vehicleInformation()->uri().toString();
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
            qDebug() << "ARRIVAL:"
                     << transfer->time().toUTC().toString("hh:mm") << "UTC"
                     << "delay:" << ((transfer->delayedTime().toSecsSinceEpoch() - transfer->time().toSecsSinceEpoch())/60) << "min"
                     << transfer->station()->name().value(QLocale::Language::Dutch)
                     << "vehicle:"
                     << transfer->arrivalLeg()->vehicleInformation()->uri().toString();
        }
    }
}
