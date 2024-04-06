#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_METRICS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_METRICS_H


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
    * The parameters represent essential information about a graph metrics, including its id and code
    *
    * @param id The id of the pumping station.
    * @param code The unique code assigned to the pumping station.
    */
    explicit GraphMetrics(
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

    [[nodiscard]] double getAbsoluteAverage() const;
    [[nodiscard]] double getAbsoluteVariance() const;
    [[nodiscard]] double getAbsoluteStandardDeviation() const;
    [[nodiscard]] double getAbsoluteMaxDifference() const;
    [[nodiscard]] double getRelativeAverage() const;
    [[nodiscard]] double getRelativeVariance() const;
    [[nodiscard]] double getRelativeStandardDeviation() const;
    [[nodiscard]] double getRelativeMaxDifference() const;
    [[nodiscard]] double getMaxFlow() const;
    [[nodiscard]] double getTotalDemand() const;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_METRICS_H
