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
    explicit router(QString departureStation,
                    QString arrivalStation,
                    QString departureTime,
                    QString maxTransfers,
                    QString mode,
                    bool verbose,
                    QObject *parent = nullptr);

public slots:
    void processRoutesFinished(QRail::RouterEngine::Journey *journey);
    void processRoutesStream(QSharedPointer<QRail::RouterEngine::Route> route);
    void route();
    void updateReceived();

private:
    QRail::RouterEngine::Planner *planner;
    QDateTime m_started_at;
    QString m_from;
    QString m_to;
    QString m_departureTime;
    QString m_maxTransfers;
    bool m_verbose;
};

#endif // ROUTER_H
