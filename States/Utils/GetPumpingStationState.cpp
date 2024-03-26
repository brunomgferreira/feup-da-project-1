#include <iostream>
#include "GetPumpingStationState.h"
#include "TryAgainState.h"

GetPumpingStationState::GetPumpingStationState(State* backState, function<void(App*, const string&)> nextStateCallback)
        : backState(backState), nextStateCallback(nextStateCallback) {}

void GetPumpingStationState::display() const {
    cout << "Insert pumping station code (Ex: PS_1): ";
}

void GetPumpingStationState::handleInput(App* app) {
    string pumpingStationCode;
    std::cin.ignore();
    std::getline(std::cin, pumpingStationCode);

    bool pumpingStationExists = app->getData()->pumpingStationExists(pumpingStationCode);

    if (pumpingStationExists) {
        nextStateCallback(app, pumpingStationCode);
    } else {
        cout << "\033[31m";
        cout << "Pumping Station does not exist." << endl;
        cout << "\033[0m";
        app->setState(new TryAgainState(backState, this));
    }
}