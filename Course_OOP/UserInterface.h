#ifndef USERINTERFACE_H
#define USERINTERFACE_H

#include "SystemController.h"

class UserInterface {
private:
    SystemController controller;

public:
    UserInterface();
    void displayMenu();
    void handleUserInput();
};

#endif // USERINTERFACE_H
