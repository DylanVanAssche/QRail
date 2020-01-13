#include "router.h"

router::router(QObject *parent) : QObject(parent)
{
    // Setup QRail
    initQRail();
    // Launch query
    planner = QRail::RouterEngine::Planner::getInstance();
    // Let the Qt meta object system know how it should handle our custom QObjects
    qRegisterMetaType<QList<QSharedPointer<QRail::RouterEngine::Route> > >("QList<QRail::RouterEngine::Route*>");
}

void router::route(QString from, QString to, QString departureTime, QString maxTransfers)
{
    planner->unwatchAll();
    // Connect the signals
    connect(planner, SIGNAL(stream(QSharedPointer<QRail::RouterEngine::Route>)), this, SLOT(processRoutesStream(QSharedPointer<QRail::RouterEngine::Route>)));
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), this, SLOT(processRoutesFinished(QRail::RouterEngine::Journey*)));
    connect(planner, SIGNAL(processing(QUrl)), this, SLOT(processing(QUrl)));
    connect(planner, SIGNAL(requested(QUrl)), this, SLOT(requested(QUrl)));

    QDateTime start = QDateTime::currentDateTimeUtc();
    planner->getConnections(
        QUrl(from), // From: Vilvoorde
        QUrl(to), // To: Brugge
        QDateTime::fromString(departureTime, Qt::ISODate), // Departure time (UTC)
        maxTransfers.toInt() // Max transfers
    );

    QEventLoop loop1;
    connect(planner, SIGNAL(finished(QRail::RouterEngine::Journey*)), &loop1, SLOT(quit()));
    loop1.exec();
    qInfo() << "Network routing" << from << "->" << to << "took"
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
                         << transfer->station()->name().value(QLocale::Language::Dutch);
                retrievedTransferStations << transfer->station()->name().value(QLocale::Language::Dutch);
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::DEPARTURE) {
                qDebug() << "DEPARTURE:"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch);
            } else if (transfer->type() == QRail::RouterEngine::Transfer::Type::ARRIVAL) {
                qDebug() << "ARRIVAL:"
                         << transfer->time().time().toString("hh:mm")
                         << transfer->station()->name().value(QLocale::Language::Dutch);
            } else {
                qCritical() << "Transfer object is INVALID";
            }
        }
    }
    qDebug() << "All routes processed";
}

void router::processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route)
{
    // Log the complete trip to the console and verify it
    foreach (QRail::RouterEngine::Transfer *transfer, route->transfers()) {
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
    }
}

void router::processing(const QUrl &pageURI)
{
    qDebug() << "Page received:" << pageURI.toString();
}

void router::requested(const QUrl &pageURI)
{
    qDebug() << "Page requested:" << pageURI.toString();
}
