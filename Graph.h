#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H


#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <unordered_map>
#include "WaterReservoir.h"
#include "DeliverySite.h"
#include "GraphMetrics.h"

using namespace std;

class Edge;

#define INF std::numeric_limits<double>::max()

/************************* Vertex  **************************/

enum class VertexType { WaterReservoir, PumpingStation, DeliverySite, MainSource, MainTarget };

/**
* @brief Represents a vertex in the graph, corresponding to
* a WaterReservoir, or a PumpingStation, or a DeliverySite,
* or the MainSource, or the MainTarget .
*/
class Vertex {
private:
    string code;            // code of the node
    VertexType type;        // type of the node
    vector<Edge *> adj;  // outgoing edges

    double flow = 0;

    // auxiliary fields
    bool visited = false; // used by DFS, BFS, Prim ...
    double dist = 0;
    Edge *path = nullptr;

    vector<Edge *> incoming; // incoming edges

public:

    /**
     * @brief Constructor for Vertex class.
     *
     * @param code The code associated with the vertex.
     * @param type The type of the vertex.
     */
    Vertex(string code, VertexType type);

    /**
     * @brief Less-than comparison operator for vertices based on distance.
     *
     * @param vertex The vertex to compare against.
     * @return True if this vertex's distance is less than the other vertex's distance, otherwise false.
     */
    bool operator<(Vertex & vertex) const;

    /**
     * @brief Get the code associated with the vertex.
     *
     * @return The code of the vertex.
     */
    [[nodiscard]] string getCode() const;

    /**
     * @brief Get the type of the vertex.
     *
     * @return The type of the vertex.
     */
    [[nodiscard]] VertexType getType() const;

    /**
     * @brief Get the adjacent edges of the vertex.
     *
     * @return Vector of pointers to adjacent edges.
     */
    [[nodiscard]] vector<Edge *> getAdj() const;

    /**
     * @brief Check if the vertex has been visited.
     *
     * @return True if the vertex has been visited, otherwise false.
     */
    [[nodiscard]] bool isVisited() const;

    /**
     * @brief Get the path associated with the vertex.
     *
     * @return Pointer to the path edge.
     */
    [[nodiscard]] Edge *getPath() const;

    /**
     * @brief Get the incoming edges of the vertex.
     *
     * @return Vector of pointers to incoming edges.
     */
    [[nodiscard]] vector<Edge *> getIncoming() const;

    /**
     * @brief Get the flow value associated with the vertex.
     *
     * @return The flow value of the vertex.
     */
    [[nodiscard]] double getFlow() const;

    /**
     * @brief Check if the vertex has non-zero flow.
     *
     * @return True if the vertex has non-zero flow, otherwise false.
     *
     * @complexity O(E)
     */
    [[nodiscard]] bool hasFlow() const;

    /**
     * @brief Set the flow value associated with the vertex.
     *
     * @param value The flow value to set.
     */
    void setFlow(double value);

    /**
     * @brief Update the flow value associated with the vertex based on incoming edges.
     *
     * @complexity O(n) where n is the number of incoming edges or, if there's no incoming edges the number of out edges.
     */
    void updateFlow();

    /**
     * @brief Set the visited status of the vertex.
     *
     * @param isVisited True if the vertex is visited, otherwise false.
     */
    void setVisited(bool isVisited);

    /**
     * @brief Set the path associated with the vertex.
     *
     * @param newPath Pointer to the new path edge.
     */
    void setPath(Edge *newPath);

    /**
     * @brief Add an edge between this vertex and a destination vertex.
     *
     * @param dest Pointer to the destination vertex.
     * @param c Capacity of the edge.
     * @param f Flow of the edge.
     *
     * @return Pointer to the newly added edge.
     */
    Edge * addEdge(Vertex *dest, double c, double f = 0);

    /**
     * @brief Find an edge between this vertex and a destination vertex.
     *
     * @param destVertex Pointer to the destination vertex.
     *
     * @return Pointer to the found edge if exists, otherwise nullptr.
     */
    Edge * findEdge(Vertex *destVertex);
};

/********************** Edge  ****************************/

/**
* @brief Class representing an edge in a graph.
*/
class Edge {
private:
    Vertex *dest; // destination vertex
    double capacity; // edge capacity

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow{}; // for flow-related problems

public:
    /**
    * @brief Constructor for the Edge class.
    *
    * @details Initializes an instance of the Edge class with the specified origin vertex, destination vertex and edge capacity.
    *
    * @param orig Pointer to the origin vertex.
    * @param dest Pointer to the destination vertex.
    * @param capacity Capacity of the edge.
    */
    Edge(Vertex *orig, Vertex *dest, double capacity);

