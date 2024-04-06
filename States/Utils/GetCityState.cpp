#include "GetCityState.h"
#include "TryAgainState.h"

GetCityState::GetCityState(State* backState, function<void(App*, const string&)> nextStateCallback)
        : backState(backState), nextStateCallback(std::move(nextStateCallback)) {}

void GetCityState::display() const {
    cout << "Insert city code (Ex: C_1): ";
}

void GetCityState::handleInput(App* app) {
    string cityCode;
    std::cin.ignore();
    std::getline(std::cin, cityCode);

    bool cityExists = app->getData()->deliverySiteExists(cityCode);

    if (cityExists) {
        nextStateCallback(app, cityCode);
    } else {
        cout << "\033[31m";
        cout << "City does not exist." << endl;
        cout << "\033[0m";
        app->setState(new TryAgainState(backState, this));
    }
}