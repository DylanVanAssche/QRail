#ifndef CSAPLANNERTEST_H
#define CSAPLANNERTEST_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>
#include "../src/linkedconnections/csa/csaplanner.h"

namespace CSA {
class PlannerTest : public QObject
{
    Q_OBJECT
private slots:
    void initCSAPlannerTest();
    void runCSAPlannerTest();
    void cleanCSAPlannerTest();

public slots:
    void processRouteFound(const QList<CSA::Route *> &routes);
    void pageProcess(const QUrl &pageURI, const qint16 &progress);
    void pageReceived(const QUrl &pageURI);
    void pageRequested(const QUrl &pageURI);

private:
    CSA::Planner *planner;
};
}

#endif // CSAPLANNERTEST_H
