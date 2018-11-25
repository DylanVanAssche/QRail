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
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtCore/QtGlobal>
#include <QtCore/QtDebug>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QDir>
#include <QtCore/QFuture>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtConcurrent/QtConcurrent>
#include <QtCore/QThreadStorage>

// Select here the right DB driver for Qt
#define DRIVER "QSQLITE"

// Singleton pattern
namespace QRail {
namespace Database {
class Manager : public QObject
{
    Q_OBJECT
public:
    static Manager *getInstance(const QString &path);
    bool execute(QSqlQuery &query);
    bool startTransaction();
    bool endTransaction();
    QSqlDatabase database() const;

private:
    static QThreadStorage<QSqlDatabase> m_database;
    static QThreadStorage<QString> m_path;
    explicit Manager(const QString &path, QObject *parent = nullptr);
    static Manager *m_instance;
};
}
}

#endif // DATABASEMANAGER_H
