#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H


#include "Graph.h"

/**
 * @brief Checks if a vertex is unvisited and there is residual capacity, then marks it as visited,
 * sets the path through which it was reached, and enqueues it.
 *
 * @details This function is used in graph traversal algorithms to visit vertices while considering residual capacities.
 * It checks if the vertex 'w' is not visited and there is residual capacity, then marks 'w' as visited, sets the path
 * through which it was reached, and enqueues it for further processing.
 *
 * @param q Reference to a queue of Vertex pointers.
 * @param e Pointer to the edge connecting the current vertex to the vertex 'w'.
 * @param w Pointer to the vertex being tested and visited.
 * @param residual The residual capacity between the current vertex and 'w'.
 *
 * @complexity The time complexity of this function is O(1) since it performs simple operations such as checking
 * whether a vertex is visited and pushing it into a queue, which take constant time.
 */
void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual);

/**
 * @brief Finds an augmenting path using Breadth-First Search.
 *
 * @details This function performs a Breadth-First Search (BFS) on the given graph 'g' starting from the source vertex 's'
 * to find an augmenting path leading to the target vertex 't'. It marks all vertices as unvisited initially, then marks
 * the source vertex 's' as visited and enqueues it. During BFS traversal, it processes outgoing and incoming edges of each
 * visited vertex to find an augmenting path.
 *
 * @param g Pointer to the graph in which the augmenting path is to be found.
 * @param s Pointer to the source vertex of the augmenting path.
 * @param t Pointer to the target vertex of the augmenting path.
 *
 * @return True if an augmenting path to the target is found, false otherwise.
 *
 * @complexity The time complexity of this function depends on the size of the graph and the number of edges. In the worst
 * case, where the graph has 'V' vertices and 'E' edges, the time complexity is O(V + E), as it performs BFS traversal.
 */
bool findAugmentingPath(Graph *g, Vertex *s, Vertex *t);

/**
 * @brief Finds the minimum residual capacity along the augmenting path from the source 's' to the target 't'.
 *
 * @details This function traverses the augmenting path from the target vertex 't' back to the source vertex 's'
 * to find the minimum residual capacity along the path. It iterates through the edges of the path, updating the
 * minimum residual capacity encountered. If the edge direction is from destination to origin, it calculates the
 * residual capacity as the edge capacity minus the flow. If the direction is from origin to destination, it calculates
 * the residual capacity as the flow. Finally, it returns the minimum residual capacity found along the augmenting path.
 *
 * @param s Pointer to the source vertex of the augmenting path.
 * @param t Pointer to the target vertex of the augmenting path.
 *
 * @return The minimum residual capacity along the augmenting path from 's' to 't'.
 *
 * @complexity The time complexity of this function depends on the length of the augmenting path, which is bounded
 * by the number of vertices in the graph. Therefore, in the worst case, where the path contains 'V' vertices, the time
 * complexity is O(V), as it iterates through each vertex on the path to find the minimum residual capacity.
 */
double findMinResidualAlongPath(Vertex *s, Vertex *t);

/**
 * @brief Augments flow along the augmenting path from the source 's' to the target 't' with the given flow value 'f'.
 *
 * @details This function traverses the augmenting path from the target vertex 't' back to the source vertex 's'
 * and updates the flow values of the edges along the path accordingly. It iterates through the edges of the path,
 * updating the flow values based on the direction of the edges. If the edge direction is from destination to origin,
 * it adds the flow value 'f' to the current flow. If the direction is from origin to destination, it subtracts
 * the flow value 'f' from the current flow. Finally, it updates the flow values of the edges along the augmenting path.
 *
 * @param s Pointer to the source vertex of the augmenting path.
 * @param t Pointer to the target vertex of the augmenting path.
 * @param f The flow value to augment along the augmenting path.
 *
 * @complexity The time complexity of this function depends on the length of the augmenting path, which is bounded
 * by the number of vertices in the graph. Therefore, in the worst case, where the path contains 'V' vertices, the time
 * complexity is O(V), as it iterates through each vertex on the path to update the flow values of the edges.
 */
void augmentFlowAlongPath(Vertex *s, Vertex *t, double f);

