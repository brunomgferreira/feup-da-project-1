#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPHMETRICS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPHMETRICS_H


#include <string>
using namespace std;

/**
* @brief Class to save the metrics of a given graph.
*/

class GraphMetrics {
private :
    // Absolute metrics
    double absoluteAverage;
    double absoluteVariance;
    double absoluteStandardDeviation;
    double absoluteMaxDifference;

    // Relative metrics
    double relativeAverage;
    double relativeVariance;
    double relativeStandardDeviation;
    double relativeMaxDifference;

    // Total Flow
    double maxFlow;
    double totalDemand;

public:

    /**
    * @brief Constructs a graph metrics object with the provided details.
    *
    * @details This constructor initializes a graph metrics object with the specified values.
    * The parameters represent essential information about a graph metrics, including its id and code AAAAAAAAAAA
    *
    * @param id The id of the pumping station.
    * @param code The unique code assigned to the pumping station.
    */
    GraphMetrics(
        double absoluteAverage = 0,
        double absoluteVariance = 0,
        double absoluteStandardDeviation = 0,
        double absoluteMaxDifference = 0,
        double relativeAverage = 0,
        double relativeVariance = 0,
        double relativeStandardDeviation = 0,
        double relativeMaxDifference = 0,
        double maxFlow = 0,
        double totalDemand = 0);

    double getAbsoluteAverage() const;
    double getAbsoluteVariance() const;
    double getAbsoluteStandardDeviation() const;
    double getAbsoluteMaxDifference() const;
    double getRelativeAverage() const;
    double getRelativeVariance() const;
    double getRelativeStandardDeviation() const;
    double getRelativeMaxDifference() const;
    double getMaxFlow() const;
    double getTotalDemand() const;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPHMETRICS_H
