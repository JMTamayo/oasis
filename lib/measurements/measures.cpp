#include <Arduino.h>

#include "measures.h"

namespace measurements {

Measures::Measures(float airTemperature, float airHumidity)
    : airTemperature(airTemperature), airHumidity(airHumidity) {}

Measures::~Measures() {}

float Measures::GetAirTemperature() { return this->airTemperature; }

float Measures::GetAirHumidity() { return this->airHumidity; }

} // namespace measurements