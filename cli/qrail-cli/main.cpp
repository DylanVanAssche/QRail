#include <QCoreApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("qrail-cli");
    QCoreApplication::setApplicationVersion("0.0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Test helper");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("departure-station", QCoreApplication::translate("main", "Departure station URI."));
    parser.addPositionalArgument("arrival-station", QCoreApplication::translate("main", "Arrival station URI."));
    parser.addPositionalArgument("departure-time", QCoreApplication::translate("main", "Departure time in ISO string format."));
    QCommandLineOption switchToSSE("sse", QCoreApplication::translate("main", "Switch to SSE mode, default polling mode"));
    parser.addOption(switchToSSE);

    // Process the actual command line arguments given by the user
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    if(args.length() != 3) {
        qCritical() << "Usage: ./qrail-cli <DEPARTURE STATION> <ARRIVAL STATION> <DEPARTURE TIME>. See ./qrail-cli -h for more information";
        return 1;
    }
    const QString departureStation = args.at(0);
    const QString arrivalStation = args.at(1);
    const QString departureTime = args.at(2);
    const QString mode = parser.isSet(switchToSSE)? "sse": "polling";

    qInfo() << "Starting QRail with the following parameters:";
    qInfo() << "\tDeparture station:" << departureStation;
    qInfo() << "\tArrival station:" << arrivalStation;
    qInfo() << "\tDeparture time:" << departureTime;
    qInfo() << "\tMode:" << mode;
}
