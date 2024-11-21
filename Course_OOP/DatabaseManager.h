#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include "Client.h"
#include "Interaction.h"
#include <sqlite3.h>

class DatabaseManager {
private:
    static DatabaseManager* instance;
    sqlite3* db;

    // Приватний конструктор
    DatabaseManager();

public:
    // Деструктор
    ~DatabaseManager();

    // Метод для отримання єдиного екземпляра
    static DatabaseManager* getInstance();

    // Методи роботи з базою даних
    bool openDatabase(const std::string& dbName);
    void closeDatabase();

    bool saveClient(Client& client);
    Client loadClient(int id);
    bool updateClient(const Client& client);
    bool deleteClient(int id);
    std::vector<Client> getAllClients();

    bool saveInteraction(const Interaction& interaction);
    std::vector<Interaction> loadInteractions(int clientId);

    // Інші методи
    void initializeDatabase();
};

#endif // DATABASEMANAGER_H
