#include "States/MainMenuState.h"
#include "PumpingStationImpactMenuState.h"
#include "States/Utils/GetPumpingStationState.h"

PumpingStationImpactMenuState::PumpingStationImpactMenuState() = default;

void PumpingStationImpactMenuState::display() const {
    cout << "\033[32m";
    cout << "==== PUMPING STATION IMPACT ====" << endl;
    cout << "\033[0m";
    cout << "   1. Not Essential             " << endl;
    cout << "   2. Specific Pumping Station  " << endl;
    cout << "   3. All Pumping Stations      \n" << endl;

    cout << "   q. Main Menu              " << endl;
    cout << "\033[32m";
    cout << "-----------------------------" << endl;
    cout << "\033[0m";
    cout << "Enter your choice: ";
}

void PumpingStationImpactMenuState::handleInput(App* app) {
    string choice;
    cin >> choice;

    if (choice.size() == 1) {
        switch (choice[0]) {
            case '1':
                app->getData()->notEssentialPumpingStations();
                PressEnterToContinue();
                app->setState(this);
                break;
            case '2':
                app->setState(new GetPumpingStationState(this, [&](App *app, const string& code) {
                    app->getData()->pumpingStationImpact(code);
                    PressEnterToContinue(1);
                    app->setState(this);
                }));
                break;
            case '3':
                app->getData()->allPumpingStationsImpact();
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