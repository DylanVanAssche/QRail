#ifndef ROUTER_H
#define ROUTER_H

#include <QObject>
#include <QUrl>
#include <QDebug>

#include "qrail.h"
#include "engines/router/routerplanner.h"

class router : public QObject
{
    Q_OBJECT
public:
    explicit router(QObject *parent = nullptr);
    void route(QString from, QString to, QString departureTime, QString maxTransfers);

public slots:
    void processRoutesFinished(QRail::RouterEngine::Journey *journey);
    void processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route);
    void processing(const QUrl &pageURI);
    void requested(const QUrl &pageURI);

private:
    QRail::RouterEngine::Planner *planner;
};

#endif // ROUTER_H
