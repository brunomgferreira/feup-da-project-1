#include "Pipe.h"

Pipe::Pipe(string servicePointA, string servicePointB, double capacity, bool unidirectional)
    : servicePointA(servicePointA), servicePointB(servicePointB), capacity(capacity), unidirectional(unidirectional) {}