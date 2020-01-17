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
    explicit router(QString departureStation, QString arrivalStation, QString departureTime, QString maxTransfers, QString mode, QObject *parent = nullptr);

public slots:
    void processRoutesFinished(QRail::RouterEngine::Journey *journey);
    void processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route);
    void processing(const QUrl &pageURI);
    void requested(const QUrl &pageURI);
    void updateReceived(qint64 time);
    void route();

private:
    QRail::RouterEngine::Planner *planner;
    QDateTime timestamp;
    QString m_from;
    QString m_to;
    QString m_departureTime;
    QString m_maxTransfers;
};

#endif // ROUTER_H
