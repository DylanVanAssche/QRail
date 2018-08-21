#ifndef LIVEBOARDFACTORYTEST_H
#define LIVEBOARDFACTORYTEST_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include "liveboard/liveboardfactory.h"

#define LIVEBOARD_WAIT_TIME 15000

namespace Liveboard {
class FactoryTest : public QObject
{
    Q_OBJECT
public slots:
    void liveboardReceived(Liveboard::Board *board);

private slots:
    void initLiveboardFactoryTest();
    void runLiveboardFactoryTest();
    void cleanLiveboardFactoryTest();

private:
    Liveboard::Factory *factory;
};
}

#endif // LIVEBOARDFACTORYTEST_H
