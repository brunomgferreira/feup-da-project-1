#include "GetPipelineState.h"
#include "TryAgainState.h"

GetPipelineState::GetPipelineState(State* backState, function<void(App*, const string&)> nextStateCallback)
        : backState(backState), nextStateCallback(std::move(nextStateCallback)) {}

void GetPipelineState::display() const {
    cout << "Insert pipeline code (Ex: PS_1-PS_2): ";
}

void GetPipelineState::handleInput(App* app) {
    string pipelineCode;
    std::cin.ignore();
    std::getline(std::cin, pipelineCode);

    bool pipelineExists = app->getData()->pipelineExists(pipelineCode);

    if (pipelineExists) {
        nextStateCallback(app, pipelineCode);
    } else {
        cout << "\033[31m";
        cout << "Pipeline does not exist." << endl;
        cout << "\033[0m";
        app->setState(new TryAgainState(backState, this));
    }
}