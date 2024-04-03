#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H


#include "Graph.h"

/**
 * @brief Tests the given vertex 'w' and visits it if conditions are met.
 * @param q The queue used for Breadth-First Search.
 * @param e The edge connecting the current vertex to 'w'.
 * @param w The vertex to be tested and visited.
 * @param residual The residual capacity of the edge.
 */
void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual);

/**
 * @brief Finds an augmenting path using Breadth-First Search.
 * @param g Pointer to the graph.
 * @param s Pointer to the source vertex.
 * @param t Pointer to the target vertex.
 * @return true if an augmenting path to the target is found, false otherwise.
 */
bool findAugmentingPath(Graph *g, Vertex *s, Vertex *t);

/**
 * @brief Finds the minimum residual capacity along the augmenting path.
 * @param s Pointer to the source vertex.
 * @param t Pointer to the target vertex.
 * @return The minimum residual capacity along the augmenting path.
 */
double findMinResidualAlongPath(Vertex *s, Vertex *t);

/**
 * @brief Augments flow along the augmenting path with the given flow value
 * @param s Pointer to the source vertex.
 * @param t Pointer to the target vertex.
 * @param f The amount by which to augment the flow.
 */
void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);

/**
 * @brief Implements the Edmonds-Karp algorithm for maximum flow.
 * @param g Pointer to the graph.
 * @param source The code of the source vertex.
 * @param target The code of the target vertex.
 * @throws logic_error if source or target vertices are invalid.
 */
void edmondsKarp(Graph *g, string source, string target);


/**
 * @brief Tests the given vertex 'w' and visits it if conditions are met, now with a deactivated vertex.
 * @param q The queue used for Breadth-First Search.
 * @param e The edge connecting the current vertex to 'w'.
 * @param w The vertex to be tested and visited.
 * @param residual The residual capacity of the edge.
 * @param deactivatedVertex The vertex that should be excluded from consideration.
 */
void testAndVisitWithDeactivatedVertex(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual, Vertex *deactivatedVertex);

/**
 * @brief Finds an augmenting path using Breadth-First Search, now with a deactivated vertex.
 * @param g Pointer to the graph.
 * @param s Pointer to the source vertex.
 * @param t Pointer to the target vertex.
 * @param deactivatedVertex The vertex that should be excluded from consideration.
 * @return true if an augmenting path to the target is found, false otherwise.
 */
bool findAugmentingPathWithDeactivatedVertex(Graph *g, Vertex *s, Vertex *t, Vertex *deactivatedVertex);

/**
 * @brief Implements the Edmonds-Karp algorithm for maximum flow, now with a deactivated vertex.
 * @param g Pointer to the graph.
 * @param source Pointer to the source vertex.
 * @param target Pointer to the target vertex.
 * @param deactivated The vertex that should be excluded from consideration.
 */
void edmondsKarpWithDeactivatedVertex(Graph *g, const string source, const string target, const string deactivated);


/**
 * @brief Tests the given vertex 'w' and visits it if conditions are met, now with a deactivated edge.
 * @param q The queue used for Breadth-First Search.
 * @param e The edge connecting the current vertex to 'w'.
 * @param w The vertex to be tested and visited.
 * @param residual The residual capacity of the edge.
 * @param servicePointA The code of the first service point of the deactivated edge.
 * @param servicePointB The code of the second service point of the deactivated edge.
 * @param unidirectional Flag indicating if the edge is unidirectional.
 */
void testAndVisitWithDeactivatedEdge(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual, const string servicePointA, const string servicePointB, bool unidirectional);

/**
 * @brief Finds an augmenting path using Breadth-First Search, now with a deactivated edge.
 * @param g Pointer to the graph.
 * @param s Pointer to the source vertex.
 * @param t Pointer to the target vertex.
 * @param servicePointA The code of the first service point of the deactivated edge.
 * @param servicePointB The code of the second service point of the deactivated edge.
 * @param unidirectional Flag indicating if the edge is unidirectional.
 * @return true if an augmenting path to the target is found, false otherwise.
 */
bool findAugmentingPathWithDeactivatedEdge(Graph *g, Vertex *s, Vertex *t, const string servicePointA, const string servicePointB, bool unidirectional);

/**
 * @brief Implements the Edmonds-Karp algorithm for maximum flow, now with a deactivated edge.
 * @param g Pointer to the graph.
 * @param source Pointer to the source vertex.
 * @param target Pointer to the target vertex.
 * @param servicePointA The code of the first service point of the deactivated edge.
 * @param servicePointB The code of the second service point of the deactivated edge.
 * @param unidirectional Flag indicating if the edge is unidirectional.
 */
void edmondsKarpWithDeactivatedEdge(Graph *g, const string source, const string target, const string servicePointA, const string servicePointB, bool unidirectional);

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
