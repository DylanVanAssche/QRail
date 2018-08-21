#include "liveboardfactorytest.h"

void Liveboard::FactoryTest::initLiveboardFactoryTest()
{
    qDebug() << "Init Liveboard::Factory test";
    factory = Liveboard::Factory::getInstance();
    connect(factory, SIGNAL(liveboardReady(Liveboard::Board*)), this, SLOT(liveboardReceived(Liveboard::Board*)));
}

void Liveboard::FactoryTest::runLiveboardFactoryTest()
{
    qDebug() << "Running Liveboard::Factory test";

    // Activate QSignalSpy
    qRegisterMetaType< Liveboard::Board* >("Liveboard::Board"); // register custom class
    QSignalSpy spyLiveboard(factory, SIGNAL(liveboardReady(Liveboard::Board*)));

    qInfo() << "Liveboard arrivals (now) for station Vilvoorde";
    factory->getLiveboardByStationURI(
                QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
                Liveboard::Board::Mode::ARRIVALS
                );
    QVERIFY(spyLiveboard.wait(LIVEBOARD_WAIT_TIME));

    qInfo() << "Liveboard departures (now) for station Vilvoorde";
    factory->getLiveboardByStationURI(
                QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
                Liveboard::Board::Mode::DEPARTURES
                );
    QVERIFY(spyLiveboard.wait(LIVEBOARD_WAIT_TIME));
}

void Liveboard::FactoryTest::cleanLiveboardFactoryTest()
{
    qDebug() << "Cleaning up Liveboard::Factory test";
}

void Liveboard::FactoryTest::liveboardReceived(Liveboard::Board *board)
{
    qDebug() << "Received liveboard from Liveboard::Factory for station" << board->station()->name().value(QLocale::Language::Dutch);
    qDebug() << "\tFrom:" << board->from();
    qDebug() << "\tUntil:" << board->until();
    qDebug() << "\tMode:" << board->mode();
    qDebug() << "\tNumber of entries:" << board->entries().size();
    qDebug() << "\tEntries:";

    foreach(CSA::VehicleStop *entry, board->entries()) {
        if(board->mode() == Liveboard::Board::Mode::ARRIVALS) {
            qDebug() << "\t\t" << entry->vehicleInformation()->headsign() << entry->arrivalTime().toString("hh:mm");
        }
        else if(board->mode() == Liveboard::Board::Mode::DEPARTURES) {
            qDebug() << "\t\t" << entry->vehicleInformation()->headsign() << entry->departureTime().toString("hh:mm");
        }
        else {
            qCritical() << "Unknown Liveboard::Board mode!";
        }
    }
}
