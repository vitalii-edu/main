#include "UserInterface.h"
#include "DatabaseManager.h"
#include <iostream>

int main() {
    DatabaseManager* dbManager = DatabaseManager::getInstance();
    if (!dbManager->openDatabase("clients.db")) {
        std::cerr << "Не вдалося відкрити базу даних." << std::endl;
        return 1;
    }

    UserInterface ui;
    ui.handleUserInput();

    dbManager->closeDatabase();
    return 0;
}
