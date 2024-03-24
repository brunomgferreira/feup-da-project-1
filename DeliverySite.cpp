#include "DeliverySite.h"

DeliverySite::DeliverySite(string city, double id, string code, double demand, double population)
    : city(city), id(id), code(code), demand(demand), population(population) {}

string DeliverySite::getCity() {
    return city;
}

double DeliverySite::getDemand() {
    return demand;
}