#include "App.h"
#include "States/MainMenuState.h"
#include "States/Utils/DataLoadError.h"

App* App::instance = nullptr;

App::App() {
    currentState = new MainMenuState();
    data = nullptr;
}

App::~App() {
    delete instance;
}

App *App::getInstance() {
    if (!instance) {
        instance = new App();
    }
    return instance;
}

State *App::getState() {
    return currentState;
}

void App::setState(State* state) {
    currentState = state;
}

void App::setData(const filesystem::path &dir_path) {
    try {
        Data *newData = new Data();
        newData->readFiles(dir_path);
        this->data = newData;
    } catch (const exception& e) {
        throw DataLoadError(e.what());
    }
}

void App::display() const {
    if(typeid(*currentState) == typeid(MainMenuState)) {
        cout << "\033[32m";
        cout << "------------------------------" << endl;
        cout << "\033[0m";
        cout << "> Loaded Network: " << (data != nullptr ? data->getNetworkName() : ("\033[31mNone\033[30m")) << endl;
    }
    currentState->display();
}

void App::handleInput() {
    currentState->handleInput(this);
}

Data *App::getData() {
    return data;
}
