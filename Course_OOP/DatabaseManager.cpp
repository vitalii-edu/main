#include "DatabaseManager.h"
#include <iostream>

DatabaseManager* DatabaseManager::instance = nullptr;

// Приватний конструктор
DatabaseManager::DatabaseManager() : db(nullptr) {}

// Деструктор
DatabaseManager::~DatabaseManager() {
    closeDatabase();
}

// Метод для отримання єдиного екземпляра
DatabaseManager* DatabaseManager::getInstance() {
    if (instance == nullptr) {
        instance = new DatabaseManager();
    }
    return instance;
}

// Відкрити базу даних
bool DatabaseManager::openDatabase(const std::string& dbName) {
    int rc = sqlite3_open(dbName.c_str(), &db);
    if (rc) {
        std::cerr << "Не вдалося відкрити базу даних: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    initializeDatabase();
    return true;
}

// Закрити базу даних
void DatabaseManager::closeDatabase() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

// Ініціалізація бази даних
void DatabaseManager::initializeDatabase() {
    const char* sqlClients = "CREATE TABLE IF NOT EXISTS Clients ("
                             "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                             "name TEXT NOT NULL,"
                             "contactInfo TEXT,"
                             "address TEXT);";

    const char* sqlInteractions = "CREATE TABLE IF NOT EXISTS Interactions ("
                                  "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                                  "clientId INTEGER,"
                                  "date TEXT,"
                                  "type TEXT,"
                                  "notes TEXT,"
                                  "FOREIGN KEY(clientId) REFERENCES Clients(id));";

    char* errMsg = nullptr;
    int rc = sqlite3_exec(db, sqlClients, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Помилка створення таблиці Clients: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }

    rc = sqlite3_exec(db, sqlInteractions, nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Помилка створення таблиці Interactions: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// Зберегти клієнта
bool DatabaseManager::saveClient(Client& client) {
    std::string sql = "INSERT INTO Clients (name, contactInfo, address) VALUES (?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, client.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, client.getContactInfo().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, client.getAddress().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Помилка виконання запиту: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    client.setId(static_cast<int>(sqlite3_last_insert_rowid(db)));
    sqlite3_finalize(stmt);
    return true;
}

// Завантажити клієнта
Client DatabaseManager::loadClient(int id) {
    Client client;
    std::string sql = "SELECT * FROM Clients WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return client;
    }
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        client.setId(sqlite3_column_int(stmt, 0));
        client.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        client.setContactInfo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        client.setAddress(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        // Завантаження взаємодій
        client.getInteractions() = loadInteractions(client.getId());
    } else {
        std::cerr << "Клієнта не знайдено." << std::endl;
    }
    sqlite3_finalize(stmt);
    return client;
}

// Оновити клієнта
bool DatabaseManager::updateClient(const Client& client) {
    std::string sql = "UPDATE Clients SET name = ?, contactInfo = ?, address = ? WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_text(stmt, 1, client.getName().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, client.getContactInfo().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, client.getAddress().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, client.getId());

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Помилка виконання запиту: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

// Видалити клієнта
bool DatabaseManager::deleteClient(int id) {
    std::string sql = "DELETE FROM Clients WHERE id = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Помилка виконання запиту: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);

    // Видалення взаємодій з клієнтом
    sql = "DELETE FROM Interactions WHERE clientId = ?;";
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту для взаємодій: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, id);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    return true;
}

// Отримати всіх клієнтів
std::vector<Client> DatabaseManager::getAllClients() {
    std::vector<Client> clients;
    std::string sql = "SELECT * FROM Clients;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return clients;
    }

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Client client;
        client.setId(sqlite3_column_int(stmt, 0));
        client.setName(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)));
        client.setContactInfo(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        client.setAddress(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        clients.push_back(client);
    }
    sqlite3_finalize(stmt);
    return clients;
}

// Зберегти взаємодію
bool DatabaseManager::saveInteraction(const Interaction& interaction) {
    std::string sql = "INSERT INTO Interactions (clientId, date, type, notes) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return false;
    }
    sqlite3_bind_int(stmt, 1, interaction.getClientId());
    sqlite3_bind_text(stmt, 2, interaction.getDate().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, interaction.getType().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, interaction.getNotes().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Помилка виконання запиту: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
}

// Завантажити взаємодії
std::vector<Interaction> DatabaseManager::loadInteractions(int clientId) {
    std::vector<Interaction> interactions;
    std::string sql = "SELECT * FROM Interactions WHERE clientId = ?;";
    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Помилка підготовки запиту: " << sqlite3_errmsg(db) << std::endl;
        return interactions;
    }
    sqlite3_bind_int(stmt, 1, clientId);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        Interaction interaction;
        interaction.setId(sqlite3_column_int(stmt, 0));
        interaction.setClientId(sqlite3_column_int(stmt, 1));
        interaction.setDate(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
        interaction.setType(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)));
        interaction.setNotes(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4)));
        interactions.push_back(interaction);
    }
    sqlite3_finalize(stmt);
    return interactions;
}
