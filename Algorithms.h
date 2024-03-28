#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H


#include "Graph.h"

void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual);
bool findAugmentingPath(Graph *g, Vertex *s, Vertex *t);
double findMinResidualAlongPath(Vertex *s, Vertex *t);
void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);
void edmondsKarp(Graph *g, string source, string target);

void testAndVisitWithDeactivatedVertex(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual, Vertex *deactivatedVertex);
bool findAugmentingPathWithDeactivatedVertex(Graph *g, Vertex *s, Vertex *t, Vertex *deactivatedVertex);
void edmondsKarpWithDeactivatedVertex(Graph *g, const string source, const string target, const string deactivated);

void testAndVisitWithDeactivatedEdge(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual, const string servicePointA, const string servicePointB, bool unidirectional);
bool findAugmentingPathWithDeactivatedEdge(Graph *g, Vertex *s, Vertex *t, const string servicePointA, const string servicePointB, bool unidirectional);
void edmondsKarpWithDeactivatedEdge(Graph *g, const string source, const string target, const string servicePointA, const string servicePointB, bool unidirectional);

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