    /**
    * @brief Get the destination vertex associated with the edge.
    *
    * @return Pointer to the destination vertex.
    */
    [[nodiscard]] Vertex * getDest() const;

    /**
     * @brief Get the capacity associated with the edge.
     *
     * @return The capacity of the edge.
     *
     */
    [[nodiscard]] double getCapacity() const;

    /**
     * @brief Get the origin vertex associated with the edge.
     *
     * @return Pointer to the origin Vertex.
     *
     */
    [[nodiscard]] Vertex * getOrig() const;

    /**
     * @brief Get reverse edge associated with the edge.
     *
     * @return Pointer to the reverse edge.
     *
     */
    [[nodiscard]] Edge *getReverse() const;

    /**
     * @brief Get the flow associated with the edge.
     *
     * @return The flow of the edge.
     *
     */
    [[nodiscard]] double getFlow() const;

    /**
     * @brief Set the reverse edge associated with this edge.
     *
     * @param reverseEdge Pointer to the reverse edge to set.
     */
    void setReverse(Edge *reverseEdge);

    /**
     * @brief Set the flow of the edge.
     *
     * @param newFlow The new flow value to set.
     */
    void setFlow(double newflow);
};

/********************** Graph  ****************************/

/**
* @brief Class representing a graph.
*/
class Graph {
private:
    unordered_map<string, Vertex *> vertices;    // vertex set
    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

public:

    /**
     * @brief Creates a deep copy of the graph.
     *
     * @details This function creates a new graph that is an exact copy of the original graph,
     * including all vertices and edges. It iterates over all vertices in the original graph,
     * copies them, and adds them to the new graph. Then, it iterates over all edges in the
     * original graph, copies them along with their attributes, and adds them to the new graph.
     * After copying, it updates the flow of all vertices in the new graph to match the original
     * graph. Finally, it returns a pointer to the newly created graph.
     *
     * @return A pointer to the newly created copy of the graph.
     *
     * @complexity The time complexity of this function depends on the number of vertices and edges
     * in the original graph. Let V be the number of vertices and E be the number of edges in the
     * original graph. The worst-case time complexity is O(E + V) due to the two loops iterating
     * over vertices and edges, respectively. Additionally, updating the flow of vertices has a
     * time complexity of O(V).
     */
    Graph *copyGraph();

