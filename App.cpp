#include "App.h"
#include "States/MainMenuState.h"

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

void App::display() const {
    currentState->display();
}

void App::handleInput() {
    currentState->handleInput(this);
}

Data *App::getData() {
    return data;
}
