#include "WaterReservoir.h"

WaterReservoir::WaterReservoir(string name, string municipality, double id, string code, double maxDelivery)
    : name(name), municipality(municipality), id(id), code(code), maxDelivery(maxDelivery) {}

double WaterReservoir::getMaxDelivery() {
    return this->maxDelivery;
}