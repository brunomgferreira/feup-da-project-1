#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_DELIVERY_SITE_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_DELIVERY_SITE_H


#include <string>
#include <utility>
using namespace std;

/**
* @brief Class representing a delivery site entity in the water supply analysis system.
*/

class DeliverySite {
private :
    string city;
    double id;
    string code;
    double demand;
    double population;

public:

    /**
    * @brief Constructs a delivery site object with the provided details.
    *
    * @details This constructor initializes a delivery site object with the specified delivery site id and code.
    * The parameters represent essential information about a delivery site, including its id and code
    *
    * @param id The id of the pumping station.
    * @param code The unique code assigned to the pumping station.
    */
    DeliverySite(string city, double id, string code, double demand, double population);

    string getCity();

    [[nodiscard]] double getDemand() const;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_DELIVERY_SITE_H
