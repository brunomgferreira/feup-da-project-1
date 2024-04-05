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

    void deleteEdge(Edge *edge);

public:
    Vertex(string code, VertexType type);
    bool operator<(Vertex & vertex) const;

    string getCode() const;
    VertexType getType() const;
    vector<Edge *> getAdj() const;
    bool isVisited() const;
    Edge *getPath() const;
    vector<Edge *> getIncoming() const;

    double getFlow() const;
    void setFlow(double value);
    void updateFlow();

    /**
     * @brief Checks if the vertex has flow.
     * @details Time complexity: O(E).
     * @return true if the vertex has flow, false otherwise.
     */
    bool hasFlow();

    void setCode(string code);
    void setType(VertexType type);
    void setVisited(bool visited);
    void setPath(Edge *path);
    Edge * addEdge(Vertex *dest, double c, double f = 0);
    Edge * findEdge(Vertex *destVertex);
    bool removeEdge(string code);
};

/********************** Edge  ****************************/

class Edge {
private:
    Vertex *dest; // destination vertex
    double capacity; // edge capacity

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow; // for flow-related problems

public:
    Edge(Vertex *orig, Vertex *dest, double capacity);

    Vertex * getDest() const;
    double getCapacity() const;
    Vertex * getOrig() const;
    Edge *getReverse() const;
    double getFlow() const;

    void setReverse(Edge *reverse);
    void setFlow(double flow);
};

/********************** Graph  ****************************/

class Graph {
private:
    unordered_map<string, Vertex *> vertices;    // vertex set

public:

    /**
     * @brief Auxiliary function to find a vertex in the graph, given its content.
     * @details Time complexity: O(1).
     * @param code The content of the vertex to be found.
     * @return A pointer to the vertex if found, nullptr otherwise.
     */
    Vertex *findVertex(const string &code) const;

    /**
     *  @brief Adds a vertex with a given content or info (in) to a graph (this).
     *  @details Time complexity: O(1).
     *  @param code Content of the vertex.
     *  @param type Type of the vertex (e.g. WaterReservoir).
     *  @return true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const string &code, const VertexType &type);

    /**
     * @brief Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge capacity (c).
     * @details Time complexity: O(1).
     * @param sourc The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param c The capacity of the edge.
     * @param f The flow of the edge. Default is 0.
     * @return true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const string &sourc, const string &dest, double c, double f = 0);

    /**
     * @brief Removes an edge from the graph, given the source and destination vertices.
     * @details Time complexity: O(1).
     * @param source The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @return true if the edge was successfully removed, false otherwise.
     */
    bool removeEdge(const string &source, const string &dest);

    /**
     * @brief Adds an bidirectional edge to the graph, given the source and destination
     * vertices and the edge capacity (c).
     * @details Time complexity: O(1).
     * @param sourc The source vertex of the edge.
     * @param dest The destination vertex of the edge.
     * @param c The capacity of the edge.
     * @param flow The flow of the edge. Default is 0.
     * @param reverseFlow The flow of the reverse edge. Default is 0.
     * @return true if the edge was successfully added, false otherwise.
     */
    bool addBidirectionalEdge(const string &sourc, const string &dest, double c, double flow = 0, double reverseFlow = 0);

    unordered_map<string, Vertex *> getVertexSet() const;

    /**
     * @brief Computes the maximum flow in the graph using the Edmonds-Karp algorithm.
     * @details Time complexity: O(V E^2).
     * @param waterReservoirs unordered_map containing water reservoirs.
     * @param deliverySites unordered_map containing delivery sites.
     */
    void maxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Creates a main source vertex and connects it to water reservoirs.
     * @details Time complexity: O(n).
     * @param code Content of the main source vertex.
     * @param waterReservoirs unordered_map containing water reservoirs.
     */
    void createMainSource(const string &code, const unordered_map<string, WaterReservoir *> *waterReservoirs);

    /**
     * @brief Deletes the main source vertex and its connections to water reservoirs.
     * @details Time complexity: O(n).
     * @param code Content of the main source vertex.
     * @param waterReservoirs unordered_map containing water reservoirs.
     */
    void deleteMainSource(const string &code, const unordered_map<string, WaterReservoir *> *waterReservoirs);

