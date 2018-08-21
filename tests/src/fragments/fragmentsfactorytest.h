#ifndef FRAGMENTSFACTORYTEST_H
#define FRAGMENTSFACTORYTEST_H

#include <QtCore/QObject>
#include <QtCore/QMetaType>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

#include "fragments/fragmentsfactory.h"
#include "database/databasemanager.h"
#include "fragments/fragmentspage.h"

#define FRAGMENT_WAIT_TIME 3000

namespace Fragments {
class FactoryTest : public QObject
{
    Q_OBJECT
private slots:
    void initFragmentsFactoryTest();
    void runFragmentsFactoryTest();
    void cleanFragmentsFactoryTest();

private:
    Fragments::Factory *factory;
    Database::Manager *db;
};
}

#endif // FRAGMENTSFACTORYTEST_H
