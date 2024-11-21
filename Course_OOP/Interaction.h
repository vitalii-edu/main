#ifndef INTERACTION_H
#define INTERACTION_H

#include <string>
#include <ctime>

class Interaction {
private:
    int id;
    int clientId;
    std::string date;
    std::string type;
    std::string notes;

public:
    // Конструктори
    Interaction();
    Interaction(int id, int clientId, const std::string& date, const std::string& type, const std::string& notes);

    // Геттери та сеттери
    int getId() const;
    void setId(int id);

    int getClientId() const;
    void setClientId(int clientId);

    std::string getDate() const;
    void setDate(const std::string& date);

    std::string getType() const;
    void setType(const std::string& type);

    std::string getNotes() const;
    void setNotes(const std::string& notes);

    // Інші методи
    void displayDetails() const;
};

#endif // INTERACTION_H
