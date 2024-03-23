#include "Algorithms.h"


// Function to test the given vertex 'w' and visit it if conditions are met
void testAndVisit(std::queue< Vertex*> &q, Edge *e, Vertex *w, unsigned long residual) {
    // Check if the vertex 'w' is not visited and there is residual capacity
    if (! w->isVisited() && residual > 0) {
        // Mark 'w' as visited, set the path through which it was reached, and enqueue it
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

// Function to find an augmenting path using Breadth-First Search
bool findAugmentingPath(Graph *g, Vertex *s, Vertex *t) {
    // Mark all vertices as not visited
    for(auto v : g->getVertexSet()) {
        v.second->setVisited(false);
    }
    // Mark the source vertex as visited and enqueue it
    s->setVisited(true);
    std::queue<Vertex *> q;
    q.push(s);
    // BFS to find an augmenting path
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
        // Process outgoing edges
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());
        }
        // Process incoming edges
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrig(), e->getFlow());
        }
    }
    // Return true if a path to the target is found, false otherwise
    return t->isVisited();
}

// Function to find the minimum residual capacity along the augmenting path
unsigned long findMinResidualAlongPath(Vertex *s, Vertex *t) {
    unsigned long f = INF;
    // Traverse the augmenting path to find the minimum residual capacity
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getCapacity() - e->getFlow());
            v = e->getOrig();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    // Return the minimum residual capacity
    return f;
}

// Function to augment flow along the augmenting path with the given flow value
void augmentFlowAlongPath(Vertex *s, Vertex *t, unsigned long f) {
    // Traverse the augmenting path and update the flow values accordingly
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        unsigned long flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrig();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

// Main function implementing the Edmonds-Karp algorithm
void edmondsKarp(Graph *g, string source, string target) {
    // Find source and target vertices in the graph
    Vertex* s = g->findVertex(source);
    Vertex* t = g->findVertex(target);
    // Validate source and target vertices
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");
    // Initialize flow on all edges to 0
    for (auto pair : g->getVertexSet()) {
        Vertex *v = pair.second;
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    // While there is an augmenting path, augment the flow along the path
    while( findAugmentingPath(g, s, t) ) {
        unsigned long f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }

    // Calculate and save incoming flow for each vertex
    for (auto pair : g->getVertexSet()) {
        Vertex *v = pair.second;
        unsigned long incomingFlow = 0;
        for (auto e: v->getIncoming()) {
            incomingFlow += e->getFlow();
        }
        v->setFlow(incomingFlow);
    }
}