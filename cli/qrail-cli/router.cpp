#include "router.h"

router::router(QString departureStation, QString arrivalStation, QString departureTime, QString maxTransfers, QString mode, QObject *parent) : QObject(parent)
{
    // Setup QRail
    initQRail();
    // Launch query
    if(mode == "polling") {
        qDebug() << "Polling subscription set";
        planner = QRail::RouterEngine::Planner::getInstance(QRail::Network::EventSource::Subscription::POLLING);
    }
    else {
        qDebug() << "SSE subscription set";
        planner = QRail::RouterEngine::Planner::getInstance(QRail::Network::EventSource::Subscription::SSE);
    }
    // Let the Qt meta object system know how it should handle our custom QObjects
    qRegisterMetaType<QList<QSharedPointer<QRail::RouterEngine::Route> > >("QList<QRail::RouterEngine::Route*>");

    m_from = departureStation;
    m_to = arrivalStation;
    m_departureTime = departureTime;
    m_maxTransfers = maxTransfers;
}

void router::route()
{
    planner->unwatchAll();
    timestamp = QDateTime::currentDateTime();
    // Connect the signals
    connect(planner, SIGNAL(stream(QSharedPointer<QRail::RouterEngine::Route>)), this, SLOT(processRoutesStream(QSharedPointer<QRail::RouterEngine::Route>)));
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), this, SLOT(processRoutesFinished(QRail::RouterEngine::Journey*)));
    connect(planner, SIGNAL(processing(QUrl)), this, SLOT(processing(QUrl)));
    connect(planner, SIGNAL(requested(QUrl)), this, SLOT(requested(QUrl)));
    connect(planner, SIGNAL(updateReceived(qint64)), this, SLOT(updateReceived(qint64)));

    QDateTime start = QDateTime::currentDateTimeUtc();
    planner->getConnections(
        QUrl(m_from), // From: Vilvoorde
        QUrl(m_to), // To: Brugge
        QDateTime::fromString(m_departureTime, Qt::ISODate), // Departure time (UTC)
        m_maxTransfers.toInt() // Max transfers
    );

    QEventLoop loop1;
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loop1, SLOT(quit()));
    loop1.exec();
    qInfo() << "Network routing" << m_from << "->" << m_to << "took"
            << start.msecsTo(QDateTime::currentDateTimeUtc())
            << "msecs";
}

void router::processRoutesFinished(QRail::RouterEngine::Journey *journey)
{
    planner->watch(journey);
    qDebug() << "JOURNEY RECEIVED:" << journey;
    qDebug() << "CSA found" << journey->routes().size() << "possible routes";
    foreach (QSharedPointer<QRail::RouterEngine::Route> route, journey->routes()) {
        // Verify the complete trip
        qDebug() << "Trip:"
                 << route->departureStation()->station()->name().value(QLocale::Language::Dutch)
                 << "->"
                 << route->arrivalStation()->station()->name().value(QLocale::Language::Dutch)
                 << " Route:";

        // Keep track of the transfer stations
        QStringList retrievedTransferStations;

        // Log the complete trip to the console and verify it
        foreach (QRail::RouterEngine::Transfer *transfer, route->transfers()) {
            if (transfer->type() == QRail::RouterEngine::Transfer::Type::TRANSFER) {
                qDebug() << "TRANSFER:"
                         << "Changing vehicle at"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch)
                         << "Arriving with:" << transfer->arrivalLeg()->vehicleInformation()->uri()
                         << "Departuring with:" << transfer->departureLeg()->vehicleInformation()->uri();
                retrievedTransferStations << transfer->station()->name().value(QLocale::Language::Dutch);
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
                qDebug() << "DEPARTURE:"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch)
                         << transfer->departureLeg()->vehicleInformation()->uri();
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
                qDebug() << "ARRIVAL:"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch)
                         << transfer->arrivalLeg()->vehicleInformation()->uri();
            } else {
                qCritical() << "Transfer object is INVALID";
            }
        }
    }
    qint64 diff = timestamp.msecsTo(QDateTime::currentDateTime());
    qDebug() << "All routes processed:" << diff << "ms";
    timestamp = QDateTime::currentDateTime();
}

void router::processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route)
{
    // Log the complete trip to the console and verify it
    qint64 diff = timestamp.msecsTo(QDateTime::currentDateTime());
    qDebug() << diff << "ms";
    /*foreach (QRail::RouterEngine::Transfer *transfer, route->transfers()) {
        if (transfer->type() == QRail::RouterEngine::Transfer::Type::TRANSFER) {
            qDebug() << "TRANSFER:"
                     << "Changing vehicle at"
                     << transfer->time().time().toString("hh:mm")
                     << transfer->station()->name().value(QLocale::Language::Dutch);
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
            qDebug() << "DEPARTURE:"
                     << transfer->time().time().toString("hh:mm")
                     << transfer->station()->name().value(QLocale::Language::Dutch);
        } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
            qDebug() << "ARRIVAL:"
                     << transfer->time().time().toString("hh:mm")
                     << transfer->station()->name().value(QLocale::Language::Dutch);
        }
    }*/
}

void router::processing(const QUrl &pageURI)
{
    qDebug() << "Page received:" << pageURI.toString();
}

void router::requested(const QUrl &pageURI)
{
    //qDebug() << "Page requested:" << pageURI.toString();
}

void router::updateReceived(qint64 time)
{
    qDebug() << "Update process started:" << time;
    timestamp = QDateTime::currentDateTime();
}
