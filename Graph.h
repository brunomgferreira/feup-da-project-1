#ifndef WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H
#define WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H


#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include "WaterReservoir.h"
#include "DeliverySite.h"

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
    bool processing = false; // used by isDAG (in addition to the visited attribute)
    unsigned int indegree; // used by topsort
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
    bool isProcessing() const;
    unsigned int getIndegree() const;
    double getDist() const;
    Edge *getPath() const;
    vector<Edge *> getIncoming() const;

    double getFlow() const;
    void setFlow(double value);
    void updateFlow();
    bool hasFlow();

    void setCode(string code);
    void setType(VertexType type);
    void setVisited(bool visited);
    void setProcesssing(bool processing);
    void setIndegree(unsigned int indegree);
    void setDist(double dist);
    void setPath(Edge *path);
    Edge * addEdge(Vertex *dest, double c, double f = 0);
    bool removeEdge(string code);
    void removeOutgoingEdges();
};

/********************** Edge  ****************************/

class Edge {
private:
    Vertex *dest; // destination vertex
    double capacity; // edge capacity

    // auxiliary fields
    bool selected = false;

    // used for bidirectional edges
    Vertex *orig;
    Edge *reverse = nullptr;

    double flow; // for flow-related problems

public:
    Edge(Vertex *orig, Vertex *dest, double capacity);

    Vertex * getDest() const;
    double getCapacity() const;
    bool isSelected() const;
    Vertex * getOrig() const;
    Edge *getReverse() const;
    double getFlow() const;

    void setSelected(bool selected);
    void setReverse(Edge *reverse);
    void setFlow(double flow);
};

/********************** Graph  ****************************/

class Graph {
private:
    unordered_map<string, Vertex *> vertices;    // vertex set

public:
    /*
    * Auxiliary function to find a vertex with a given the content.
    */
    Vertex *findVertex(const string &code) const;
    /*
     *  Adds a vertex with a given content or info (in) to a graph (this).
     *  Returns true if successful, and false if a vertex with that content already exists.
     */
    bool addVertex(const string &code, const VertexType &type);

    /*
     * Adds an edge to a graph (this), given the contents of the source and
     * destination vertices and the edge capacity (c).
     * Returns true if successful, and false if the source or destination vertex does not exist.
     */
    bool addEdge(const string &sourc, const string &dest, double c, double f = 0);
    bool removeEdge(const string &source, const string &dest);
    bool addBidirectionalEdge(const string &sourc, const string &dest, double c);

    int getNumVertex() const;
    unordered_map<string, Vertex *> getVertexSet() const;

    vector<string> dfs() const;
    vector<string> dfs(const string & source) const;
    void dfsVisit(Vertex *v,  vector<string> & res) const;
    vector<string> bfs(const string & source) const;

    bool isDAG() const;
    bool dfsIsDAG(Vertex *v) const;
    vector<string> topsort() const;

    void maxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites);

    void createMainSource(const string &code, const unordered_map<string, WaterReservoir *> *waterReservoirs);
    void deleteMainSource(const string &code, const unordered_map<string, WaterReservoir *> *waterReservoirs);

    void createMainTarget(const string &code, const unordered_map<string, DeliverySite *> *deliverySites);
    void deleteMainTarget(const string &code, const unordered_map<string, DeliverySite *> *deliverySites);

    void calculateMetrics(double &absoluteAverage, double &absoluteVariance, double &absoluteMaxDifference, double &relativeAverage, double &relativeVariance, double &relativeMaxDifference);

    void optimizedMaxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites);

    void reservoirOutOfCommission(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites, string const *code);

    void deactivateVertex(Vertex *v, const string mainSourceCode, const string mainTargetCode);

    void setAllEdgesFlow(double f);
    void setAllVerticesFlow(double f);

    void updateAllVerticesFlow();
};

#endif //WATER_SUPPLY_ANALYSIS_SYSTEM_GRAPH_H
