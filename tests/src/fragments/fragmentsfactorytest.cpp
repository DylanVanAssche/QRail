#include "fragmentsfactorytest.h"

void Fragments::FactoryTest::initFragmentsFactoryTest()
{
    qDebug() << "Init Fragments::Factory test";
    factory = Fragments::Factory::getInstance();
}

void Fragments::FactoryTest::runFragmentsFactoryTest()
{
    qDebug() << "Running Fragments::Factory test";

    // Activate QSignalSpy
    qRegisterMetaType< Fragments::Page* >("Fragments::Page"); // register custom class
    QSignalSpy spyPage(factory, SIGNAL(pageReady(Fragments::Page*)));

    // Linked Connections page should be loaded from the SQLite DB (if this test already ran once)
    factory->getPage(QUrl("https://graph.irail.be/sncb/connections?departureTime=2018-07-21T07:29:00.000Z"));
    QVERIFY(spyPage.wait(FRAGMENT_WAIT_TIME));

    factory->getPage(QUrl("https://graph.irail.be/sncb/connections?departureTime=2018-07-21T08:14:00.000Z"));
    QVERIFY(spyPage.wait(FRAGMENT_WAIT_TIME));

    factory->getPage(QUrl("https://graph.irail.be/sncb/connections?departureTime=2018-07-21T06:45:00.000Z"));
    QVERIFY(spyPage.wait(FRAGMENT_WAIT_TIME));

    // Linked Connections page should be loaded from the Linked Connections API (if it's a new page)
    factory->getPage(QDateTime::currentDateTime());
    QVERIFY(spyPage.wait(FRAGMENT_WAIT_TIME));
}

void Fragments::FactoryTest::cleanFragmentsFactoryTest()
{
    qDebug() << "Cleaning up Fragments::Fragment test";
}