/**
 * @brief Implements the Edmonds-Karp algorithm for finding the maximum flow in a graph.
 *
 * @details This function implements the Edmonds-Karp algorithm, which finds the maximum flow from a source vertex 's'
 * to a target vertex 't' in a graph. It iterates through the graph until no augmenting path from 's' to 't' exists,
 * augmenting the flow along each found path. First, it finds the source and target vertices in the graph. Then, it
 * validates the source and target vertices. After that, it enters a loop where it repeatedly finds an augmenting path
 * using BFS, computes the minimum residual capacity along the path, and augments the flow along the path accordingly.
 * Once no augmenting path exists, the algorithm calculates and saves the incoming flow for each vertex in the graph.
 *
 * @param g Pointer to the graph on which the Edmonds-Karp algorithm is to be applied.
 *
 * @throws std::logic_error if the source or target vertex is invalid or if the source is equal to the target.
 *
 * @complexity The time complexity of this function depends on the number of iterations required to find the maximum flow.
 * In the worst case, where the algorithm iterates through all possible augmenting paths, the time complexity is O(V * E^2),
 * where 'V' is the number of vertices and 'E' is the number of edges in the graph.
 */
void edmondsKarp(Graph *g);


/**
 * @brief Tests the given vertex 'w' and visits it if certain conditions are met.
 *
 * @details This function checks if the vertex 'w' is not visited, if there is residual capacity, and if 'w' is not the same
 * as if the deactivated vertex. If all conditions are met, it marks 'w' as visited, sets the path through which it was reached,
 * and enqueues it for further processing.
 *
 * @param q Reference to a queue of Vertex pointers.
 * @param e Pointer to the edge connecting the current vertex to the vertex 'w'.
 * @param w Pointer to the vertex being tested and visited.
 * @param residual The residual capacity between the current vertex and 'w'.
 * @param deactivatedVertex Pointer to the deactivated vertex. 'w' will not be visited if it matches this vertex.
 *
 * @complexity The time complexity of this function is O(1) since it performs simple operations such as checking
 * whether a vertex is visited and pushing it into a queue, which take constant time.
 */
void testAndVisitWithDeactivatedVertex(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual, Vertex *deactivatedVertex);

/**
 * @brief Finds an augmenting path using Breadth-First Search (BFS), excluding a specified deactivated vertex.
 *
 * @details This function performs a Breadth-First Search (BFS) on the given graph 'g' starting from the source vertex 's'
 * to find an augmenting path leading to the target vertex 't'. It marks all vertices as unvisited initially, then marks
 * the source vertex 's' as visited and enqueues it. During BFS traversal, it processes outgoing and incoming edges of each
 * visited vertex to find an augmenting path, excluding the specified deactivated vertex from being visited.
 *
 * @param g Pointer to the graph in which the augmenting path is to be found.
 * @param s Pointer to the source vertex of the augmenting path.
 * @param t Pointer to the target vertex of the augmenting path.
 * @param deactivatedVertex Pointer to the vertex that should not be visited during path finding.
 *
 * @return True if an augmenting path to the target is found, false otherwise.
 *
 * @complexity The time complexity of this function depends on the size of the graph and the number of edges. In the worst
 * case, where the graph has 'V' vertices and 'E' edges, the time complexity is O(V + E), as it performs BFS traversal.
 */
bool findAugmentingPathWithDeactivatedVertex(Graph *g, Vertex *s, Vertex *t, Vertex *deactivatedVertex);

/**
 * @brief Implements the Edmonds-Karp algorithm for finding the maximum flow in a graph, excluding a specified deactivated vertex.
 *
 * @details This function implements the Edmonds-Karp algorithm, which finds the maximum flow from a source vertex 's'
 * to a target vertex 't' in a graph, while excluding a specified deactivated vertex from the path finding process. It iterates
 * through the graph until no augmenting path from 's' to 't' exists, augmenting the flow along each found path. First, it finds
 * the source and target vertices in the graph, as well as the deactivated vertex. Then, it validates the source and target vertices.
 * After that, it enters a loop where it repeatedly finds an augmenting path using BFS, computes the minimum residual capacity along
 * the path, and augments the flow along the path accordingly. Once no augmenting path exists, the algorithm calculates and saves
 * the incoming flow for each vertex in the graph, excluding the deactivated vertex from the flow calculation.
 *
 * @param g Pointer to the graph on which the Edmonds-Karp algorithm is to be applied.
 * @param deactivated The code of the vertex to be deactivated during path finding.
 *
 * @throws std::logic_error if the source or target vertex is invalid or if the source is equal to the target.
 *
 * @complexity The time complexity of this function depends on the number of iterations required to find the maximum flow.
 * In the worst case, where the algorithm iterates through all possible augmenting paths, the time complexity is O(V * E^2),
 * where 'V' is the number of vertices and 'E' is the number of edges in the graph.
 */
