#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include "Interaction.h"

class Client {
private:
    int id;
    std::string name;
    std::string contactInfo;
    std::string address;
    std::vector<Interaction> interactions;

public:
    // Конструктори
    Client();
    Client(int id, const std::string& name, const std::string& contactInfo, const std::string& address);

    // Геттери та сеттери
    int getId() const;
    void setId(int id);

    std::string getName() const;
    void setName(const std::string& name);

    std::string getContactInfo() const;
    void setContactInfo(const std::string& contactInfo);

    std::string getAddress() const;
    void setAddress(const std::string& address);

    // Методи роботи з взаємодіями
    void addInteraction(const Interaction& interaction);
    std::vector<Interaction> getInteractions() const;

    // Інші методи
    void displayInfo() const;
};

#endif // CLIENT_H
