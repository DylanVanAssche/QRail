#ifndef LIVEBOARDFACTORYTEST_H
#define LIVEBOARDFACTORYTEST_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include "engines/liveboard/liveboardfactory.h"

#define LIVEBOARD_WAIT_TIME 45000

namespace LiveboardEngine {
class FactoryTest : public QObject
{
    Q_OBJECT
public slots:
    void liveboardReceived(LiveboardEngine::Board *board);

private slots:
    void initLiveboardFactoryTest();
    void runLiveboardFactoryTest();
    void cleanLiveboardFactoryTest();

private:
    LiveboardEngine::Factory *factory;
};
}

#endif // LIVEBOARDFACTORYTEST_H
