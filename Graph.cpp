#include <valarray>
#include "Graph.h"
#include "Algorithms.h"

/************************* Vertex  **************************/

void Vertex::deleteEdge(Edge *edge) {
    Vertex *dest = edge->getDest();
    // Remove the corresponding edge from the incoming list
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getCode() == this->code) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

Vertex::Vertex(string code, VertexType type) : code(code), type(type) {}

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

void Vertex::setFlow(double value) {
    this->flow = value;
}

void Vertex::setCode(string code) {
    this->code = code;
}

void Vertex::setType(VertexType type) {
    this->type = type;
}

void Vertex::setVisited(bool visited) {
    this->visited = visited;
}

void Vertex::setPath(Edge *path) {
    this->path = path;
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

bool Vertex::removeEdge(string code) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        Vertex *dest = edge->getDest();
        if (dest->getCode() == code) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true; // allows for multiple edges to connect the same pair of vertices (multigraph)
        }
        else {
            it++;
        }
    }
    return removedEdge;
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

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

void Edge::setFlow(double flow) {
    this->flow = flow;
}


/********************** Graph  ****************************/

Vertex *Graph::findVertex(const string &code) const {
    auto it = this->vertices.find(code);
    if (it != this->vertices.end()) {
        return it->second;
    }
    return nullptr;
}

bool Graph::addVertex(const string &code, const VertexType &type) {
    if(findVertex(code) == nullptr) {
        Vertex *newVertex = new Vertex(code, type);
        this->vertices.insert({code, newVertex});
        return true;
    }
    return false;
}

