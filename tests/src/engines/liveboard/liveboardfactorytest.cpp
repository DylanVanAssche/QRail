#include "liveboardfactorytest.h"

void LiveboardEngine::FactoryTest::initLiveboardFactoryTest()
{
    qDebug() << "Init LiveboardEngine::Factory test";
    factory = LiveboardEngine::Factory::getInstance();
    connect(factory, SIGNAL(finished(LiveboardEngine::Board*)), this, SLOT(liveboardReceived(LiveboardEngine::Board*)));
}

void LiveboardEngine::FactoryTest::runLiveboardFactoryTest()
{
    qDebug() << "Running LiveboardEngine::Factory test";

    // Activate QSignalSpy
    qRegisterMetaType< LiveboardEngine::Board* >("LiveboardEngine::Board"); // register custom class
    QSignalSpy spyLiveboard(factory, SIGNAL(finished(LiveboardEngine::Board*)));

    qDebug() << "Liveboard arrivals (now) for station Vilvoorde";
    factory->getLiveboardByStationURI(
                QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
                LiveboardEngine::Board::Mode::ARRIVALS
                );
    QVERIFY(spyLiveboard.wait(LIVEBOARD_WAIT_TIME));

    qDebug() << "Liveboard departures (now) for station Vilvoorde";
    factory->getLiveboardByStationURI(
                QUrl("http://irail.be/stations/NMBS/008811189"), // Vilvoorde
                LiveboardEngine::Board::Mode::DEPARTURES
                );
    QVERIFY(spyLiveboard.wait(LIVEBOARD_WAIT_TIME));
}

void LiveboardEngine::FactoryTest::cleanLiveboardFactoryTest()
{
    qDebug() << "Cleaning up LiveboardEngine::Factory test";
}

void LiveboardEngine::FactoryTest::liveboardReceived(LiveboardEngine::Board *board)
{
    qDebug() << "Received liveboard from LiveboardEngine::Factory for station" << board->station()->name().value(QLocale::Language::Dutch);
    qDebug() << "\tFrom:" << board->from();
    qDebug() << "\tUntil:" << board->until();
    qDebug() << "\tMode:" << board->mode();
    qDebug() << "\tNumber of entries:" << board->entries().size();
    qDebug() << "\tEntries:";

    foreach(VehicleEngine::Vehicle *entry, board->entries()) {
        if(board->mode() == LiveboardEngine::Board::Mode::ARRIVALS) {
            qDebug() << "\t\t" << entry->headsign() << entry->intermediaryStops().first()->arrivalTime().toString("hh:mm");
        }
        else if(board->mode() == LiveboardEngine::Board::Mode::DEPARTURES) {
            qDebug() << "\t\t" << entry->headsign() << entry->intermediaryStops().first()->departureTime().toString("hh:mm");
        }
        else {
            qCritical() << "Unknown LiveboardEngine::Board mode!";
        }
    }
}
