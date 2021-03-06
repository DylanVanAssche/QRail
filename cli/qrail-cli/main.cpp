#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

#include "router.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("qrail-cli");
    QCoreApplication::setApplicationVersion("0.0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("CLI QRail application. Plan your route using Linked Connections.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("departure-station", QCoreApplication::translate("main", "Departure station URI."));
    parser.addPositionalArgument("arrival-station", QCoreApplication::translate("main", "Arrival station URI."));
    parser.addPositionalArgument("departure-time", QCoreApplication::translate("main", "Departure time in ISO string format."));
    parser.addPositionalArgument("max-transfers", QCoreApplication::translate("main", "The maximum amount of transfers that are allowed for the journey."));
    QCommandLineOption switchToSSE("sse", QCoreApplication::translate("main", "Switch to SSE mode, default none reference mode"));
    QCommandLineOption switchToPolling("polling", QCoreApplication::translate("main", "Switch to SSE mode, default none reference mode"));
    QCommandLineOption enableVerbose("verbose", QCoreApplication::translate("main", "Print verbose information during CSA routing"));
    parser.addOption(switchToSSE);
    parser.addOption(switchToPolling);
    parser.addOption(enableVerbose);

    // Process the actual command line arguments given by the user
    parser.process(app);
    const QStringList args = parser.positionalArguments();
    if(args.length() != 4) {
        qCritical() << "Usage: ./qrail-cli <DEPARTURE STATION> <ARRIVAL STATION> <DEPARTURE TIME> <MAX TRANSFERS>. See ./qrail-cli -h for more information";
        return 1;
    }
    if(parser.isSet(switchToSSE) && parser.isSet(switchToPolling)) {
        qCritical() << "Choose either --sse or --polling, cannot do both at the same time";
        return 2;
    }

    // Read arguments
    const QString departureStation = args.at(0);
    const QString arrivalStation = args.at(1);
    const QString departureTime = args.at(2);
    const QString maxTransfers = args.at(3);
    QString mode = "none"; // reference mode, no rollback
    if(parser.isSet(switchToSSE)) {
         mode = "sse";
    }
    else if(parser.isSet(switchToPolling)) {
        mode = "polling";
    }
    const bool verbose = parser.isSet(enableVerbose);

    qInfo() << "Starting QRail with the following parameters:";
    qInfo() << "\tDeparture station:" << departureStation;
    qInfo() << "\tArrival station:" << arrivalStation;
    qInfo() << "\tDeparture time (UTC):" << departureTime;
    qInfo() << "\tCurrent time (UTC):" << QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    qInfo() << "\tMax transfers:" << maxTransfers;
    qInfo() << "\tMode:" << mode;
    qInfo() << "\tVerbose:" << verbose;

    router* r = new router(departureStation, arrivalStation, departureTime, maxTransfers, mode, verbose);

    // We need to use the event loop for deleteLater();
    QTimer::singleShot(0, r, SLOT(route()));

    app.exec();
}
