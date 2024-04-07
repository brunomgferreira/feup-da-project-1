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
     * @brief Constructor for DeliverySite class.
     *
     * @param city The city associated with the delivery site.
     * @param id The ID of the delivery site.
     * @param code The code associated with the delivery site.
     * @param demand The demand of the delivery site.
     * @param population The population associated with the delivery site.
     */
    DeliverySite(string city, double id, string code, double demand, double population);

    /**
     * @brief Get the city associated with the delivery site.
     *
     * @return The city of the delivery site.
     */
    string getCity();

    /**
     * @brief Get the demand of the delivery site.
     *
     * @return The demand of the delivery site.
     */
    [[nodiscard]] double getDemand() const;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_DELIVERY_SITE_H
