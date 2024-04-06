#include <utility>
#include <valarray>
#include "Graph.h"
#include "Algorithms.h"

/************************* Vertex  **************************/

Vertex::Vertex(string code, VertexType type) : code(std::move(code)), type(type) {}

bool Vertex::operator<(Vertex & vertex) const {
    return this->dist < vertex.dist;
}

string Vertex::getCode() const {
    return this->code;
}

VertexType Vertex::getType() const {
    return this->type;
}

vector<Edge *> Vertex::getAdj() const {
    return this->adj;
}

bool Vertex::isVisited() const {
    return this->visited;
}

Edge *Vertex::getPath() const {
    return this->path;
}

vector<Edge *> Vertex::getIncoming() const {
    return this->incoming;
}

double Vertex::getFlow() const {
    return this->flow;
}

bool Vertex::hasFlow() const {
    double inFlow = 0;
    double outFlow = 0;

    for (auto e: this->getIncoming()) inFlow += e->getFlow();
    for (auto e: this->getAdj()) outFlow += e->getFlow();

    if((inFlow > 0) || (outFlow > 0)) return true;
    return false;
}

void Vertex::setFlow(double value) {
    this->flow = value;
}

void Vertex::updateFlow() {
    double incomingFlow = 0;
    if(this->incoming.empty()) {
        for (auto e: this->adj) {
            incomingFlow += e->getFlow();
        }
    }
    else {
        for (auto e: this->incoming) {
            incomingFlow += e->getFlow();
        }
    }
    this->flow = incomingFlow;
}

void Vertex::setVisited(bool isVisited) {
    this->visited = isVisited;
}

void Vertex::setPath(Edge *newPath) {
    this->path = newPath;
}

Edge * Vertex::addEdge(Vertex *dest, double c, double f) {
    auto newEdge = new Edge(this, dest, c);
    newEdge->setFlow(f);
    adj.push_back(newEdge);
    dest->incoming.push_back(newEdge);
    return newEdge;
}

Edge * Vertex::findEdge(Vertex *destVertex) {
    for(auto e : adj) {
        Vertex *dest = e->getDest();
        if(dest->getCode() == destVertex->getCode()) {
            return e;
        }
    }
    return nullptr;
}

/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, double capacity) : orig(orig), dest(dest), capacity(capacity) {}

Vertex * Edge::getDest() const {
    return this->dest;
}

double Edge::getCapacity() const {
    return this->capacity;
}

