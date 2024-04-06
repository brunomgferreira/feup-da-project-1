#include "Pipe.h"

Pipe::Pipe(string servicePointA, string servicePointB, double capacity, bool unidirectional)
    : servicePointA(std::move(servicePointA)), servicePointB(std::move(servicePointB)), capacity(capacity), unidirectional(unidirectional) {}

string Pipe::getServicePointA() {
    return this->servicePointA;
}

string Pipe::getServicePointB() {
    return this->servicePointB;
}

bool Pipe::getUnidirectional() const {
    return this->unidirectional;
}

double Pipe::getCapacity() const {
    return this->capacity;
}
