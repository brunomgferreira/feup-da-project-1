#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_GETRESERVOIRSTATE_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_GETRESERVOIRSTATE_H


#include "States/State.h"

/**
* @brief Class that represents a state for obtaining the network reservoirs.
*/

class GetReservoirState : public State {
private:
    State* backState;
    function<void(App*, const string&)> nextStateCallback;
public:

    /**
    * @brief Constructs an instance of GetReservoirState with specified back state and callback function.
    *
    * @details This constructor initializes an instance of the GetReservoirState class with the given back state
    * and a callback function for transitioning to the next state. The back state represents the state to which
    * the application should return when the user chooses to go back from the current state. The callback function
    * specifies the action to be performed in the next state, usually involving capturing the reservoir code from user
    * input and proceeding to the next state with the captured information.
    *
    * @param backState A pointer to the state to which the application should return when the user chooses to go back.
    * @param nextStateCallback A function defining the action to be performed in the next state, usually involving a reservoir code.
    */
    GetReservoirState(State* backState, function<void(App*, const string&)> nextStateCallback);

    /**
    * @brief Displays a prompt for inserting a reservoir code.
    *
    * @details This method prints a prompt to the console, asking the user to insert a reservoir code. The displayed
    * prompt guides the user on the expected format for entering the reservoir code. The user is expected to input a valid
    * reservoir code based on the specified format.
    */
    void display() const override;

    /**
    * @brief Handles user input for obtaining a reservoir code.
    *
    * @details This method reads a line of input from the console, representing a reservoir code. The method validates
    * the existence of the entered reservoir code in the application's data. If the reservoir code is valid, the callback
    * function is invoked with the captured reservoir code, and the application transitions to the next state accordingly.
    * If the reservoir code is invalid, the user is prompted with an error message, and the state transitions to a
    * "Try Again" state, allowing the user to make another attempt.
    *
    * @param app A pointer to the application instance.
    */
    void handleInput(App* app) override;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GETRESERVOIRSTATE_H