Vertex * Edge::getOrig() const {
    return this->orig;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

double Edge::getFlow() const {
    return this->flow;
}

void Edge::setReverse(Edge *reverseEdge) {
    this->reverse = reverseEdge;
}

void Edge::setFlow(double newFlow) {
    this->flow = newFlow;
}


/********************** Graph  ****************************/

// Copy Graph

Graph *Graph::copyGraph() {
    auto *newGraph = new Graph();

    // Copy vertices
    for(auto &pair : vertices) {
        string code = pair.first;
        Vertex *v = pair.second;

        newGraph->addVertex(code, v->getType());
    }

    // Copy edges
    for(auto &pair : vertices) {
        string code = pair.first;
        Vertex *v = pair.second;

        for(auto e : v->getAdj()) {
            string origin = e->getOrig()->getCode();
            string dest = e->getDest()->getCode();
            double capacity = e->getCapacity();
            double flow = e->getFlow();

            newGraph->addEdge(origin, dest, capacity, flow);
        }
    }
    newGraph->updateAllVerticesFlow();
    return newGraph;
}

// Metrics

GraphMetrics Graph::calculateMetrics(const unordered_map<string, DeliverySite *> *deliverySites) {
    // Absolute metrics
    double absoluteAverage;
    double absoluteVariance;
    double absoluteStandardDeviation;
    double absoluteMaxDifference = 0;

    // Relative metrics
    double relativeAverage;
    double relativeVariance;
    double relativeStandardDeviation;
    double relativeMaxDifference = 0;

    // Aux variables
    int numberOfPipes = 0;
    double absoluteSum = 0;
    double relativeSum = 0;

    // Determine total demand and the max flow
    auto totalDemandAndMaxFlow = getTotalDemandAndMaxFlow(deliverySites);
    double totalDemand = totalDemandAndMaxFlow.first;
    double maxFlow = totalDemandAndMaxFlow.second;

    // Determine average
    for(auto &pair : vertices) {
        const Vertex *v = pair.second;

        for(auto e : v->getAdj()) {
            // Ignore auxiliary edges
            if(e->getOrig()->getCode() == mainSourceCode || e->getDest()->getCode() == mainTargetCode) continue;

            double absoluteDifference = e->getCapacity() - e->getFlow();
            double relativeDifference = ( e->getCapacity() - e->getFlow() ) / e->getCapacity();

            if(absoluteDifference > absoluteMaxDifference) absoluteMaxDifference = absoluteDifference;
            if(relativeDifference > relativeMaxDifference) relativeMaxDifference = relativeDifference;

            // Sum all differences
            absoluteSum += absoluteDifference;
            relativeSum += relativeDifference;

            numberOfPipes++;
        }
    }

    // Divide by the number of pipes and get the average of all differences
    absoluteAverage = absoluteSum / numberOfPipes;
    relativeAverage = relativeSum / numberOfPipes;

    absoluteSum = 0;
    relativeSum = 0;

    // Determine variance
    for(auto &pair : vertices) {
        const Vertex *v = pair.second;

        for(auto e : v->getAdj()) {
            double absoluteDifference = e->getCapacity() - e->getFlow();
            double relativeDifference = ( e->getCapacity() - e->getFlow() ) / e->getCapacity();

            // Sum all variance steps
            absoluteSum += pow(absoluteDifference - absoluteAverage, 2);
            relativeSum += pow(relativeDifference - relativeAverage, 2);
        }
    }

    // Divide by the number of pipes and get the variance
    absoluteVariance = absoluteSum / numberOfPipes;
    relativeVariance = relativeSum / numberOfPipes;

    // Sqrt the variance ang get the standard deviation
    absoluteStandardDeviation = sqrt(absoluteVariance);
    relativeStandardDeviation = sqrt(relativeVariance);

    GraphMetrics metrics(
            absoluteAverage,
            absoluteVariance,
            absoluteStandardDeviation,
            absoluteMaxDifference,
            relativeAverage,
            relativeVariance,
            relativeStandardDeviation,
            relativeMaxDifference,
            maxFlow,
            totalDemand);

    return metrics;
}

// Determine total demand and the max flow
pair<double, double> Graph::getTotalDemandAndMaxFlow(const unordered_map<string, DeliverySite *> *deliverySites) const {
    double maxFlow = 0;
    double totalDemand = 0;

    for(auto &pair : *deliverySites) {
        const string code = pair.first;
        double flow = this->findVertex(code)->getFlow();
        maxFlow += flow;
        totalDemand += pair.second->getDemand();
    }

    return {totalDemand, maxFlow};
}

string Graph::getMainSourceCode() {
    return mainSourceCode;
}

string Graph::getMainTargetCode() {
    return mainTargetCode;
}

Vertex *Graph::findVertex(const string &code) const {
    auto it = this->vertices.find(code);
    if (it != this->vertices.end()) {
        return it->second;
    }
    return nullptr;
}

bool Graph::addVertex(const string &code, const VertexType &type) {
    if(findVertex(code) == nullptr) {
        auto *newVertex = new Vertex(code, type);
        this->vertices.insert({code, newVertex});
        return true;
    }
    return false;
}

bool Graph::addEdge(const string &source, const string &dest, double c, double f) const {
    Vertex *originVertex = findVertex(source);
    Vertex *destVertex = findVertex(dest);

    if (originVertex && destVertex) {
        auto e1 = originVertex->addEdge(destVertex, c, f);
        auto e2 = destVertex->findEdge(originVertex);

        if(e2 != nullptr) {
            if(e1->getCapacity() == e2->getCapacity()) {
                e1->setReverse(e2);
                e2->setReverse(e1);
            }
        }
        return true;
    }

    return false;
}

bool Graph::addBidirectionalEdge(const string &source, const string &dest, double c, double flow, double reverseFlow) const {
    auto v1 = findVertex(source);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, c, flow);
    auto e2 = v2->addEdge(v1, c, reverseFlow);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

unordered_map<string, Vertex *> Graph::getVertexSet() const {
    return this->vertices;
}

// All Vertices/Edges Flow

void Graph::setAllEdgesFlow(double f) {
    for(auto &pair : vertices) {
        Vertex *v = pair.second;
        for(auto e : v->getAdj()) {
            e->setFlow(f);
        }
    }
}

void Graph::setAllVerticesFlow(double f) {
    for(auto &pair : vertices) {
        Vertex *v = pair.second;
        v->setFlow(f);
    }
}

void Graph::updateAllVerticesFlow() {
    for (auto &pair : vertices) {
        Vertex *v = pair.second;
        v->updateFlow();
    }
}

// Main Source & Main Target

void Graph::createMainSource(const unordered_map<string, WaterReservoir *> *waterReservoirs) {
    this->addVertex(mainSourceCode, VertexType::MainSource);

    for (auto& pair : *waterReservoirs) {
        string wrCode = pair.first;
        WaterReservoir* wr = pair.second;
        double maxDelivery = wr->getMaxDelivery();

        auto it = this->findVertex(wrCode);
        double f = (*it).getFlow();

        this->addEdge(mainSourceCode, wrCode, maxDelivery, f);

    }
}

void Graph::createMainTarget(const unordered_map<string, DeliverySite *> *deliverySites) {
    this->addVertex(mainTargetCode, VertexType::MainTarget);

    for (auto& pair : *deliverySites) {
        string dsCode = pair.first;
        DeliverySite* ds = pair.second;
        double demand = ds->getDemand();

        auto it = this->findVertex(dsCode);
        double f = (*it).getFlow();

        this->addEdge(dsCode, mainTargetCode, demand, f);
    }
}

// Max Flow

void Graph::maxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites) {
    createMainSource(waterReservoirs);
    createMainTarget(deliverySites);

    this->setAllEdgesFlow(0);
    this->setAllVerticesFlow(0);

    edmondsKarp(this);
}

