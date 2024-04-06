#include "WaterReservoir.h"

WaterReservoir::WaterReservoir(string name, string municipality, double id, string code, double maxDelivery)
    : name(std::move(name)), municipality(std::move(municipality)), id(id), code(std::move(code)), maxDelivery(maxDelivery) {}

double WaterReservoir::getMaxDelivery() {
    return this->maxDelivery;
}

string WaterReservoir::getName() {
    return this->name;
}