    /**
     * @brief Creates a main target vertex and its connections to delivery sites.
     * @details Time complexity: O(n).
     * @param code Content of the main target vertex.
     * @param deliverySites unordered_map containing delivery sites.
     */
    void createMainTarget(const string &code, const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Deletes the main target vertex and its connections to delivery sites.
     * @details Time complexity: O(n).
     * @param code Content of the main target vertex.
     * @param deliverySites unordered_map containing delivery sites.
     */
    void deleteMainTarget(const string &code, const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Calculates various metrics for the graph
     * @details This function calculates metrics including absolute and relative average, variance, standard
     * deviation and maximum difference in capacity and flow for edges in the graph.
     * @details Time complexity: O(V * E).
     * @param deliverySites unordered_map containing delivery sites.
     * @return an 'GraphMetrics' object containing all calculated metrics.
     */
    GraphMetrics calculateMetrics(const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Calculates total demand and maximum flow in the graph.
     * @details Time complexity: O(m), 'm' is the number of delivery sites.
     * @param deliverySites unordered_map containing delivery sites.
     * @return a pair containing the total demand and the maximum flow.
     */
    pair<double, double> getTotalDemandAndMaxFlow(const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Optimizes the load distribution in the graph.
     * @details Time complexity: O(E * (P + Q)), 'P' is the number of paths and 'Q' is the
     * maximum number of edges in a path.
     * @param deliverySites unordered_map containing delivery sites.
     */
    void optimizeLoad(const unordered_map<string, DeliverySite *> *deliverySites);

    /**
     * @brief Finds all paths between two vertices of the graph.
     * @details Time complexity: O(V + P), 'P' is the number of paths.
     * @param sourc Content of the source vertex.
     * @param dest Content of the destination vertex.
     * @return Vector of vectors containing all found paths.
     */
    vector<vector<Edge *>> getPaths(const string sourc, const string dest);

    /**
     * @brief Performs depth-first search (DFS) to find all paths from a source to a destination vertex.
     * @details Time complexity: O(V + E)
     * @param current Content of the current vertex.
     * @param dest Content of the destination vertex.
     * @param path Vector containing the current path being explored.
     * @param paths Vector of vectors to store all found paths.
     */
    void dfs(const string current, const string dest, vector<Edge *> &path, vector<vector<Edge *>> &paths);

    /**
     * @brief Marks a water reservoir as out of commission and adjusts the flow in the graph.
     * @details Time complexity: O(V E^2).
     * @param code The code of the reservoir.
     */
    void reservoirOutOfCommission(string const *code);

    /**
     * @brief Deactivates a vertex and adjusts the flow in the graph.
     * @details Time complexity: O(V + E).
     * @param v Vertex to be deactivated.
     * @param mainSourceCode Content of the main source vertex.
     * @param mainTargetCode Content of the main target vertex.
     */
    void deactivateVertex(Vertex *v, const string mainSourceCode, const string mainTargetCode);

    /**
     * @brief Sets flow on all edges in the graph to the specified value.
     * @details Time complexity: O(V + E).
     * @param f The flow value to be set on all edges.
     */
    void setAllEdgesFlow(double f);

    /**
     * @brief Sets flow on all vertices in the graph to the specified value.
     * @details Time complexity: O(V).
     * @param f The flow value to be set on all vertices.
     */
    void setAllVerticesFlow(double f);

    /**
     * @brief Updates flow on all vertices in the graph.
     * @details Time complexity: O(V).
     */
    void updateAllVerticesFlow();

    /**
     * @brief Marks a pumping station as out of commission and adjusts the flow in the graph.
     * @details Time complexity: O(V E^2).
     * @param waterReservoirs unordered_map containing water reservoirs.
     * @param deliverySites unordered_map containing delivery sites.
     * @param code The code of the pumping station.
     */
    void pumpingStationOutOfCommission(string const *code);

    /**
     * @brief Detects and deactivates flow cycles containing the specified vertex.
     * @details Time complexity: O(V + E)
     * @param deactivatedVertex The vertex to be deactivated.
     * @return true if a flow cycle is found and deactivated, false otherwise.
     */
    bool detectAndDeactivateFlowCycles(Vertex *deactivatedVertex);

    /**
     * @brief Finds and deactivates a flow path passing through the specified deactivated vertex.
     * @details Time complexity: O(V + E).
     * @param deactivatedVertex The vertex to be deactivated.
     * @param mainSourceCode Content of the main source vertex.
     * @param mainTargetCode Content of the main target vertex.
     */
    void findAndDeactivateFlowPath(Vertex *deactivatedVertex, const string mainSourceCode, const string mainTargetCode);

    /**
     * @brief Creates a deep copy of the graph.
     * @details Time complexity: O(V + E).
     * @return A pointer to the created copy of the graph.
     */
    Graph *copyGraph();

    /**
     * @brief Marks a pipeline as out of commission and adjusts the flow in the graph.
     * @details Time complexity: O(V E^2).
     * @param waterReservoirs unordered_map containing water reservoirs.
     * @param deliverySites unordered_map containing delivery sites.
     * @param servicePointA Code of the first service point of the pipeline to be marked as out of commission.
     * @param servicePointB Code of the second service point of the pipeline to be marked as out of commission.
     * @param unidirectional A flag indicating if the pipeline is unidirectional.
     */
    void pipelineOutOfCommission(string const *servicePointA, string const *servicePointB, bool unidirectional);
};

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H
