#include "MainMenuState.h"
#include "States/MaxFlow/MaxFlowMenuState.h"
#include "States/PumpingStationImpact/PumpingStationImpactMenuState.h"
#include "States/PipelineImpact/PipelineImpactMenuState.h"
#include "States/ReservoirImpact/ReservoirImpactMenuState.h"
#include "States/Utils/GetFilesPathState.h"

MainMenuState::MainMenuState() = default;

void MainMenuState::display() const {
    cout << "\033[32m";
    cout << "========== MAIN MENU ==========" << endl;
    cout << "\033[0m";
    cout << "   1. Load Network            " << endl;
    cout << "   2. Find Max Water Flow     " << endl;
    cout << "   3. Verify Water Supply     " << endl;
    cout << "   4. Load Optimization       " << endl;
    cout << "   5. Reservoir Impact        " << endl;
    cout << "   6. Pumping Station Impact  " << endl;
    cout << "   7. Pipeline Failure Impact \n" << endl;

    cout << "   q. Exit           " << endl;
    cout << "\033[32m";
    cout << "------------------------------" << endl;
    cout << "\033[0m";
    cout << "Enter your choice: ";
}

void MainMenuState::handleInput(App* app) {
    string choice;
    cin >> choice;

    if (choice.size() == 1) {
        if(app->getData() == nullptr) {
            switch (choice[0]) {
                case '1':
                    app->setState(new GetFilesPathState(this, [&](App *app) {
                        cout << "Network loaded successfully! " << endl;
                        PressEnterToContinue(1);
                        app->setState(this);
                    }));
                    break;
                case 'q':
                    cout << "\033[32m";
                    cout << "========================================" << endl;
                    cout << "\033[0m";
                    cout << "Exiting the program..." << endl << endl;
                    cout << "System Developed by:" << endl;
                    cout << "  > António Santos" << " - " << "up201705558@fe.up.pt" << endl;
                    cout << "  > Bruno Ferreira" << " - " << "up202207863@fe.up.pt" << endl;
                    cout << "  > Gonçalo Ferros" << " - " << "up202207592@fe.up.pt" << endl;
                    cout << "\033[32m";
                    cout << "========================================" << endl;
                    cout << "\033[0m";
                    app->setState(nullptr);
                    break;
                default:
                    cout << endl;
                    cout << "\033[31m" << "> No water network loaded. Please load a network first." << "\033[0m" << endl << endl;
                    PressEnterToContinue();
            }
        }
        else {
                switch (choice[0]) {
                    case '1':
                        app->setState(new GetFilesPathState(this, [&](App *app) {
                            cout << "Network loaded successfully! " << endl;
                            PressEnterToContinue(1);
                            app->setState(this);
                        }));
                        break;
                    case '2':
                        app->setState(new MaxFlowMenuState());
                        break;
                    case '3':
                        app->getData()->verifyWaterSupply();
                        PressEnterToContinue();
                        break;
                    case '4':
                        app->getData()->loadOptimization();
                        PressEnterToContinue();
                        break;
                    case '5':
                        app->setState(new ReservoirImpactMenuState());
                        break;
                    case '6':
                        app->setState(new PumpingStationImpactMenuState());
                        break;
                    case '7':
                        app->setState(new PipelineImpactMenuState());
                        break;
                    case 'q':
                        cout << "\033[32m";
                        cout << "========================================" << endl;
                        cout << "\033[0m";
                        cout << "Exiting the program..." << endl << endl;
                        cout << "System Developed by:" << endl;
                        cout << "  > António Santos" << " - " << "up201705558@fe.up.pt" << endl;
                        cout << "  > Bruno Ferreira" << " - " << "up202207863@fe.up.pt" << endl;
                        cout << "  > Gonçalo Ferros" << " - " << "up202207592@fe.up.pt" << endl;
                        cout << "\033[32m";
                        cout << "========================================" << endl;
                        cout << "\033[0m";
                        app->setState(nullptr);
                        break;
                    default:
                        cout << "\033[31m" << "Invalid choice. Please try again." << "\033[0m" << endl;
                }
            }
    } else {
        cout << "\033[31m";
        cout << "Invalid input. Please enter a single character." << endl;
        cout << "\033[0m";
    }
}