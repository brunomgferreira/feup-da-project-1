#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H


#include "Graph.h"

void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual);
bool findAugmentingPath(Graph *g, Vertex *s, Vertex *t);
double findMinResidualAlongPath(Vertex *s, Vertex *t);
void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);
void edmondsKarp(Graph *g, string source, string target);

bool optimizedFindAugmentingPath(Graph *g, Vertex *s, Vertex *t, double smallestCapacity, double i);
double optimizedFindMinResidualAlongPath(Vertex *s, Vertex *t, double smallestCapacity);
void optimizedAugmentFlowAlongPath(Vertex *s, Vertex *t, double f);
void optimizedEdmondsKarp(Graph *g, const string source, const string target, const double biggestCapacity, const double smallestCapacity, double *iterationCount);

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
