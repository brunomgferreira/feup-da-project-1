#include "PumpingStation.h"

PumpingStation::PumpingStation(double id, string code) : id(id), code(std::move(code)) {}
