#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_PIPE_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_PIPE_H


#include <string>
#include <utility>
using namespace std;

/**
* @brief Class representing a pipe entity in the water supply analysis system.
*/

class Pipe {
private :
    string servicePointA;
    string servicePointB;
    double capacity;
    bool unidirectional;

public:

    /**
    * @brief Constructs a pipe object with the provided details.
    *
    * @details This constructor initializes a pipe object with the specified pipe id and code.
    * The parameters represent essential information about a pipe, including its id and code
    *
    * @param servicePointA The source service that can be a water reservoir, a pumping station, or delivery site.
    * @param servicePointB The target service that can be a water reservoir, pumping station, or a delivery site.
    * @param capacity The maximum capacity of the pipe.
    * @param unidirectional Boolean indicating whether the connection between source service and target service is undirected or bi-directed
    */
    Pipe(string servicePointA, string servicePointB, double capacity, bool unidirectional);

    /**
     * @brief Get the name of service point A.
     *
     * @return The name of service point A.
     */
    string getServicePointA();

    /**
     * @brief Get the name of service point B.
     *
     * @return The name of service point B.
     */
    string getServicePointB();

    /**
     * @brief Check if the pipe is unidirectional.
     *
     * @return True if the pipe is unidirectional, otherwise false.
     */
    [[nodiscard]] bool getUnidirectional() const;

    /**
     * @brief Get the capacity of the pipe.
     *
     * @return The capacity of the pipe.
     */
    [[nodiscard]] double getCapacity() const;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_PIPE_H
