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

bool Vertex::isProcessing() const {
    return this->processing;
}

unsigned int Vertex::getIndegree() const {
    return this->indegree;
}

double Vertex::getDist() const {
    return this->dist;
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

void Vertex::setProcesssing(bool processing) {
    this->processing = processing;
}

void Vertex::setIndegree(unsigned int indegree) {
    this->indegree = indegree;
}

void Vertex::setDist(double dist) {
    this->dist = dist;
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

void Vertex::removeOutgoingEdges() {
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge *edge = *it;
        it = adj.erase(it);
        deleteEdge(edge);
    }
}


/********************** Edge  ****************************/

Edge::Edge(Vertex *orig, Vertex *dest, double capacity) : orig(orig), dest(dest), capacity(capacity) {}

Vertex * Edge::getDest() const {
    return this->dest;
}

double Edge::getCapacity() const {
    return this->capacity;
}

bool Edge::isSelected() const {
    return this->selected;
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

void Edge::setSelected(bool selected) {
    this->selected = selected;
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
        originVertex->addEdge(destVertex, c, f);
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

bool Graph::addBidirectionalEdge(const string &sourc, const string &dest, double c) {
    auto v1 = findVertex(sourc);
    auto v2 = findVertex(dest);
    if (v1 == nullptr || v2 == nullptr)
        return false;
    auto e1 = v1->addEdge(v2, c);
    auto e2 = v2->addEdge(v1, c);
    e1->setReverse(e2);
    e2->setReverse(e1);
    return true;
}

int Graph::getNumVertex() const {
    return this->vertices.size();
}

unordered_map<string, Vertex *> Graph::getVertexSet() const {
    return this->vertices;
}

// TODO - Dont know if this methods are needed
vector<string> Graph::dfs() const {
    vector<string> res;
    return res;
}

// TODO - Dont know if this methods are needed
vector<string> Graph::dfs(const string & source) const {
    vector<string> res;
    return res;
}

// TODO - Dont know if this methods are needed
void Graph::dfsVisit(Vertex *v,  vector<string> & res) const {}

// TODO - Dont know if this methods are needed
vector<string> Graph::bfs(const string & source) const {
    vector<string> res;
    return res;
}

// TODO - Dont know if this methods are needed
bool Graph::isDAG() const {
    return false;
}

// TODO - Dont know if this methods are needed
bool Graph::dfsIsDAG(Vertex *v) const {
    return false;
}

// TODO - Dont know if this methods are needed
vector<string> Graph::topsort() const {
    vector<string> res;
    return res;
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

    deleteMainSource(mainSourceCode, waterReservoirs);
    deleteMainTarget(mainTargetCode, deliverySites);
}

void Graph::calculateMetrics(double &absoluteAverage, double &absoluteVariance, double &absoluteMaxDifference, double &relativeAverage, double &relativeVariance, double &relativeMaxDifference) {

    int numberOfPipes = 0;

    double absoluteSum = 0;
    double relativeSum = 0;

    // Determine average
    for(auto &pair : vertices) {
        const Vertex *v = pair.second;

        for(auto e : v->getAdj()) {
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
}

void Graph::optimizedMaxFlow(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites) {

    this->setAllEdgesFlow(0);
    this->setAllVerticesFlow(0);

    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

    createMainSource(mainSourceCode, waterReservoirs);
    createMainTarget(mainTargetCode, deliverySites);

    double biggestCapacity = -INF;
    double smallestCapacity = INF;

    this->setAllEdgesFlow(0);
    this->setAllVerticesFlow(0);

    for(auto &pair : vertices) {
        Vertex *v = pair.second;
        for(auto e : v->getAdj()) {
            double capacity = e->getCapacity();
            if(capacity < smallestCapacity) smallestCapacity = capacity;
            if(capacity > biggestCapacity) biggestCapacity = capacity;
        }
    }

    double c = 0;

    optimizedEdmondsKarp(this, mainSourceCode, mainTargetCode, biggestCapacity, smallestCapacity, &c);

    this->updateAllVerticesFlow();

    deleteMainSource(mainSourceCode, waterReservoirs);
    deleteMainTarget(mainTargetCode, deliverySites);
}

void Graph::reservoirOutOfCommission(const unordered_map<string, WaterReservoir *> *waterReservoirs, const unordered_map<string, DeliverySite *> *deliverySites, string const *code) {
    string mainSourceCode = "mainSource";
    string mainTargetCode = "mainTarget";

    createMainSource(mainSourceCode, waterReservoirs);
    createMainTarget(mainTargetCode, deliverySites);

    edmondsKarp(this, mainSourceCode, mainTargetCode);

    Vertex *wr = findVertex(*code);

    while (wr->hasFlow()) {
        this->deactivateVertex(wr, mainSourceCode, mainTargetCode);
    }

    this->updateAllVerticesFlow();

    cout << findVertex(*code)->getFlow() << endl;

    deleteMainSource(mainSourceCode, waterReservoirs);
    deleteMainTarget(mainTargetCode, deliverySites);

}

bool Vertex::hasFlow() {
    double inFlow = 0;
    double outFlow = 0;

    for (auto e: this->getIncoming()) inFlow += e->getFlow();
    for (auto e: this->getAdj()) outFlow += e->getFlow();

    if((inFlow > 0) || (outFlow > 0)) return true;
    return false;
}


void Graph::deactivateVertex(Vertex *deactivatedVertex, const string mainSourceCode, const string mainTargetCode) {
    Vertex *mainTarget = findVertex(mainTargetCode);

    for(auto pair : vertices) {
        Vertex *v = pair.second;
        v->setVisited(false);
    }

    queue<Vertex *> q;
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

    q.push(deactivatedVertex);
    deactivatedVertex->setVisited(true);

    while (!q.empty()) {
        Vertex *u = q.front();
        q.pop();
        for (Edge *e: u->getIncoming()) {
            Vertex *w = e->getOrig();
            if (!w->isVisited() && e->getFlow() > 0) {
                q.push(w);
                w->setVisited(true);
                u->setPath(e);
            }
        }
    }

    double f = INF;
    // Traverse the augmenting path to find the minimum residual capacity
    for (Vertex *v = mainTarget; v->getCode() != mainSourceCode; ) {
        auto e = v->getPath();
        f = std::min(f, e->getFlow());
        v = e->getOrig();
    }

    // Traverse the augmenting path and update the flow values accordingly
    for (Vertex *v = mainTarget; v->getCode() != mainSourceCode; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
        e->setFlow(flow - f);
        v = e->getOrig();
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
    for (auto e: this->incoming) {
        incomingFlow += e->getFlow();
    }
    this->flow = incomingFlow;
}