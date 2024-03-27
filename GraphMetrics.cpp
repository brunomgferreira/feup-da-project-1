#include "GraphMetrics.h"

GraphMetrics::GraphMetrics(
        double absoluteAverage,
        double absoluteVariance,
        double absoluteStandardDeviation,
        double absoluteMaxDifference,
        double relativeAverage,
        double relativeVariance,
        double relativeStandardDeviation,
        double relativeMaxDifference,
        double maxFlow,
        double totalDemand)
        : absoluteAverage(absoluteAverage),
        absoluteVariance(absoluteVariance),
        absoluteStandardDeviation(absoluteStandardDeviation),
        absoluteMaxDifference(absoluteMaxDifference),
        relativeAverage(relativeAverage),
        relativeVariance(relativeVariance),
        relativeStandardDeviation(relativeStandardDeviation),
        relativeMaxDifference(relativeMaxDifference),
        maxFlow(maxFlow),
        totalDemand(totalDemand) {}

double GraphMetrics::getAbsoluteAverage() const {
    return this->absoluteAverage;
}

double GraphMetrics::getAbsoluteVariance() const {
    return this->absoluteVariance;
}

double GraphMetrics::getAbsoluteStandardDeviation() const {
    return this->absoluteStandardDeviation;
}

double GraphMetrics::getAbsoluteMaxDifference() const {
    return this->absoluteMaxDifference;
}

double GraphMetrics::getRelativeAverage() const {
    return this->relativeAverage;
}

double GraphMetrics::getRelativeVariance() const {
    return this->relativeVariance;
}

double GraphMetrics::getRelativeStandardDeviation() const {
    return this->relativeStandardDeviation;
}

double GraphMetrics::getRelativeMaxDifference() const {
    return this->relativeMaxDifference;
}

double GraphMetrics::getMaxFlow() const {
    return this->maxFlow;
}

double GraphMetrics::getTotalDemand() const {
    return this->totalDemand;
}
