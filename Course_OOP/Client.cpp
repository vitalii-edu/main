#include "Client.h"
#include <iostream>

// Конструктори
Client::Client() : id(0), name(""), contactInfo(""), address("") {}

Client::Client(int id, const std::string& name, const std::string& contactInfo, const std::string& address)
    : id(id), name(name), contactInfo(contactInfo), address(address) {}

// Геттери та сеттери
int Client::getId() const {
    return id;
}

void Client::setId(int id) {
    this->id = id;
}

std::string Client::getName() const {
    return name;
}

void Client::setName(const std::string& name) {
    this->name = name;
}

std::string Client::getContactInfo() const {
    return contactInfo;
}

void Client::setContactInfo(const std::string& contactInfo) {
    this->contactInfo = contactInfo;
}

std::string Client::getAddress() const {
    return address;
}

void Client::setAddress(const std::string& address) {
    this->address = address;
}

// Методи роботи з взаємодіями
void Client::addInteraction(const Interaction& interaction) {
    interactions.push_back(interaction);
}

std::vector<Interaction> Client::getInteractions() const {
    return interactions;
}

// Інші методи
void Client::displayInfo() const {
    std::cout << "ID: " << id << std::endl;
    std::cout << "Ім'я: " << name << std::endl;
    std::cout << "Контактна інформація: " << contactInfo << std::endl;
    std::cout << "Адреса: " << address << std::endl;
}
