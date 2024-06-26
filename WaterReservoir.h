#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_WATER_RESERVOIR_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_WATER_RESERVOIR_H


#include <string>
#include <utility>
using namespace std;

/**
* @brief Class representing a water reservoir entity in the water supply analysis system.
*/

class WaterReservoir {
private :
    string name;
    string municipality;
    double id;
    string code;
    double maxDelivery;

public:

    /**
    * @brief Constructs a water reservoir object with the provided details.
    *
    * @details This constructor initializes a water reservoir object with the specified water reservoir name, municipality, id, code and maxDelivery.
    * The parameters represent essential information about an water reservoir, including its name, municipality, id, code and maxDelivery
    *
    * @param name The name of the water reservoir.
    * @param municipality The name of the municipality.
    * @param id The id of the water reservoir.
    * @param code The unique code assigned to the water reservoir.
    * @param maxDelivery The maximum delivery of water reservoir in m³/sec.
    */
    WaterReservoir(string name, string municipality, double id, string code, double maxDelivery = 0);

    /**
     * @brief Get the maximum delivery capacity of the water reservoir.
     *
     * @return The maximum delivery capacity.
     */
    double getMaxDelivery();

    /**
     * @brief Get the name of the water reservoir.
     *
     * @return The name of the water reservoir.
     */
    string getName();
};

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_WATER_RESERVOIR_H
