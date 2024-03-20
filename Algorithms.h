#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H


#include "Graph.h"

void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, unsigned long residual);
bool findAugmentingPath(Graph *g, Vertex *s, Vertex *t);
unsigned long findMinResidualAlongPath(Vertex *s, Vertex *t);
void augmentFlowAlongPath(Vertex *s, Vertex *t, unsigned long f);
void edmondsKarp(Graph *g, string source, string target);



#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
