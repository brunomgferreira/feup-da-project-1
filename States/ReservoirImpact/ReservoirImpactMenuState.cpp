#include <iostream>
#include "States/MainMenuState.h"
#include "ReservoirImpactMenuState.h"
#include "States/Utils/GetReservoirState.h"

ReservoirImpactMenuState::ReservoirImpactMenuState() {}

void ReservoirImpactMenuState::display() const {
    cout << "\033[32m";
    cout << "======= RESERVOIR IMPACT =======" << endl;
    cout << "\033[0m";
    cout << "   1. Specific Reservoir    " << endl;
    cout << "   2. All Reservoirs      \n" << endl;

    cout << "   q. Main Menu              " << endl;
    cout << "\033[32m";
    cout << "-----------------------------" << endl;
    cout << "\033[0m";
    cout << "Enter your choice: ";
}

void ReservoirImpactMenuState::handleInput(App* app) {
    string choice;
    cin >> choice;

    if (choice.size() == 1) {
        switch (choice[0]) {
            case '1':
                app->setState(new GetReservoirState(this, [&](App *app, const string& code) {
                    app->getData()->reservoirImpact(code);
                    PressEnterToContinue(1);
                    app->setState(this);
                }));
                break;
            case '2':
                app->getData()->allReservoirsImpact();
                PressEnterToContinue();
                app->setState(this);
                break;
            case 'q':
                app->setState(new MainMenuState());
                break;
            default:
                cout << "\033[31m" << "Invalid choice. Please try again." << "\033[0m"  << endl;
        }
    } else  {
        cout << "\033[31m";
        cout << "Invalid input. Please enter a single character." << endl;
        cout << "\033[0m";
    }
}
