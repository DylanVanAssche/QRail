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
#include "engines/station/stationfactory.h"
using namespace QRail;
StationEngine::Factory *StationEngine::Factory::m_instance = nullptr;

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief StationEngine::Factory constructor: empty
 * @param QObject *parent = nullptr
 * @package StationEngine
 * @private
 * Constructs a StationEngine::Factory to
 * generate StationEngine::Station objects on the fly.
 */
StationEngine::Factory::Factory(QObject *parent) : QObject(parent) {
    // Make sure that the DB path is writable and created
    QString path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dbDirectory;
    dbDirectory.mkpath(path);

    // Setup DB
    this->setDb(QRail::Database::Manager::getInstance(path + "/stations.db"));
    this->initDatabase();

    // Init caching
    m_cache = QMap<QUrl, StationEngine::Station*>();
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets a StationEngine::Factory instance
 * @param QObject *parent = nullptr
 * @return StationEngine::Factory *factory
 * @package StationEngine
 * @public
 * Constructs a StationEngine::Factory if none exists and returns the instance.
 */
StationEngine::Factory *StationEngine::Factory::getInstance() {
    // Singleton pattern
    if (m_instance == nullptr) {
        qDebug() << "Generating new StationEngine::Factory";
        m_instance = new StationEngine::Factory();
    }
    return m_instance;
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Retrieves a station by URI
 * @param const QUrl &uri
 * @return StationEngine::Station *station
 * @package StationEngine
 * @public
 * Retrieves a station by URI from the database.
 * In case something goes wrong, a StationEngine::NullStation instance is
 * returned.
 */
StationEngine::Station *
StationEngine::Factory::getStationByURI(const QUrl &uri) {
    // Init StationEngine::Station variable
    StationEngine::Station *station;

    // Try to get the station from the cache
    station = this->fetchStationFromCache(uri);

    // Station isn't cached yet
    if (!station) {
        // Fetch station from database
        QSqlQuery query(this->db()->database());
        query.prepare("SELECT "
                      "uri, "
                      "name, "
                      "alternativeFR, "
                      "alternativeNL, "
                      "alternativeDE, "
                      "alternativeEN, "
                      "street, "
                      "zip, "
                      "city, "
                      "countryCode, "
                      "longitude, "
                      "latitude, "
                      "ticketVendingMachine, "
                      "luggageLockers, "
                      "freeParking, "
                      "taxi, "
                      "bicycleSpots, "
                      "blueBike, "
                      "bus, "
                      "tram, "
                      "metro, "
                      "wheelchairAvailable, "
                      "ramp, "
                      "disabledParkingSpots, "
                      "elevatedPlatform, "
                      "escalatorUp, "
                      "escalatorDown, "
                      "elevatorPlatform, "
                      "hearingAidSignal, "
                      "salesOpenMonday, "
                      "salesCloseMonday, "
                      "salesOpenTuesday, "
                      "salesCloseTuesday, "
                      "salesOpenWednesday, "
                      "salesCloseWednesday, "
                      "salesOpenThursday, "
                      "salesCloseThursday, "
                      "salesOpenFriday, "
                      "salesCloseFriday, "
                      "salesOpenSaturday, "
                      "salesCloseSaturday, "
                      "salesOpenSunday, "
                      "salesCloseSunday, "
                      "avgStopTimes "
                      "FROM stations "
                      "WHERE uri = :uri");
        query.bindValue(":uri", uri); // Match page URI's
        this->db()->execute(query);

        // Read result and create a new Station object
        while (query.next()) {
            // Using the field name in overload query.value(x) is less efficient then
            // using indexes according to the Qt 5.6.3 docs
            QUrl uri = query.value(0).toUrl();

            // Convert to QMap, check always if an alternative name if available. If
            // not, insert to default one for that language.
            QMap<QLocale::Language, QString> name;
            QList<QLocale::Language> languages;
            languages << QLocale::Language::C << QLocale::Language::French
                      << QLocale::Language::Dutch << QLocale::Language::German
                      << QLocale::Language::English;
            qint16 i = 1; // CSV file: station names start at index 1
            foreach (QLocale::Language language, languages) {
                QString localizedName = query.value(i).toString();
                QString defaultName = query.value(1).toString();

                if (localizedName.length() > 0) {
                    name.insert(language, localizedName);
                } else {
                    name.insert(language, defaultName);
                }
                i++;
            }

            // Convert country code to QLocale::Country enum
            QLocale::Country country; // Unknown at first
            QString countryCode = query.value(9).toString();
            if (countryCode == "be") {
                country = QLocale::Country::Belgium;
            } else if (countryCode == "nl") {
                country = QLocale::Country::Netherlands;
            } else if (countryCode == "gb") {
                country = QLocale::Country::UnitedKingdom;
            } else if (countryCode == "lu") {
                country = QLocale::Country::Luxembourg;
            } else if (countryCode == "ch") {
                country = QLocale::Country::Switzerland;
            } else if (countryCode == "fr") {
                country = QLocale::Country::France;
            } else if (countryCode == "de") {
                country = QLocale::Country::Germany;
            } else {
                country =
                        QLocale::Country::Belgium; // Qt 5.6 lacks an unknown country enum
            }

            // Convert latitude and longitude to QGeoCoordinate
            QGeoCoordinate position;
            position.setLongitude(query.value(10).toDouble());
            position.setLatitude(query.value(11).toDouble());

            // Fetch the average stop times for this station
            qreal averageStopTimes = query.value(43).toDouble();

            // Get the platform data for this station
            QMap<QUrl, QString> platforms = this->getPlatformsByStationURI(uri);

            // Only process the following fields if any facility data is available
            // We check this by looking at the full address of the station, if that's
            // missing then probably all the rest of the data will be missing too.
            bool hasFacilities = query.value(6).toString().length() > 0 &&
                    query.value(7).toString().length() > 0 &&
                    query.value(8).toString().length() > 0;
            if (hasFacilities) {
                // Convert street, zip and city to QGeoAddress
                QGeoAddress address;
                address.setStreet(query.value(6).toString());
                address.setPostalCode(query.value(7).toString());
                address.setCity(query.value(8).toString());

                bool hasTicketVendingMachine = query.value(12).toBool();
                bool hasLuggageLockers = query.value(13).toBool();
                bool hasFreeParking = query.value(14).toBool();
                bool hasTaxi = query.value(15).toBool();
                bool hasBicycleSpots = query.value(16).toBool();
                bool hasBlueBike = query.value(17).toBool();
                bool hasBus = query.value(18).toBool();
                bool hasTram = query.value(19).toBool();
                bool hasMetro = query.value(20).toBool();
                bool hasWheelchairAvailable = query.value(21).toBool();
                bool hasRamp = query.value(22).toBool();
                qint16 disabledParkingSpots = query.value(23).toInt();
                bool hasElevatedPlatform = query.value(24).toBool();
                bool hasEscalatorUp = query.value(25).toBool();
                bool hasEscalatorDown = query.value(26).toBool();
                bool hasElevatorPlatform = query.value(27).toBool();
                bool hasHearingAidSignal = query.value(28).toBool();

                // Convert openinghours to QMap
                QMap<StationEngine::Station::Day, QPair<QTime, QTime>>
                        openingHours; // Example: 06:45
                openingHours.insert(
                            StationEngine::Station::Day::MONDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(29).toString(), "hh:mm"),
                                QTime::fromString(query.value(30).toString(), "hh:mm"))
                            );
                openingHours.insert(
                            StationEngine::Station::Day::TUESDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(31).toString(), "hh:mm"),
                                QTime::fromString(query.value(32).toString(), "hh:mm"))
                            );
                openingHours.insert(
                            StationEngine::Station::Day::WEDNESDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(33).toString(), "hh:mm"),
                                QTime::fromString(query.value(34).toString(), "hh:mm"))
                            );
                openingHours.insert(
                            StationEngine::Station::Day::THURSDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(35).toString(), "hh:mm"),
                                QTime::fromString(query.value(36).toString(), "hh:mm"))
                            );
                openingHours.insert(
                            StationEngine::Station::Day::FRIDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(37).toString(), "hh:mm"),
                                QTime::fromString(query.value(38).toString(), "hh:mm"))
                            );
                openingHours.insert(
                            StationEngine::Station::Day::SATURDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(39).toString(), "hh:mm"),
                                QTime::fromString(query.value(40).toString(), "hh:mm"))
                            );
                openingHours.insert(
                            StationEngine::Station::Day::SUNDAY,
                            QPair<QTime, QTime>(
                                QTime::fromString(query.value(41).toString(), "hh:mm"),
                                QTime::fromString(query.value(42).toString(), "hh:mm"))
                            );

                station = new StationEngine::Station(
                            uri,
                            name,
                            country,
                            position,
                            address,
                            hasTicketVendingMachine,
                            hasLuggageLockers,
                            hasFreeParking,
                            hasTaxi,
                            hasBicycleSpots,
                            hasBlueBike,
                            hasBus,
                            hasTram,
                            hasMetro,
                            hasWheelchairAvailable,
                            hasRamp,
                            disabledParkingSpots,
                            hasElevatedPlatform,
                            hasEscalatorUp,
                            hasEscalatorDown,
                            hasElevatorPlatform,
                            hasHearingAidSignal,
                            openingHours,
                            averageStopTimes,
                            platforms
                            );
            } else {
                station = new StationEngine::Station(
                            uri,
                            name,
                            country,
                            position,
                            averageStopTimes,
                            platforms
                            );
            }

            // Add station to cache
            this->addStationToCache(station);
        }
    }
