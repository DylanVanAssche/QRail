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
    QCommandLineOption switchToSSE("sse", QCoreApplication::translate("main", "Switch to SSE mode, default polling mode"));
    parser.addOption(switchToSSE);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if(args.length() != 4) {
        qCritical() << "Usage: ./qrail-cli <DEPARTURE STATION> <ARRIVAL STATION> <DEPARTURE TIME> <MAX TRANSFERS>. See ./qrail-cli -h for more information";
        return 1;
    }
    const QString departureStation = args.at(0);
    const QString arrivalStation = args.at(1);
    const QString departureTime = args.at(2);
    const QString maxTransfers = args.at(3);
    const QString mode = parser.isSet(switchToSSE)? "sse": "polling";

    qInfo() << "Starting QRail with the following parameters:";
    qInfo() << "\tDeparture station:" << departureStation;
    qInfo() << "\tArrival station:" << arrivalStation;
    qInfo() << "\tDeparture time:" << departureTime;
    qInfo() << "\tMax transfers:" << maxTransfers;
    qInfo() << "\tMode:" << mode;

    router* r = new router(mode);
    r->route(departureStation, arrivalStation, departureTime, maxTransfers);

    app.exec();
}
