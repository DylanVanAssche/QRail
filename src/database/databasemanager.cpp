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
/** @file */
#include "database/databasemanager.h"

using namespace QRail;
QRail::Database::Manager *QRail::Database::Manager::m_instance = nullptr;
QThreadStorage<QSqlDatabase> QRail::Database::Manager::m_database;
QThreadStorage<QString> QRail::Database::Manager::m_path;

QRail::Database::Manager::Manager(const QString &path, QObject *parent): QObject(parent)
{
    if (QSqlDatabase::isDriverAvailable(DRIVER)) {
        m_path.setLocalData(path);
    } else {
        qCritical() << "Missing support for SQL driver:" << DRIVER;
    }
}

QRail::Database::Manager *QRail::Database::Manager::getInstance(const QString &path)
{
    // NICE-TO-HAVE: Allow access to multiple databases by checking the path of
    // the database
    if (m_instance == nullptr) {
        qDebug() << "Creating new QRail::Database::Manager";
        m_instance = new Manager(path);
    }
    return m_instance;
}

bool QRail::Database::Manager::execute(QSqlQuery &query)
{
    if (this->database().isOpen() && query.exec()) {
        return true;
    } else {
        qCritical() << "Executing query:" << query.executedQuery() << "FAILED:" << query.lastError().text() << "DB OPEN?" << this->database().isOpen();
        return false;
    }
}

bool QRail::Database::Manager::startTransaction()
{
    bool result = this->database().transaction();
    return result;
}

bool QRail::Database::Manager::endTransaction()
{
    bool result = this->database().commit();
    return result;
}

QSqlDatabase QRail::Database::Manager::database() const
{
    if(!m_database.hasLocalData()) {
        qDebug() << "No local DB connection for this thread, creating one";
        QSqlDatabase db = QSqlDatabase::addDatabase(DRIVER, QUuid::createUuid().toString());
        db.setDatabaseName(m_path.localData());
        db.open();
        m_database.setLocalData(db);
    }
    return m_database.localData();
}
