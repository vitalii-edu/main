#include "SystemController.h"
#include <iostream>
#include <iomanip>

SystemController::SystemController() {
    dbManager = DatabaseManager::getInstance();
}

// Додати нового клієнта
void SystemController::addNewClient() {
    std::string name, contactInfo, address;
    std::cout << "Введіть ім'я клієнта: ";
    std::getline(std::cin, name);
    std::cout << "Введіть контактну інформацію: ";
    std::getline(std::cin, contactInfo);
    std::cout << "Введіть адресу: ";
    std::getline(std::cin, address);

    Client client(0, name, contactInfo, address);
    if (dbManager->saveClient(client)) {
        std::cout << "Клієнта успішно додано з ID: " << client.getId() << std::endl;
    } else {
        std::cout << "Помилка додавання клієнта." << std::endl;
    }
}

// Переглянути клієнта
void SystemController::viewClient() {
    int id;
    std::cout << "Введіть ID клієнта: ";
    std::cin >> id;
    std::cin.ignore();

    Client client = dbManager->loadClient(id);
    if (client.getId() != 0) {
        client.displayInfo();
        std::vector<Interaction> interactions = client.getInteractions();
        if (!interactions.empty()) {
            std::cout << "Історія взаємодій:" << std::endl;
            for (const auto& interaction : interactions) {
                interaction.displayDetails();
                std::cout << "-----------------------" << std::endl;
            }
        } else {
            std::cout << "Взаємодій не знайдено." << std::endl;
        }
    } else {
        std::cout << "Клієнта не знайдено." << std::endl;
    }
}

// Оновити клієнта
void SystemController::updateClient() {
    int id;
    std::cout << "Введіть ID клієнта: ";
    std::cin >> id;
    std::cin.ignore();

    Client client = dbManager->loadClient(id);
    if (client.getId() != 0) {
        std::string name, contactInfo, address;
        std::cout << "Введіть нове ім'я клієнта (поточне: " << client.getName() << "): ";
        std::getline(std::cin, name);
        std::cout << "Введіть нову контактну інформацію (поточна: " << client.getContactInfo() << "): ";
        std::getline(std::cin, contactInfo);
        std::cout << "Введіть нову адресу (поточна: " << client.getAddress() << "): ";
        std::getline(std::cin, address);

        if (!name.empty()) client.setName(name);
        if (!contactInfo.empty()) client.setContactInfo(contactInfo);
        if (!address.empty()) client.setAddress(address);

        if (dbManager->updateClient(client)) {
            std::cout << "Клієнта успішно оновлено." << std::endl;
        } else {
            std::cout << "Помилка оновлення клієнта." << std::endl;
        }
    } else {
        std::cout << "Клієнта не знайдено." << std::endl;
    }
}

// Видалити клієнта
void SystemController::deleteClient() {
    int id;
    std::cout << "Введіть ID клієнта: ";
    std::cin >> id;
    std::cin.ignore();

    if (dbManager->deleteClient(id)) {
        std::cout << "Клієнта успішно видалено." << std::endl;
    } else {
        std::cout << "Помилка видалення клієнта." << std::endl;
    }
}

// Список всіх клієнтів
void SystemController::listAllClients() {
    std::vector<Client> clients = dbManager->getAllClients();
    if (!clients.empty()) {
        std::cout << std::left << std::setw(5) << "ID" << std::setw(20) << "Ім'я" << std::setw(20) << "Контактна інформація" << std::setw(20) << "Адреса" << std::endl;
        std::cout << "--------------------------------------------------------------------------------" << std::endl;
        for (const auto& client : clients) {
            std::cout << std::left << std::setw(5) << client.getId()
                      << std::setw(20) << client.getName()
                      << std::setw(20) << client.getContactInfo()
                      << std::setw(20) << client.getAddress() << std::endl;
        }
    } else {
        std::cout << "Клієнтів не знайдено." << std::endl;
    }
}

// Додати взаємодію
void SystemController::addInteraction() {
    int clientId;
    std::cout << "Введіть ID клієнта: ";
    std::cin >> clientId;
    std::cin.ignore();

    Client client = dbManager->loadClient(clientId);
    if (client.getId() != 0) {
        std::string date, type, notes;
        std::cout << "Введіть дату (YYYY-MM-DD): ";
        std::getline(std::cin, date);
        std::cout << "Введіть тип взаємодії: ";
        std::getline(std::cin, type);
        std::cout << "Введіть примітки: ";
        std::getline(std::cin, notes);

        Interaction interaction(0, clientId, date, type, notes);
        if (dbManager->saveInteraction(interaction)) {
            std::cout << "Взаємодію успішно додано." << std::endl;
        } else {
            std::cout << "Помилка додавання взаємодії." << std::endl;
        }
    } else {
        std::cout << "Клієнта не знайдено." << std::endl;
    }
}
