#ifndef SYSTEMCONTROLLER_H
#define SYSTEMCONTROLLER_H

#include "DatabaseManager.h"

class SystemController {
private:
    DatabaseManager* dbManager;

public:
    SystemController();

    // Методи для обробки запитів
    void addNewClient();
    void viewClient();
    void updateClient();
    void deleteClient();
    void listAllClients();
    void addInteraction();

    // Інші методи
};

#endif // SYSTEMCONTROLLER_H
