#include "DeliverySite.h"

DeliverySite::DeliverySite(string city, unsigned long id, string code, unsigned long demand, unsigned long population)
    : city(city), id(id), code(code), demand(demand), population(population) {}

string DeliverySite::getCity() {
    return city;
}

unsigned long DeliverySite::getDemand() {
    return demand;
}