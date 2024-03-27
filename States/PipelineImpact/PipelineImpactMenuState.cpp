#include <iostream>
#include "States/MainMenuState.h"
#include "PipelineImpactMenuState.h"

PipelineImpactMenuState::PipelineImpactMenuState() {}

void PipelineImpactMenuState::display() const {
    cout << "\033[32m";
    cout << "======= PIPELINE IMPACT =======" << endl;
    cout << "\033[0m";
    cout << "   1. Essential             " << endl;
    cout << "   2. Specific Pipeline     " << endl;
    cout << "   3. All Pipelines       \n" << endl;

    cout << "   q. Main Menu              " << endl;
    cout << "\033[32m";
    cout << "-----------------------------" << endl;
    cout << "\033[0m";
    cout << "Enter your choice: ";
}

void PipelineImpactMenuState::handleInput(App* app) {
    string choice;
    cin >> choice;

    if (choice.size() == 1) {
        switch (choice[0]) {
            case '1':
                app->getData()->essentialPipelines();
                PressEnterToContinue();
                app->setState(this);
                break;
            case '2':
                /*
                app->setState(new GetPipelineState(this, [&](App *app, const string& code) {
                    app->getData()->pipelineImpact(code);
                    PressEnterToContinue(1);
                    app->setState(this);
                }));
                */
                app->setState(this);
                break;
            case '3':
                // app->getData()->allPipelinesImpact();
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