// Load Optimization & Auxiliary Functions

void Graph::optimizeLoad(const unordered_map<string, DeliverySite *> *deliverySites) {
    GraphMetrics initialMetrics = this->calculateMetrics(deliverySites);
    GraphMetrics finalMetrics = initialMetrics;

    vector<Edge *> edges;

    for (auto &pair : vertices) {
        Vertex *v = pair.second;
        for (Edge *e : v->getAdj()) {
            edges.push_back(e);
        }
    }

    int iterations = 0;

    do {
        sort(edges.begin(), edges.end(), [](Edge *a, Edge *b) {
            double differenceA = (a->getCapacity() - a->getFlow())/a->getCapacity();
            double differenceB = (b->getCapacity() - b->getFlow())/b->getCapacity();

            if(differenceA == differenceB) return a->getFlow() > b->getFlow();
            return differenceA < differenceB;
        });

        for(Edge *edge : edges) {
            vector<Edge *> path;
            vector<vector<Edge *>> paths;

            if(edge->getFlow() == 0) break;

            paths = this->getPaths(edge->getOrig()->getCode(), edge->getDest()->getCode());

            if(paths.empty()) continue;

            double maxDiff = -1;

            for(const vector<Edge *>& p : paths) {
                double minDiff = INF;
                for(Edge *e : p) {
                    minDiff = min(minDiff, e->getCapacity() - e->getFlow());
                }
                if(minDiff > maxDiff) {
                    maxDiff = minDiff;
                    path = p;
                }
            }

            if(edge->getFlow() < maxDiff) maxDiff = edge->getFlow();
            edge->setFlow(edge->getFlow() - maxDiff);
            for(auto e : path) e->setFlow(e->getFlow() + maxDiff);
        }

        initialMetrics = finalMetrics;
        finalMetrics = this->calculateMetrics(deliverySites);
        iterations++;
    } while((finalMetrics.getAbsoluteVariance() < initialMetrics.getAbsoluteVariance()
            || finalMetrics.getRelativeVariance() < initialMetrics.getRelativeVariance()
            || finalMetrics.getAbsoluteAverage() < initialMetrics.getAbsoluteAverage()
            || finalMetrics.getRelativeAverage() < initialMetrics.getRelativeAverage())
            && iterations < edges.size());
    this->updateAllVerticesFlow();
}

// Load Optimization Auxiliary Functions

vector<vector<Edge *>> Graph::getPaths(const string& source, const string &dest) {
    vector<vector<Edge *>> paths;
    vector<Edge *> path;

    for(auto &pair : vertices) {
        Vertex *v = pair.second;
        v->setVisited(false);
    }

    dfs(source, dest, path, paths);

    return paths;
}

void Graph::dfs(const string &current, const string &dest, vector<Edge *> &path, vector<vector<Edge *>> &paths) {
    Vertex *source = findVertex(current);
    source->setVisited(true);

    if(current == dest) {
        paths.push_back(path);
    }
    else {
        for(auto e : source->getAdj()) {
            Vertex *u = e->getDest();
            if(!u->isVisited() && e->getCapacity() - e->getFlow() > 0) {
                path.push_back(e);
                dfs(u->getCode(), dest, path, paths);
                path.pop_back();
            }
        }
    }
}

// Out of Commission Functions

void Graph::stationOutOfCommission(string const *code) {
    edmondsKarp(this);

    Vertex *ps = findVertex(*code);

    this->deactivateVertex(ps);

    edmondsKarpWithDeactivatedVertex(this, *code);

    this->updateAllVerticesFlow();
}

