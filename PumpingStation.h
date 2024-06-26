#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_PUMPING_STATION_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_PUMPING_STATION_H


#include <string>
#include <utility>
using namespace std;

/**
* @brief Class representing a pumping station entity in the water supply analysis system.
*/

class PumpingStation {
private :
    double id;
    string code;

public:

    /**
    * @brief Constructs a pumping station object with the provided details.
    *
    * @details This constructor initializes a pumping station object with the specified pumping station id and code.
    * The parameters represent essential information about a pumping station, including its id and code
    *
    * @param id The id of the pumping station.
    * @param code The unique code assigned to the pumping station.
    */
    PumpingStation(double id, string code);
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_PUMPING_STATION_H
