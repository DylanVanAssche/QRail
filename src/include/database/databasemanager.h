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
#include <QtCore/QUuid>
#include <QtCore/QFuture>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtCore/QThreadStorage>

// Select here the right DB driver for Qt
#define DRIVER "QSQLITE"

// Singleton pattern
namespace QRail {
namespace Database {
/**
 * @class Manager
 * @brief The Database::Manager class manages a given database.
 * Using the Database::Manager you can be sure that only 1 object is interacting with the database, thanks to the Singleton design pattern.<br>
 * The Database::Manager allows the user to perform queries with or without transaction support in an easy way.<br>
 * Transparant support for multiple threads is also implemented by using a QThreadStorage class.<br>
 */
class Manager : public QObject
{
    Q_OBJECT
public:

    static Manager *getInstance(const QString &path);
    //! Executes a given QSqlQuery
    /*!
        \param query the SQL query to execute.
        \return True if success.
        \public
        Executes the given QSqlQuery query on the active database.<br>
        Before the execution takes place, the connection is checked.<br>
        During the execution, the errors are catched and logged as CRITICAL.<br>
    */
    bool execute(QSqlQuery &query);
    //! Starts the transaction
    /*!
        \return True if success.
        \public
        Starts a transaction in the database.<br>
        After you are done with your changes you should
        call QRail::Database::Manager::endTransaction() to
        commit your changes.
     */
    bool startTransaction();
    //! Ends the transaction.
    /*!
        \return True if success.
        \note This method won't do anything if no transaction was running.
        \public
        Commits changes to the database and returns true if success.
     */
    bool endTransaction();
    //! Gets the current QSqlDatabase database.
    /*!
      \return A QSqlDatabase object.
      \public
      Gets the associated QSqlDatabase database object and returns it.
    */
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