    /**
     * @brief Calculates various metrics for the graph based on delivery site flow data.
     *
     * @details This function calculates absolute and relative metrics for the graph based on the flow values of the delivery sites.
     * Absolute metrics include average absolute difference, absolute variance, absolute standard deviation, and maximum absolute difference.
     * Relative metrics include average relative difference, relative variance, relative standard deviation, and maximum relative difference.
     * It also determines the maximum flow and total demand in the graph. The calculated metrics are encapsulated in a GraphMetrics object
     * and returned.
     *
     * @param deliverySites A pointer to an unordered map containing delivery site objects.
     *
     * @return A GraphMetrics object containing various metrics calculated for the graph.
     *
     * @complexity The time complexity of this function depends on the number of edges in the graph.
     * Let, E be the total number of edges. The worst-case time complexity is O(V + E)
     * due to the nested loops iterating over vertices and edges.
     */
    GraphMetrics calculateMetrics(const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Calculates the total demand and maximum flow in the graph.
     *
     * @details This function iterates over all delivery sites in the provided unordered map and
     * calculates the total demand and maximum flow in the graph based on the flow values of the delivery
     * sites. It sums up the demand of each delivery site to calculate the total demand and adds the flow
     * of each delivery site to calculate the maximum flow. It returns a pair containing the total demand
     * and maximum flow.
     *
     * @param deliverySites A pointer to an unordered map containing delivery site objects.
     *
     * @return A pair containing the total demand and maximum flow in the graph.
     *
     * @complexity The time complexity of this function depends on the number of delivery sites in the
     * provided unordered map. In the worst case, it iterates over all delivery sites once, resulting in
     * a time complexity of O(n), where n is the number of delivery sites.
     */
    pair<double, double> getTotalDemandAndMaxFlow(const unordered_map<string, DeliverySite *> *deliverySites) const;

    /**
     * @brief Retrieves the code of the main source vertex in the graph.
     *
     * @details This function returns the code of the main source vertex, which is a specific vertex
     * used in flow algorithms such as max flow.
     *
     * @return The code of the main source vertex.
     *
     * @complexity This function has a time complexity of O(1) as it performs a simple retrieval
     * operation.
     */
    string getMainSourceCode();

    /**
     * @brief Retrieves the code of the main target vertex in the graph.
     *
     * @details This function returns the code of the main target vertex, which is a specific vertex
     * used in flow algorithms such as max flow.
     *
     * @return The code of the main target vertex.
     *
     * @complexity This function has a time complexity of O(1) as it performs a simple retrieval
     * operation.
     */
    string getMainTargetCode();

    /**
     * @brief Finds a vertex in the graph based on its code.
     *
     * @details This function searches for a vertex with the given code in the graph's vertex set. If
     * the vertex is found, a pointer to it is returned; otherwise, nullptr is returned to indicate that
     * the vertex could not be found.
     *
     * @param code The code of the vertex to be found.
     *
     * @return A pointer to the vertex if found, nullptr otherwise.
     *
     * @complexity The time complexity of this function depends on the time complexity of finding an
     * element in an unordered_map, which is O(1) in the worst case.
     */
    Vertex *findVertex(const string &code) const;

    /**
     * @brief Adds a vertex to the graph.
     *
     * @details This function adds a vertex with the given code and type to the graph. If a vertex with
     * the same code already exists in the graph, the function returns false, indicating that the vertex
     * could not be added. Otherwise, it creates a new vertex, inserts it into the graph's vertex set,
     * and returns true to indicate a successful addition.
     *
     * @param code The code of the vertex to be added.
     * @param type The type of the vertex to be added.
     *
     * @return True if the vertex is successfully added, false otherwise.
     *
     * @complexity The time complexity of this function depends on the time complexity of finding a
     * vertex in the graph, which is O(1) in the worst case, and inserting a new vertex into
     * the vertex set, which is also O(1) in the worst case.
     */
    bool addVertex(const string &code, const VertexType &type);

    /**
     * @brief Adds an edge between two vertices in the graph.
     *
     * @details This function adds an edge from the vertex with the source code to the vertex with the
     * destination code. If either of the vertices does not exist in the graph, the function returns
     * false, indicating that the edge could not be added. Otherwise, it adds the edge to the origin
     * vertex and checks if there is a corresponding reverse edge in the destination vertex. If a
     * reverse edge exists and has the same capacity as the newly added edge, it sets the reverse
     * pointers for both edges to maintain bidirectionality.
     *
     * @param source The code of the source vertex.
     * @param dest The code of the destination vertex.
     * @param c The capacity of the edge.
     * @param f The flow through the edge from source to dest.
     *
     * @return True if the edge is successfully added, false otherwise.
     *
     * @complexity The time complexity of this function depends on the time complexity of finding
     * vertices in the graph, adding edges to the vertices, and setting reverse pointers, all of which
     * are O(1) in the worst case.
     */
    bool addEdge(const string &source, const string &dest, double c, double f = 0) const;

    /**
     * @brief Adds a bidirectional edge between two vertices in the graph.
     *
     * @details This function adds a bidirectional edge between the vertices with the given source and
     * destination codes. If either of the vertices does not exist in the graph, the function returns
     * false, indicating that the edge could not be added. Otherwise, it creates two edges: one from
     * source to dest and another from dest to source, each with the specified capacity and flow.
     * Additionally, it sets the reverse pointers for the edges to maintain bidirectionality.
     *
     * @param source The code of the source vertex.
     * @param dest The code of the destination vertex.
     * @param c The capacity of the edge.
     * @param flow The flow through the edge from source to dest.
     * @param reverseFlow The flow through the reverse edge from dest to source.
     *
     * @return True if the bidirectional edge is successfully added, false otherwise.
     *
     * @complexity The time complexity of this function depends on the time complexity of finding
     * vertices in the graph, adding edges to the vertices, and setting reverse pointers, all of which
     * are O(1) in the worst case.
     */
    bool addBidirectionalEdge(const string &source, const string &dest, double c, double flow = 0, double reverseFlow = 0) const;

    /**
     * @brief Retrieves the set of vertices in the graph.
     *
     * @return An unordered map containing the vertices of the graph, where the keys are vertex codes
     * and the values are pointers to the corresponding vertices.
     *
     * @details This function returns a copy of the unordered map containing all vertices in the graph.
     * It does not modify the original graph.
     *
     * @complexity O(1)
     */
    unordered_map<string, Vertex *> getVertexSet() const;

    /**
     * @brief Sets the flow value of all edges in the graph to a specified value.
     *
     * @details This function iterates through all vertices in the graph and for each vertex, it
     * iterates through its adjacent edges. It then sets the flow value of each edge to the specified
     * value 'f' by calling the setFlow() function of the Edge class for each edge.
     *
     * @param f The flow value to set for all edges.
     *
     * @complexity The time complexity of this function depends on the number of vertices and edges
     * in the graph. Let V be the number of vertices and E be the number of edges. The worst-case time
     * complexity is O(V + E), as it iterates through all vertices and edges once to set the flow value
     * of each edge.
     */
    void setAllEdgesFlow(double f);

    /**
     * @brief Sets the flow value of all vertices in the graph to a specified value.
     *
     * @details This function iterates through all vertices in the graph and sets the flow value
     * of each vertex to the specified value 'f' by calling the setFlow() function of the Vertex
     * class for each vertex.
     *
     * @param f The flow value to set for all vertices.
     *
     * @complexity The time complexity of this function depends on the number of vertices in the graph.
     * Let V be the number of vertices. The time complexity is O(V), as it iterates through all vertices
     * once and sets the flow value of each vertex.
     */
    void setAllVerticesFlow(double f);

    /**
     * @brief Updates the flow values of all vertices in the graph.
     *
     * @details This function iterates through all vertices in the graph and updates the flow value
     * of each vertex by calling the updateFlow() function of the Vertex class for each vertex.
     *
     * @complexity The time complexity of this function depends on the number of vertices in the graph.
     * Let V be the number of vertices. The time complexity is O(V), as it iterates through all vertices
     * once and updates the flow value of each vertex.
     */
    void updateAllVerticesFlow();

    /**
     * @brief Creates the main source vertex and connects it to water reservoirs.
     *
     * @details This function creates the main source vertex in the graph and connects it to each
     * water reservoir with an edge representing the maximum delivery capacity of the reservoir.
     * It iterates through the water reservoirs provided in the unordered map and adds an edge from
     * the main source vertex to each water reservoir, with the capacity equal to the maximum delivery
     * capacity of the reservoir and the current flow in the corresponding edge.
     *
     * @param waterReservoirs A pointer to the unordered map containing water reservoir data.
     *
     * @complexity The time complexity of this function depends on the number of water reservoirs in
     * the input unordered map. If there are n water reservoirs, the time complexity is O(n).
     */
    void createMainSource(const unordered_map<string, WaterReservoir *> *waterReservoirs);

    /**
     * @brief Creates the main target vertex and connects it to delivery sites.
     *
     * @details This function creates the main target vertex in the graph and connects it to each
     * delivery site with an edge representing the demand of the delivery site. It iterates through
     * the delivery sites provided in the unordered map and adds an edge from each delivery site to
     * the main target vertex, with the capacity equal to the demand of the delivery site and the
     * current flow in the corresponding edge.
     *
     * @param deliverySites A pointer to the unordered map containing delivery site data.
     *
     * @complexity The time complexity of this function depends on the number of delivery sites in
     * the input unordered map. If there are n delivery sites, the time complexity is O(n).
     */
    void createMainTarget(const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Computes the maximum flow in the graph using the Edmonds-Karp algorithm.
     *
     * @details This function computes the maximum flow in the graph using the Edmonds-Karp algorithm.
     * It first creates the main source and main target vertices based on the provided water reservoirs
     * and delivery sites. Then, it initializes the flow of all edges and vertices to zero. Finally, it
     * applies the Edmonds-Karp algorithm to compute the maximum flow.
     *
     * @param waterReservoirs A pointer to the unordered map containing water reservoir data.
     * @param deliverySites A pointer to the unordered map containing delivery site data.
     *
     * @complexity The time complexity of this function depends on the implementation of the Edmonds-Karp
     * algorithm, which has a worst-case time complexity of O(V * E^2), where V is the number of vertices
     * and E is the number of edges in the graph.
     */
    void maxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Optimizes the load distribution in the graph to improve flow characteristics.
     *
     * @details This function optimizes the load distribution in the graph to improve flow characteristics.
     * It calculates the initial metrics of the graph using the provided delivery sites, initializes the final metrics
     * to be the same as the initial metrics, and retrieves all edges in the graph. Then, it iteratively performs load
     * optimization until convergence criteria are met or the maximum number of iterations is reached. In each iteration,
     * it sorts the edges based on the difference between capacity and flow, and then for each edge, it finds paths
     * between its source and destination vertices. It selects the path with the maximum minimum residual capacity
     * and adjusts the flow along the path accordingly. After each iteration, it updates the final metrics and checks
     * for convergence. The optimization process continues until convergence criteria are met or the maximum number
     * of iterations is reached. Finally, it updates the flow values of all vertices in the graph.
     *
     * @param deliverySites Pointer to the unordered map containing delivery sites information.
     *
     * @complexity The time complexity of this function depends on the number of edges in the graph, the number of
     * delivery sites, and the efficiency of the path-finding algorithm. Let V be the number of vertices and E be
     * the number of edges. The worst-case time complexity is O(n(E * (V + E))), where n is the number of iterations.
     */
    void optimizeLoad(const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Finds all paths from a source vertex to a destination vertex in the graph.
     *
     * @details This function finds all paths from a given source vertex to a destination vertex in the graph.
     * It initializes a vector of vectors of edges to store the paths found. Then, it initializes an empty vector
     * representing the current path and marks all vertices as unvisited. Next, it invokes the depth-first search (DFS)
     * algorithm to explore paths from the source to the destination. Finally, it returns the vector of paths found.
     *
     * @param source The code of the source vertex.
     * @param dest The code of the destination vertex.
     * @return A vector of vectors of edges representing all paths from the source to the destination.
     *
     * @complexity The time complexity of this function depends on the size and structure of the graph, as well as
     * the number of paths found. It calls the DFS function, which has a time complexity of O(V + E), where V is the number
     * of vertices and E is the number of edges. Therefore, the overall time complexity is also O(V + E) in the worst case.
     * However, in practice, it often performs efficiently, especially if the graph is sparse or there are a limited number
     * of paths from the source to the destination.
     */
    vector<vector<Edge *>> getPaths(const string& source, const string &dest);

    /**
     * @brief Performs depth-first search (DFS) to find paths from a source vertex to a destination vertex in the graph.
     *
     * @details This function recursively performs depth-first search (DFS) starting from the current vertex
     * to find paths from a source vertex to a destination vertex in the graph. It marks visited vertices to avoid
     * infinite loops and maintains a vector of edges representing the current path being explored. If the destination
     * vertex is reached, the current path is added to the vector of paths. Otherwise, it continues exploring adjacent
     * vertices that have available capacity for flow. This function is used to find all possible paths from a source
     * to a destination in the graph.
     *
     * @param current The code of the current vertex being visited.
     * @param dest The code of the destination vertex.
     * @param path The vector of edges representing the current path being explored.
     * @param paths The vector of vectors of edges representing all found paths from the source to the destination.
     *
     * @complexity A DFS has a time complexity of O(V+E). Where V is the number of vertices and E is the number of edges.
     */
    void dfs(const string &current, const string &dest, vector<Edge *> &path, vector<vector<Edge *>> &paths);

    /**
     * @brief Marks a pumping station or reservoir as out of commission and adjusts the flow in the graph.
     *
     * @details This function marks a pumping station or reservoir as out of commission by deactivating
     * the corresponding vertex and adjusts the flow in the graph. It first runs the Edmonds-Karp algorithm
     * to find the maximum flow in the graph. Then, it deactivates the vertex corresponding to the provided code.
     * After deactivating the vertex, it recalculates the maximum flow in the graph using the Edmonds-Karp algorithm
     * with the deactivated vertex. Finally, it updates the flow values of all vertices in the graph.
     *
     * @param code Pointer to the code of the pumping station or reservoir to be deactivated.
     *
     * @complexity The time complexity of this function depends on the size and structure of the graph,
     * as well as the efficiency of the Edmonds-Karp algorithm. In the worst case, where the graph is dense
     * and the Edmonds-Karp algorithm has to run multiple times, the time complexity can be O(V * (E^2)), where V
     * is the number of vertices and E is the number of edges. However, in practice, it performs efficiently,
     * because it does not execute the Edmonds-Karp from the beginning.
     */
    void stationOutOfCommission(string const *code);

    /**
     * @brief Marks a pipeline between two service points as out of commission and adjusts the flow in the graph.
     *
     * @details This function marks a pipeline between two service points as out of commission by deactivating
     * the corresponding vertices and and adjusts the flow in the graph. Then, it deactivates the vertex corresponding
     * to the origin service point. If the pipeline is bidirectional, it also deactivates the vertex corresponding to
     * the destination service point. After deactivating the vertices, it recalculates the maximum flow in the graph
     * using the Edmonds-Karp algorithm with the deactivated edges. Finally, it updates the flow values of all vertices in the graph.
     *
     * @param servicePointA Pointer to the code of the first service point.
     * @param servicePointB Pointer to the code of the second service point.
     * @param unidirectional Indicates whether the pipeline is unidirectional (true) or bidirectional (false).
     *
     * @complexity The time complexity of this function depends on the size and structure of the graph,
     * as well as the efficiency of the Edmonds-Karp algorithm. In the worst case, where the graph is dense
     * and the Edmonds-Karp algorithm has to run multiple times, the time complexity can be O(V * (E^2)), where V
     * is the number of vertices and E is the number of edges. However, in practice, it performs efficiently,
     * because it does not execute the Edmonds-Karp from the beginning.
     */
    void pipelineOutOfCommission(string const *servicePointA, string const *servicePointB, bool unidirectional);

    /**
     * @brief Deactivates a vertex and adjusts the flow in the graph.
     *
     * @details This function deactivates a vertex and adjusts the flow in the graph.
     * It iteratively checks for flow cycles involving the deactivated vertex and deactivates them by reducing
     * the flow along the cycle. If no flow cycles are detected, it finds and deactivates flow paths originating
     * from the deactivated vertex. This process continues until the deactivated vertex has no more flow.
     *
     * @param deactivatedVertex The vertex to deactivate.
     *
     * @complexity The time complexity of this function depends on the size and structure of the graph, as well as the
     * number of flow cycles and flow paths involving the deactivated vertex. In the worst case, where the deactivated
     * vertex has flow connected to all other vertices in the graph, and there are many flow cycles and paths, the
     * function's time complexity can be O(V * (E^2)), where V is the number of vertices and E is the number of edges.
     * However, in practice, it often performs efficiently since it terminates once the deactivated vertex has no more flow.
     */
    void deactivateVertex(Vertex *v);

    /**
     * @brief Detects and deactivates flow cycles originating from a specified vertex.
     *
     * @details This function detects and deactivates flow cycles in the graph originating from the specified vertex.
     * It performs a breadth-first search (BFS) starting from the deactivated vertex to identify cycles with positive
     * flow. If a cycle is found, it deactivates the flow along the cycle by updating the flow values of the edges.
     * The function returns true if a cycle is detected and deactivated; otherwise, it returns false.
     *
     * @param deactivatedVertex The vertex from which to start detecting flow cycles.
     *
     * @return True if a flow cycle is detected and deactivated, false otherwise.
     *
     * @complexity The time complexity of this function depends on the size of the graph and the presence of flow
     * cycles. In the worst case, where there are flow cycles originating from the deactivated vertex, the function
     * performs a BFS traversal, which has a time complexity of O(V + E), where V is the number of vertices and E is
     * the number of edges in the graph. Additionally, the function updates flow values along the cycle, which adds
     * additional time complexity proportional to the number of edges in the cycle.
     */
    bool detectAndDeactivateFlowCycles(Vertex *deactivatedVertex);

    /**
     * @brief Finds and deactivates a flow path originating from a specified vertex.
     *
     * @details This function finds and deactivates a flow path in the graph originating from the specified vertex.
     * It first performs a BFS traversal from the deactivated vertex to find a path to the main target vertex. Then,
     * it performs another BFS traversal from the main source vertex to the deactivated vertex. Finally, it identifies
     * the common path between the two traversals and deactivates the flow along this path. The function updates the
     * flow values of the edges along the path to reduce the flow. If no path is found or the path has no positive flow,
     * no action is taken.
     *
     * @param deactivatedVertex The vertex from which to start searching for a flow path.
     *
     * @complexity The time complexity of this function depends on the size of the graph and the length of the flow path
     * from the deactivated vertex to the main target vertex. In the worst case, where the flow path covers the entire
     * graph, the function performs two BFS traversals, each with a time complexity of O(V + E), where V is the number
     * of vertices and E is the number of edges in the graph. Additionally, the function updates flow values along the
     * identified path, which adds additional time complexity proportional to the number of edges in the path.
     */
    void findAndDeactivateFlowPath(Vertex *deactivatedVertex);
};

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H
