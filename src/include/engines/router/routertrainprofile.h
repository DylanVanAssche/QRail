/*
*   This file is part of QRail.
*
*   QRail is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   QRail is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with QRail.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef CSATRAINPROFILE_H
#define CSATRAINPROFILE_H

#include <QtCore/QObject>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>
#include "fragments/fragmentsfragment.h"

namespace QRail {
namespace RouterEngine {
class TrainProfile : public QObject
{
    Q_OBJECT
public:
    explicit TrainProfile(
        const QDateTime &arrivalTime,
        QSharedPointer<QRail::Fragments::Fragment> arrivalConnection,
        const qint16 transfers,
        QObject *parent = nullptr
    );
    QDateTime arrivalTime() const;
    void setArrivalTime(const QDateTime &arrivalTime);
    QSharedPointer<QRail::Fragments::Fragment> arrivalConnection() const;
    void setArrivalConnection(QSharedPointer<QRail::Fragments::Fragment> arrivalConnection);
    qint16 transfers() const;
    void setTransfers(const qint16 &transfers);

signals:
    void arrivalTimeChanged();
    void arrivalConnectionChanged();
    void transfersChanged();

private:
    QDateTime m_arrivalTime;
    QSharedPointer<QRail::Fragments::Fragment> m_arrivalConnection;
    qint16 m_transfers;
};
}
}

#endif // CSATRAINPROFILE_H
