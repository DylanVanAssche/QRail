#ifndef LINKEDCONNECTIONFACTORYTEST_H
#define LINKEDCONNECTIONFACTORYTEST_H

#include <QObject>
#include <QtTest/QtTest>
#include <QSignalSpy>
#include <QMetaType>
#include <QtSql/QSqlQuery>
#include "fragments/fragmentsfactory.h"
#include "database/databasemanager.h"
#include "fragments/fragmentspage.h"

#define SIGNAL_WAIT_TIME 3000

namespace Fragments {
class FactoryTest : public QObject
{
    Q_OBJECT
private slots:
    void initLinkedConnectionFactory();
    void runLinkedConnectionFactory();
    void cleanLinkedConnectionFactory();

private:
    Fragments::Factory *factory;
    Database::Manager *db;
};
}

#endif // LINKEDCONNECTIONFACTORYTEST_H
