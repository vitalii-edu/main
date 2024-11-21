#include "Interaction.h"
#include <iostream>

// Конструктори
Interaction::Interaction() : id(0), clientId(0), date(""), type(""), notes("") {}

Interaction::Interaction(int id, int clientId, const std::string& date, const std::string& type, const std::string& notes)
    : id(id), clientId(clientId), date(date), type(type), notes(notes) {}

// Геттери та сеттери
int Interaction::getId() const {
    return id;
}

void Interaction::setId(int id) {
    this->id = id;
}

int Interaction::getClientId() const {
    return clientId;
}

void Interaction::setClientId(int clientId) {
    this->clientId = clientId;
}

std::string Interaction::getDate() const {
    return date;
}

void Interaction::setDate(const std::string& date) {
    this->date = date;
}

std::string Interaction::getType() const {
    return type;
}

void Interaction::setType(const std::string& type) {
    this->type = type;
}

std::string Interaction::getNotes() const {
    return notes;
}

void Interaction::setNotes(const std::string& notes) {
    this->notes = notes;
}

// Інші методи
void Interaction::displayDetails() const {
    std::cout << "Дата: " << date << std::endl;
    std::cout << "Тип: " << type << std::endl;
    std::cout << "Примітки: " << notes << std::endl;
}