void edmondsKarpWithDeactivatedVertex(Graph *g, const string &deactivated);


/**
 * @brief Tests the given vertex 'w' and visits it if certain conditions are met, excluding a specified edge.
 *
 * @details This function checks if the vertex 'w' is not visited, if there is residual capacity, and if the edge connecting
 * 'w' to its origin and destination vertices does not match the specified service points. If all conditions are met,
 * it marks 'w' as visited, sets the path through which it was reached, and enqueues it for further processing. Additionally,
 * if the graph is bidirectional, it checks both directions of the edge. If unidirectional, it only checks the given direction.
 *
 * @param q Reference to a queue of Vertex pointers.
 * @param e Pointer to the edge connecting the current vertex to the vertex 'w'.
 * @param w Pointer to the vertex being tested and visited.
 * @param residual The residual capacity between the current vertex and 'w'.
 * @param servicePointA Code of the first service point to be excluded from the edge check.
 * @param servicePointB Code of the second service point to be excluded from the edge check.
 * @param unidirectional Flag indicating whether the graph is unidirectional or bidirectional.
 *
 * @complexity The time complexity of this function is O(1) since it performs simple operations such as checking
 * whether a vertex is visited and pushing it into a queue, which take constant time.
 */
void testAndVisitWithDeactivatedEdge(std::queue< Vertex*> &q, Edge *e, Vertex *w, double residual, const string &servicePointA, const string &servicePointB, bool unidirectional);

/**
 * @brief Finds an augmenting path using Breadth-First Search (BFS), excluding specified edges.
 *
 * @details This function performs a Breadth-First Search (BFS) on the given graph 'g' starting from the source vertex 's'
 * to find an augmenting path leading to the target vertex 't'. It marks all vertices as unvisited initially, then marks
 * the source vertex 's' as visited and enqueues it. During BFS traversal, it processes outgoing and incoming edges of each
 * visited vertex to find an augmenting path, excluding edges that match the specified service points. Additionally, if the
 * graph is bidirectional, it checks both directions of each edge. If unidirectional, it only checks the given direction.
 *
 * @param g Pointer to the graph in which the augmenting path is to be found.
 * @param s Pointer to the source vertex of the augmenting path.
 * @param t Pointer to the target vertex of the augmenting path.
 * @param servicePointA Code of the first service point to be excluded from edge checks.
 * @param servicePointB Code of the second service point to be excluded from edge checks.
 * @param unidirectional Flag indicating whether the graph is unidirectional or bidirectional.
 *
 * @return True if an augmenting path to the target is found, false otherwise.
 *
 * @complexity The time complexity of this function depends on the size of the graph and the number of edges. In the worst
 * case, where the graph has 'V' vertices and 'E' edges, the time complexity is O(V + E), as it performs BFS traversal.
 */
bool findAugmentingPathWithDeactivatedEdge(Graph *g, Vertex *s, Vertex *t, const string &servicePointA, const string &servicePointB, bool unidirectional);

/**
 * @brief Implements the Edmonds-Karp algorithm for finding the maximum flow in a graph, excluding specified edges.
 *
 * @details This function implements the Edmonds-Karp algorithm, which finds the maximum flow from a source vertex 's'
 * to a target vertex 't' in a graph, while excluding edges that match the specified service points. It iterates
 * through the graph until no augmenting path from 's' to 't' exists, augmenting the flow along each found path. First,
 * it finds the source and target vertices in the graph. Then, it validates the source and target vertices. After that,
 * it enters a loop where it repeatedly finds an augmenting path using BFS, computes the minimum residual capacity along
 * the path, and augments the flow along the path accordingly, excluding edges based on the specified service points.
 *
 * @param g Pointer to the graph on which the Edmonds-Karp algorithm is to be applied.
 * @param servicePointA Code of the first service point to be excluded from edge checks.
 * @param servicePointB Code of the second service point to be excluded from edge checks.
 * @param unidirectional Flag indicating whether the graph is unidirectional or bidirectional.
 *
 * @throws std::logic_error if the source or target vertex is invalid or if the source is equal to the target.
 *
 * @complexity The time complexity of this function depends on the number of iterations required to find the maximum flow.
 * In the worst case, where the algorithm iterates through all possible augmenting paths, the time complexity is O(V * E^2),
 * where 'V' is the number of vertices and 'E' is the number of edges in the graph.
 */
void edmondsKarpWithDeactivatedEdge(Graph *g, const string &servicePointA, const string &servicePointB, bool unidirectional);

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_ALGORITHMS_H
