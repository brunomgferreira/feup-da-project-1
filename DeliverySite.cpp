#include "DeliverySite.h"

DeliverySite::DeliverySite(string city, double id, string code, double demand, double population)
    : city(std::move(city)), id(id), code(std::move(code)), demand(demand), population(population) {}

string DeliverySite::getCity() {
    return city;
}

double DeliverySite::getDemand() const {
    return demand;
}