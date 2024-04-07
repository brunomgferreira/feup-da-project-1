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
     * @brief Constructor for GraphMetrics class.
     *
     * @details This constructor initializes a graph metrics object with the specified values.
     * The parameters represent essential information about a graph metrics, including its id and code
     *
     * @param absoluteAverage The absolute average value.
     * @param absoluteVariance The absolute variance value.
     * @param absoluteStandardDeviation The absolute standard deviation value.
     * @param absoluteMaxDifference The maximum absolute difference value.
     * @param relativeAverage The relative average value.
     * @param relativeVariance The relative variance value.
     * @param relativeStandardDeviation The relative standard deviation value.
     * @param relativeMaxDifference The maximum relative difference value.
     * @param maxFlow The maximum flow value.
     * @param totalDemand The total demand value.
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

    /**
     * @brief Get the absolute average value.
     *
     * @return The absolute average value.
     */
    [[nodiscard]] double getAbsoluteAverage() const;

    /**
     * @brief Get the absolute variance value.
     *
     * @return The absolute variance value.
     */
    [[nodiscard]] double getAbsoluteVariance() const;

    /**
     * @brief Get the absolute standard deviation value.
     *
     * @return The absolute standard deviation value.
     */
    [[nodiscard]] double getAbsoluteStandardDeviation() const;

    /**
     * @brief Get the maximum absolute difference value.
     *
     * @return The maximum absolute difference value.
     */
    [[nodiscard]] double getAbsoluteMaxDifference() const;

    /**
     * @brief Get the relative average value.
     *
     * @return The relative average value.
     */
    [[nodiscard]] double getRelativeAverage() const;

    /**
     * @brief Get the relative variance value.
     *
     * @return The relative variance value.
     */
    [[nodiscard]] double getRelativeVariance() const;

    /**
     * @brief Get the relative standard deviation value.
     *
     * @return The relative standard deviation value.
     */
    [[nodiscard]] double getRelativeStandardDeviation() const;

    /**
     * @brief Get the maximum relative difference value.
     *
     * @return The maximum relative difference value.
     */
    [[nodiscard]] double getRelativeMaxDifference() const;

    /**
     * @brief Get the maximum flow value.
     *
     * @return The maximum flow value.
     */
    [[nodiscard]] double getMaxFlow() const;

    /**
     * @brief Get the total demand value.
     *
     * @return The total demand value.
     */
    [[nodiscard]] double getTotalDemand() const;
};


#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_METRICS_H
