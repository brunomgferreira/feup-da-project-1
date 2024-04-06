#include "GetReservoirState.h"
#include "TryAgainState.h"

GetReservoirState::GetReservoirState(State* backState, function<void(App*, const string&)> nextStateCallback)
: backState(backState), nextStateCallback(std::move(nextStateCallback)) {}

void GetReservoirState::display() const {
    cout << "Insert reservoir code (Ex: R_1): ";
}

void GetReservoirState::handleInput(App* app) {
    string reservoirCode;
    std::cin.ignore();
    std::getline(std::cin, reservoirCode);

    bool reservoirExists = app->getData()->waterReservoirExists(reservoirCode);

    if (reservoirExists) {
        nextStateCallback(app, reservoirCode);
    } else {
        cout << "\033[31m";
        cout << "Reservoir does not exist." << endl;
        cout << "\033[0m";
        app->setState(new TryAgainState(backState, this));
    }
}