void Graph::pipelineOutOfCommission(string const *servicePointA, string const *servicePointB, bool unidirectional) {
    edmondsKarp(this);

    Vertex *origin = findVertex(*servicePointA);
    Vertex *dest = findVertex(*servicePointB);

    this->deactivateVertex(origin);

    if(!unidirectional)
        this->deactivateVertex(dest);

    edmondsKarpWithDeactivatedEdge(this, *servicePointA, *servicePointB, unidirectional);

    this->updateAllVerticesFlow();
}

// Deactivate Vertex & Auxiliary Functions

void Graph::deactivateVertex(Vertex *deactivatedVertex) {
    while(deactivatedVertex->hasFlow()) {
        // Check for flow cycles
        if(detectAndDeactivateFlowCycles(deactivatedVertex)) continue;
        // Find a path between the Master Source and the Master Target that passes through the Deactivated Vertex
        findAndDeactivateFlowPath(deactivatedVertex);
    }
}

bool Graph::detectAndDeactivateFlowCycles(Vertex *deactivatedVertex) {
    // Initialize the vertices
    for(auto const &pair : vertices) {
        Vertex *v = pair.second;
        v->setVisited(false);
        v->setPath(nullptr);
    }

    queue<Vertex *> q;
    q.push(deactivatedVertex);
    deactivatedVertex->setVisited(true);
    bool cycleFound = false;

    while (!q.empty()) {

        if(cycleFound) break;

        Vertex *u = q.front();
        q.pop();

        for (Edge *e: u->getAdj()) {
            Vertex *w = e->getDest();
            if (w->getCode() == deactivatedVertex->getCode() && e->getFlow() > 0) {
                w->setPath(e);
                cycleFound = true;
                break;
            }
            if (!w->isVisited() && e->getFlow() > 0) {
                q.push(w);
                w->setVisited(true);
                w->setPath(e);
            }
        }
    }

    // No cycles found
    if(!cycleFound) return false;

    double f = INF;

    // Traverse the path to find the minimum residual capacity
    Vertex *v = deactivatedVertex;
    while (true) {
        auto e = v->getPath();
        f = std::min(f, e->getFlow());
        v = e->getOrig();
        if(v->getCode() == deactivatedVertex->getCode())
            break;
    }

    // Traverse the path and update the flow values accordingly
    v = deactivatedVertex;
    while (true) {
        auto e = v->getPath();
        double flow = e->getFlow();
        e->setFlow(flow - f);
        v = e->getOrig();
        if(v->getCode() == deactivatedVertex->getCode())
            break;
    }

    // Found a cycle
    return true;
}

void Graph::findAndDeactivateFlowPath(Vertex *deactivatedVertex) {
    Vertex *mainSource = findVertex(mainSourceCode);
    Vertex *mainTarget = findVertex(mainTargetCode);

    // Initialize the vertices
    for(auto const &pair : vertices) {
        Vertex *v = pair.second;
        v->setVisited(false);
        v->setPath(nullptr);
    }

    queue<Vertex *> q;

    // Find a path from the deactivated vertex to the target
    q.push(deactivatedVertex);
    deactivatedVertex->setVisited(true);
    while (!q.empty()) {
        Vertex *u = q.front();
        q.pop();
        for (Edge *e: u->getAdj()) {
            Vertex *w = e->getDest();
            if (!w->isVisited() && e->getFlow() > 0) {
                q.push(w);
                w->setVisited(true);
                w->setPath(e);
            }
        }
    }

    // Find a path from the source to the deactivated vertex
    q.push(mainSource);
    deactivatedVertex->setVisited(false);
    mainSource->setVisited(true);
    while (!q.empty()) {
        Vertex *u = q.front();
        q.pop();
        for (Edge *e: u->getAdj()) {
            Vertex *w = e->getDest();
            if (!w->isVisited() && e->getFlow() > 0) {
                q.push(w);
                w->setVisited(true);
                w->setPath(e);
            }
        }
    }

    double f = INF;
    // Traverse the path to find the minimum residual capacity
    for (Vertex *v = mainTarget; v->getCode() != mainSourceCode; ) {
        auto e = v->getPath();
        if(v->getCode() != mainSourceCode && e == nullptr)
            break;
        f = std::min(f, e->getFlow());
        v = e->getOrig();
    }

    // Traverse the path and update the flow values accordingly
    for (Vertex *v = mainTarget; v->getCode() != mainSourceCode; ) {
        auto e = v->getPath();
        if(v->getCode() != mainSourceCode && e == nullptr)
            break;
        double flow = e->getFlow();
        e->setFlow(flow - f);
        v = e->getOrig();
    }
}