#ifdef VERBOSE_CACHE
    else {
        qDebug() << "Station" << uri.toString() << "found in cache.";
    }
#endif

    return station;
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Init the database for stations
 * @package StationEngine
 * @public
 * Init the database for stations by reading the CSV files from iRail using
 * QtCSV.
 */
bool StationEngine::Factory::initDatabase() {
    // On instantation, create the tables in the database if they don't exist yet.
    QSqlQuery query(this->db()->database());

    // Drop old tables, TODO check first the CSV file if an update is required
    bool success = query.prepare("DROP TABLE IF EXISTS stations");
    success = this->db()->execute(query);
    query.clear(); // Release resources for reuse

    success = query.prepare("DROP TABLE IF EXISTS platforms");
    success = this->db()->execute(query);
    query.clear();

    // STATIONS table
    success = query.prepare("CREATE TABLE IF NOT EXISTS stations ("
                            "uri TEXT PRIMARY KEY, "
                            "name TEXT NOT NULL, "
                            "alternativeFR TEXT, "
                            "alternativeNL TEXT, "
                            "alternativeDE TEXT, "
                            "alternativeEN TEXT, "
                            "street TEXT, "
                            "zip INT, "
                            "city TEXT, "
                            "countryCode TEXT, "
                            "longitude REAL, "
                            "latitude REAL, "
                            "ticketVendingMachine TEXT, "
                            "luggageLockers INT, "
                            "freeParking INT, "
                            "taxi INT, "
                            "bicycleSpots INT, "
                            "blueBike INT, "
                            "bus INT, "
                            "tram INT, "
                            "metro INT, "
                            "wheelchairAvailable INT, "
                            "ramp INT, "
                            "disabledParkingSpots INT, "
                            "elevatedPlatform INT, "
                            "escalatorUp INT, "
                            "escalatorDown INT, "
                            "elevatorPlatform INT, "
                            "hearingAidSignal INT, "
                            "salesOpenMonday TEXT, "
                            "salesCloseMonday TEXT, "
                            "salesOpenTuesday TEXT, "
                            "salesCloseTuesday TEXT, "
                            "salesOpenWednesday TEXT, "
                            "salesCloseWednesday TEXT, "
                            "salesOpenThursday TEXT, "
                            "salesCloseThursday TEXT, "
                            "salesOpenFriday TEXT, "
                            "salesCloseFriday TEXT, "
                            "salesOpenSaturday TEXT, "
                            "salesCloseSaturday TEXT, "
                            "salesOpenSunday TEXT, "
                            "salesCloseSunday TEXT, "
                            "avgStopTimes REAL)");
    success = this->db()->execute(query);
    query.clear(); // Release resources for reuse

    // PLATFORMS table
    success = query.prepare("CREATE TABLE IF NOT EXISTS platforms ("
                            "uri TEXT PRIMARY KEY, "
                            "parentStop TEXT, "
                            "longitude REAL, "
                            "latitude REAL, "
                            "name TEXT, "
                            "platform TEXT, "
                            "FOREIGN KEY(parentStop) REFERENCES stations(uri))");
    success = this->db()->execute(query);

    if (success) {
        qDebug() << "Database init OK";
    } else {
        qCritical() << "Unable to create tables:" << query.lastError().text();
    }

    // Start DB transaction to speedup insertion
    this->db()->startTransaction();

    // Read CSV files using the QtCSV library
    QList<QStringList> stationsCSV =
            QtCSV::Reader::readToList(":/database/stations/stations.csv");
    QList<QStringList> facilitiesCSV =
            QtCSV::Reader::readToList(":/database/stations/facilities.csv");
    QList<QStringList> stopsCSV =
            QtCSV::Reader::readToList(":/database/stations/stops.csv");

    // Synchronise all QFutures at the end of the transaction
    QFutureSynchronizer<bool> synchronizer;

    // Loop through the stations CSV file and insert every station into the DB
    foreach (QStringList station, stationsCSV) {
        // We remove the title line from the CSV
        if (!QString(station.at(0)).startsWith("http")) {
            continue;
        }

        // Check if we have an URI match between both CSV files.
        qint16 facilityIndex = 0;
        for (qint16 i = 0; i < facilitiesCSV.size(); i++) {
            if (facilitiesCSV.at(i).at(0) == station.at(0)) {
                facilityIndex = i;
                break;
            }
        }

        // Only when we have an URI match between the facilitiesCSV and stationsCSV
        // we can create a complete Station object
        if (facilityIndex > 0) {
            synchronizer.addFuture(this->insertStationWithFacilitiesIntoDatabase(
                                       station, facilitiesCSV.at(facilityIndex)));
        } else {
            // qWarning() << "No facilties found for station:" << station.at(0);
            synchronizer.addFuture(
                        this->insertStationWithoutFacilitiesIntoDatabase(station));
        }
    }

    // Loop through the stops CSV file and insert every stop into the DB
    foreach (QStringList platform, stopsCSV) {
        // We remove the title line from the CSV
        if (!QString(platform.at(0)).startsWith("http")) {
            continue;
        }
        synchronizer.addFuture(this->insertPlatformIntoDatabase(platform));
    }

    // Insertion complete, synchronize everything and end the transaction
    foreach (QFuture<bool> future, synchronizer.futures()) {
        success = future.result();
        if (!success) {
            qCritical() << "Insertion failed";
            synchronizer.waitForFinished(); // Wait in case we have a failed insertion
            // for the rest of the QFutures
            break;
        }
    }
    this->db()->endTransaction();

    return success;
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Inserts a station with facilities into the database
 * @param const QStringList &station
 * @param const QStringList &facilities
 * @package StationEngine
 * @public
 * Inserts a station with facilities into the database from the CSV file.
 */
QFuture<bool> StationEngine::Factory::insertStationWithFacilitiesIntoDatabase(const QStringList &station, const QStringList &facilities) {
    QSqlQuery query(this->db()->database());
    query.prepare("INSERT INTO stations ("
                  "uri, "
                  "name, "
                  "alternativeFR, "
                  "alternativeNL, "
                  "alternativeDE, "
                  "alternativeEN, "
                  "street, "
                  "zip, "
                  "city, "
                  "countryCode, "
                  "longitude, "
                  "latitude, "
                  "ticketVendingMachine, "
                  "luggageLockers, "
                  "freeParking, "
                  "taxi, "
                  "bicycleSpots, "
                  "blueBike, "
                  "bus, "
                  "tram, "
                  "metro, "
                  "wheelchairAvailable, "
                  "ramp, "
                  "disabledParkingSpots, "
                  "elevatedPlatform, "
                  "escalatorUp, "
                  "escalatorDown, "
                  "elevatorPlatform, "
                  "hearingAidSignal, "
                  "salesOpenMonday, "
                  "salesCloseMonday, "
                  "salesOpenTuesday, "
                  "salesCloseTuesday, "
                  "salesOpenWednesday, "
                  "salesCloseWednesday, "
                  "salesOpenThursday, "
                  "salesCloseThursday, "
                  "salesOpenFriday, "
                  "salesCloseFriday, "
                  "salesOpenSaturday, "
                  "salesCloseSaturday, "
                  "salesOpenSunday, "
                  "salesCloseSunday, "
                  "avgStopTimes)"
                  " VALUES( "
                  ":uri, "
                  ":name, "
                  ":alternativeFR, "
                  ":alternativeNL, "
                  ":alternativeDE, "
                  ":alternativeEN, "
                  ":street, "
                  ":zip, "
                  ":city, "
                  ":countryCode, "
                  ":longitude, "
                  ":latitude, "
                  ":ticketVendingMachine, "
                  ":luggageLockers, "
                  ":freeParking, "
                  ":taxi, "
                  ":bicycleSpots, "
                  ":blueBike, "
                  ":bus, "
                  ":tram, "
                  ":metro, "
                  ":wheelchairAvailable, "
                  ":ramp, "
                  ":disabledParkingSpots, "
                  ":elevatedPlatform, "
                  ":escalatorUp, "
                  ":escaltorDown, "
                  ":elevatorPlatform, "
                  ":hearingAidSignal, "
                  ":salesOpenMonday, "
                  ":salesCloseMonday, "
                  ":salesOpenTuesday, "
                  ":salesCloseTuesday, "
                  ":salesOpenWednesday, "
                  ":salesCloseWednesday, "
                  ":salesOpenThursday, "
                  ":salesCloseThursday, "
                  ":salesOpenFriday, "
                  ":salesCloseFriday, "
                  ":salesOpenSaturday, "
                  ":salesCloseSaturday, "
                  ":salesOpenSunday, "
                  ":salesCloseSunday, "
                  ":avgStopTimes)");
    query.bindValue(":uri", station.at(0));  // Same as facilities.at(0)
    query.bindValue(":name", station.at(1)); // Same as facilties.at(1)
    query.bindValue(":alternativeFR", station.at(2));
    query.bindValue(":alternativeNL", station.at(3));
    query.bindValue(":alternativeDE", station.at(4));
    query.bindValue(":alternativeEN", station.at(5));
    query.bindValue(":street", facilities.at(2));
    query.bindValue(":zip", facilities.at(3).toInt());
    query.bindValue(":city", facilities.at(4));
    query.bindValue(":countryCode", station.at(6));
    query.bindValue(":longitude", station.at(7).toDouble());
    query.bindValue(":latitude", station.at(8).toDouble());
    query.bindValue(":ticketVendingMachine", facilities.at(5).toInt());
    query.bindValue(":luggageLockers", facilities.at(6).toInt());
    query.bindValue(":freeParking", facilities.at(7).toInt());
    query.bindValue(":taxi", facilities.at(8).toInt());
    query.bindValue(":bicycleSpots", facilities.at(9).toInt());
    query.bindValue(":blueBike", facilities.at(10).toInt());
    query.bindValue(":bus", facilities.at(11).toInt());
    query.bindValue(":tram", facilities.at(12).toInt());
    query.bindValue(":metro", facilities.at(13).toInt());
    query.bindValue(":wheelchairAvailable", facilities.at(14).toInt());
    query.bindValue(":ramp", facilities.at(15).toInt());
    query.bindValue(":disabledParkingSpots", facilities.at(16).toInt());
    query.bindValue(":elevatedPlatform", facilities.at(17).toInt());
    query.bindValue(":escalatorUp", facilities.at(18).toInt());
    query.bindValue(":escalatorDown", facilities.at(19).toInt());
    query.bindValue(":elevatorPlatform", facilities.at(20).toInt());
    query.bindValue(":hearingAidSignal", facilities.at(21));
    query.bindValue(":salesOpenMonday", facilities.at(22));
    query.bindValue(":salesCloseMonday", facilities.at(23));
    query.bindValue(":salesOpenTuesday", facilities.at(24));
    query.bindValue(":salesCloseTuesday", facilities.at(25));
    query.bindValue(":salesOpenWednesday", facilities.at(26));
    query.bindValue(":salesCloseWednesday", facilities.at(27));
    query.bindValue(":salesOpenThursday", facilities.at(28));
    query.bindValue(":salesCloseThursday", facilities.at(29));
    query.bindValue(":salesOpenFriday", facilities.at(30));
    query.bindValue(":salesCloseFriday", facilities.at(31));
    query.bindValue(":salesOpenSaturday", facilities.at(32));
    query.bindValue(":salesCloseSaturday", facilities.at(33));
    query.bindValue(":salesOpenSunday", facilities.at(34));
    query.bindValue(":salesCloseSunday", facilities.at(35));
    query.bindValue(":avgStopTimes", station.at(9));
    return this->db()->executeAsync(query);
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Inserts a station without facilities into the database
 * @param const QStringList &station
 * @package StationEngine
 * @public
 * Inserts a station without facilities into the database from the CSV file.
 */
QFuture<bool>
StationEngine::Factory::insertStationWithoutFacilitiesIntoDatabase(const QStringList &station) {
    QSqlQuery query(this->db()->database());
    query.prepare("INSERT INTO stations ("
                  "uri, "
                  "name, "
                  "alternativeFR, "
                  "alternativeNL, "
                  "alternativeDE, "
                  "alternativeEN, "
                  "avgStopTimes)"
                  " VALUES( "
                  ":uri, "
                  ":name, "
                  ":alternativeFR, "
                  ":alternativeNL, "
                  ":alternativeDE, "
                  ":alternativeEN, "
                  ":avgStopTimes)");
    query.bindValue(":uri", station.at(0));
    query.bindValue(":name", station.at(1));
    query.bindValue(":alternativeFR", station.at(2));
    query.bindValue(":alternativeNL", station.at(3));
    query.bindValue(":alternativeDE", station.at(4));
    query.bindValue(":alternativeEN", station.at(5));
    query.bindValue(":avgStopTimes", station.at(9));
    return this->db()->executeAsync(query);
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Inserts a platform into the database
 * @param const QStringList &platform
 * @package StationEngine
 * @public
 * Inserts a platform into the database from the CSV file.
 */
QFuture<bool> StationEngine::Factory::insertPlatformIntoDatabase(const QStringList &platform) {
    QSqlQuery query(this->db()->database());
    query.prepare("INSERT INTO platforms ("
                  "uri, "
                  "parentStop, "
                  "longitude, "
                  "latitude, "
                  "name, "
                  "platform)"
                  " VALUES("
                  ":uri, "
                  ":parentStop, "
                  ":longitude, "
                  ":latitude, "
                  ":name, "
                  ":platform)");
    query.bindValue(":uri", platform.at(0));
    query.bindValue(":parentStop", platform.at(1));
    query.bindValue(":longitude", platform.at(2));
    query.bindValue(":latitude", platform.at(3));
    query.bindValue(":name", platform.at(4));
    query.bindValue(":platform", platform.at(5));
    return this->db()->executeAsync(query);
}

// Helpers
/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Tries to fetch a station from cache
 * @return StationEngine::Station *station
 * @package StationEngine
 * @private
 * @note If no station is found in the memory cache, a NULL pointer is returned.
 * Tries to fetch a station from the memory cache and returns it.
 */
StationEngine::Station *StationEngine::Factory::fetchStationFromCache(const QUrl &uri) const {
    if (m_cache.contains(uri)) {
        return this->m_cache.value(uri);
    }
    return nullptr;
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Adds a station to the cache
 * @param StationEngine::Station *station
 * @package StationEngine
 * @private
 * Adds a station to the memory cache to avoid duplicate StationEngine::Station
 * objects. This also reduces the look up time for station (less heavy database
 * operations).
 *
 * BENCHMARK: Jolla 1 saves 400-500 ms by using this caching method for a single
 * routing calculation.
 */
void StationEngine::Factory::addStationToCache(StationEngine::Station *station) {
    this->m_cache.insert(station->uri(), station);
}

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 13 Aug 2018
 * @brief Fetches the platforms of a station
 * @param const QUrl &uri
 * @return const QMap<QUrl, QString> platformsMap
 * @package StationEngine
 * @private
 * Fetches the platforms of a station by URI from the database.
 * The platforms are added in a QMap and returned.
 */
QMap<QUrl, QString> StationEngine::Factory::getPlatformsByStationURI(const QUrl &uri) {
    // Fetch platforms from database for a specific station URI.
    QSqlQuery query(this->db()->database());
    query.prepare("SELECT "
                  "uri, "
                  "parentStop, "
                  "platform "
                  "FROM platforms "
                  "WHERE parentStop = :parentStop");
    query.bindValue(":parentStop", uri);
    this->db()->execute(query);

    // Read result and add the platforms to a QMap with their URI as ID.
    QMap<QUrl, QString> platformsMap = QMap<QUrl, QString>();

    while (query.next()) {
        platformsMap.insert(
                    query.value(0).toUrl(),   // Platform URI
                    query.value(2).toString() // Platform name
                    );
    }

    return platformsMap;
}

// Getters & Setters
/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Gets the QRail::Database::Manager instance
 * @return QRail::Database::Manager *manager
 * @package StationEngine
 * @public
 * Gets the QRail::Database::Manager instance and returns it.
 */
QRail::Database::Manager *StationEngine::Factory::db() const { return m_db; }

/**
 * @file stationfactory.cpp
 * @author Dylan Van Assche
 * @date 09 Aug 2018
 * @brief Sets the QRail::Database::Manager instance
 * @param QRail::Database::Manager *manager
 * @package StationEngine
 * @public
 * Sets the QRail::Database::Manager instance to the given
 * QRail::Database::Manager *db.
 */
void StationEngine::Factory::setDb(QRail::Database::Manager *db) { m_db = db; }
