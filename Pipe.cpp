#include "Pipe.h"

Pipe::Pipe(string servicePointA, string servicePointB, double capacity, bool unidirectional)
    : servicePointA(servicePointA), servicePointB(servicePointB), capacity(capacity), unidirectional(unidirectional) {}

string Pipe::getServicePointA() {
    return this->servicePointA;
}

string Pipe::getServicePointB() {
    return this->servicePointB;
}

bool Pipe::getUnidirectional() {
    return this->unidirectional;
}
