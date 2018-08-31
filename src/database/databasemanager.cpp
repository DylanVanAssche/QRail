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
#include "database/databasemanager.h"
using namespace QRail;
QRail::Database::Manager *QRail::Database::Manager::m_instance = nullptr;

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 20 Jul 2018
 * @brief Manager facade constructor
 * @param QObject *parent
 * @param QString path
 * @package Database
 * @public
 * Constructs a Manager facade to access the database using the SQL language.
 * The Manager facade makes database access in Qt abstract from the underlying
 * database (SQLite, MySQL, ORACLE, ...). Any errors during initialisation of
 * the database are catched and logged as CRITICAL.
 */
QRail::Database::Manager::Manager(const QString &path, QObject *parent): QObject(parent) {
    if (QSqlDatabase::isDriverAvailable(DRIVER)) {
        this->setDatabase(QSqlDatabase::addDatabase(DRIVER));
        this->database().setDatabaseName(path);
        qDebug() << "Database name:" << this->database().databaseName();

        if (!this->database().open()) {
            qCritical() << "Connection to database failed";
        } else {
            qInfo() << "Database connection OK";
        }
    } else {
        qCritical() << "Missing support for SQL driver:" << DRIVER;
    }
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 21 Jul 2018
 * @brief Gets a QRail::Database::Manager instance
 * @return QRail::Database::Manager *manager
 * @package Database
 * @public
 * Constructs a QRail::Database::Manager instance if none exists and returns it.
 */
QRail::Database::Manager *QRail::Database::Manager::getInstance(const QString &path) {
    // NICE-TO-HAVE: Allow access to multiple databases by checking the path of
    // the database
    if (m_instance == nullptr) {
        qDebug() << "Creating new QRail::Database::Manager";
        m_instance = new Manager(path);
    }
    return m_instance;
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 20 Jul 2018
 * @brief Executes a given QSqlQuery
 * @param QSqlQuery query
 * @return bool success
 * @package Database
 * @public
 * Executes the given QSqlQuery query on the active database.
 * Before the execution takes place, the connection is checked.
 * During the execution, the errors are catched and logged as CRITICAL.
 */
bool QRail::Database::Manager::execute(QSqlQuery &query) {
    if (this->database().isOpen() && query.exec()) {
        return true;
    } else {
        qCritical() << "Executing querry:" << query.executedQuery() << "FAILED:" << query.lastError().text();
        return false;
    }
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 13 Aug 2018
 * @brief Executes a given QSqlQuery asynchronous
 * @param QSqlQuery query
 * @return bool success
 * @package Database
 * @public
 * Executes the given QSqlQuery query asynchronous on the active database.
 * Before the execution takes place, the connection is checked.
 * During the execution, the errors are catched and logged as CRITICAL.
 */
QFuture<bool> QRail::Database::Manager::executeAsync(QSqlQuery &query) {
    QFuture<bool> future = QtConcurrent::run(this, &QRail::Database::Manager::execute, query);
    return future;
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 9 Aug 2018
 * @brief Starts the transaction
 * @return bool success
 * @package Database
 * @public
 * Starts a transaction in the database.
 * After you are done with your changes you should
 * call QRail::Database::Manager::endTransaction() to
 * commit your changes.
 */
bool QRail::Database::Manager::startTransaction() {
    return this->database().transaction();
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 9 Aug 2018
 * @brief Ends the transaction
 * @return bool success
 * @package Database
 * @warning This method won't do anything if no transaction was running.
 * @public
 * Commits changes to the database and returns true if success.
 */
bool QRail::Database::Manager::endTransaction() {
    return this->database().commit();
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 20 Jul 2018
 * @brief Sets the QSqlDatabase database
 * @param const QSqlDatabase &database
 * @package Database
 * @private
 * Sets the current QSqlDatabase database to the given QSqlDatabase &database.
 */
void QRail::Database::Manager::setDatabase(const QSqlDatabase &database) {
    m_database = database;
}

/**
 * @file databasemanager.cpp
 * @author Dylan Van Assche
 * @date 20 Jul 2018
 * @brief Gets the current QSqlDatabase database
 * @return QSqlDatabase database
 * @package Database
 * @public
 * Gets the QSqlDatabase database and returns it.
 */
QSqlDatabase QRail::Database::Manager::database() const {
    return m_database;
}
