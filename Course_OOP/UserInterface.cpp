#include "UserInterface.h"
#include <iostream>

UserInterface::UserInterface() {
    // Конструктор
}

void UserInterface::displayMenu() {
    std::cout << "\n=== Система управління контактами з клієнтами ===" << std::endl;
    std::cout << "1. Додати нового клієнта" << std::endl;
    std::cout << "2. Переглянути клієнта" << std::endl;
    std::cout << "3. Оновити клієнта" << std::endl;
    std::cout << "4. Видалити клієнта" << std::endl;
    std::cout << "5. Список всіх клієнтів" << std::endl;
    std::cout << "6. Додати взаємодію з клієнтом" << std::endl;
    std::cout << "0. Вихід" << std::endl;
    std::cout << "Виберіть опцію: ";
}

void UserInterface::handleUserInput() {
    int choice;
    while (true) {
        displayMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1:
                controller.addNewClient();
                break;
            case 2:
                controller.viewClient();
                break;
            case 3:
                controller.updateClient();
                break;
            case 4:
                controller.deleteClient();
                break;
            case 5:
                controller.listAllClients();
                break;
            case 6:
                controller.addInteraction();
                break;
            case 0:
                std::cout << "Вихід з програми..." << std::endl;
                return;
            default:
                std::cout << "Невірний вибір. Спробуйте ще раз." << std::endl;
                break;
        }
    }
}