bool Graph::addEdge(const string &sourc, const string &dest, double c, double f) {
    Vertex *originVertex = findVertex(sourc);
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

bool Graph::removeEdge(const string &source, const string &dest) {
    Vertex * srcVertex = findVertex(source);
    if (srcVertex == nullptr) {
        return false;
    }
    return srcVertex->removeEdge(dest);
}

bool Graph::addBidirectionalEdge(const string &sourc, const string &dest, double c, double flow, double reverseFlow) {
    auto v1 = findVertex(sourc);
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

void Graph::createMainSource(const string &code, const unordered_map<string, WaterReservoir *> *waterReservoirs) {
    this->addVertex(code, VertexType::MainSource);

    for (auto& pair : *waterReservoirs) {
        string wrCode = pair.first;
        WaterReservoir* wr = pair.second;
        double maxDelivery = wr->getMaxDelivery();

        auto it = this->findVertex(wrCode);
        double f = (*it).getFlow();

        this->addEdge(code, wrCode, maxDelivery, f);

    }
}

void Graph::deleteMainSource(const string &code, const unordered_map<string, WaterReservoir *> *waterReservoirs) {
    for (auto& pair : *waterReservoirs) {
        string wrCode = pair.first;
        this->removeEdge(code, wrCode);
    }

    auto it = vertices.find(code);
    this->vertices.erase(it);
}

void Graph::createMainTarget(const string &code, const unordered_map<string, DeliverySite *> *deliverySites) {
    this->addVertex(code, VertexType::MainTarget);

    for (auto& pair : *deliverySites) {
        string dsCode = pair.first;
        DeliverySite* ds = pair.second;
        double demand = ds->getDemand();

        auto it = this->findVertex(dsCode);
        double f = (*it).getFlow();

        this->addEdge(dsCode, code, demand, f);
    }
}

void Graph::deleteMainTarget(const string &code, const unordered_map<string, DeliverySite *> *deliverySites) {
    for (auto& pair : *deliverySites) {
        string dsCode = pair.first;
        this->removeEdge(dsCode, code);
    }

    auto it = vertices.find(code);
    this->vertices.erase(it);
}

void Graph::maxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites) {
    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

    createMainSource(mainSourceCode, waterReservoirs);
    createMainTarget(mainTargetCode, deliverySites);

    this->setAllEdgesFlow(0);
    this->setAllVerticesFlow(0);

    edmondsKarp(this, mainSourceCode, mainTargetCode);
}

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

    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

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
pair<double, double> Graph::getTotalDemandAndMaxFlow(const unordered_map<string, DeliverySite *> *deliverySites) {
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

            for(vector<Edge *> p : paths) {
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

void Graph::dfs(const string current, const string dest, vector<Edge *> &path, vector<vector<Edge *>> &paths) {
    Vertex *sourc = findVertex(current);
    sourc->setVisited(true);

    if(current == dest) {
        paths.push_back(path);
    }
    else {
        for(auto e : sourc->getAdj()) {
            Vertex *u = e->getDest();
            if(!u->isVisited() && e->getCapacity() - e->getFlow() > 0) {
                path.push_back(e);
                dfs(u->getCode(), dest, path, paths);
                path.pop_back();
            }
        }
    }
}

vector<vector<Edge *>> Graph::getPaths(const string sourc, const string dest) {
    vector<vector<Edge *>> paths;
    vector<Edge *> path;

    for(auto pair : vertices) {
        Vertex *v = pair.second;
        v->setVisited(false);
    }

    dfs(sourc, dest, path, paths);


    return paths;
}

void Graph::reservoirOutOfCommission(string const *code) {
    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

    edmondsKarp(this, mainSourceCode, mainTargetCode);

    Vertex *wr = findVertex(*code);

    this->deactivateVertex(wr, mainSourceCode, mainTargetCode);

    this->updateAllVerticesFlow();
}

bool Vertex::hasFlow() {
    double inFlow = 0;
    double outFlow = 0;

    for (auto e: this->getIncoming()) inFlow += e->getFlow();
    for (auto e: this->getAdj()) outFlow += e->getFlow();

    if((inFlow > 0) || (outFlow > 0)) return true;
    return false;
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

void Graph::findAndDeactivateFlowPath(Vertex *deactivatedVertex, const string mainSourceCode, const string mainTargetCode) {
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

void Graph::deactivateVertex(Vertex *deactivatedVertex, const string mainSourceCode, const string mainTargetCode) {
    while(deactivatedVertex->hasFlow()) {
        // Check for flow cycles
        if(detectAndDeactivateFlowCycles(deactivatedVertex)) continue;
        // Find a path between the Master Source and the Master Target that passes through the Deactivated Vertex
        findAndDeactivateFlowPath(deactivatedVertex, mainSourceCode, mainTargetCode);
    }
}

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
        v->setFlow(0);
    }
}

void Graph::updateAllVerticesFlow() {
    for (auto pair : vertices) {
        Vertex *v = pair.second;
        v->updateFlow();
    }
}

void Vertex::updateFlow() {
    double incomingFlow = 0;
    if(this->incoming.size() == 0) {
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

void Graph::pumpingStationOutOfCommission(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites, string const *code) {
    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

    edmondsKarp(this, mainSourceCode, mainTargetCode);

    Vertex *ps = findVertex(*code);

    this->deactivateVertex(ps, mainSourceCode, mainTargetCode);

    edmondsKarpWithDeactivatedVertex(this, mainSourceCode, mainTargetCode, *code);

    this->updateAllVerticesFlow();
}

Graph *Graph::copyGraph() {
    Graph *newGraph = new Graph();

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

void Graph::pipelineOutOfCommission(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites, string const *servicePointA, string const *servicePointB, bool unidirectional) {
    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

    edmondsKarp(this, mainSourceCode, mainTargetCode);

    Vertex *origin = findVertex(*servicePointA);
    Vertex *dest = findVertex(*servicePointB);

    this->deactivateVertex(origin, mainSourceCode, mainTargetCode);

    if(!unidirectional)
        this->deactivateVertex(dest, mainSourceCode, mainTargetCode);

    edmondsKarpWithDeactivatedEdge(this, mainSourceCode, mainTargetCode, *servicePointA, *servicePointB, unidirectional);

    this->updateAllVerticesFlow();
}