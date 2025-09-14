#include <Arduino.h>

#include "measures.h"

namespace measurements {

Measures::Measures(float airTemperature, float airRelativeHumidity)
    : airTemperature(airTemperature), airRelativeHumidity(airRelativeHumidity) {
}

Measures::~Measures() {}

float Measures::GetAirTemperature() { return this->airTemperature; }

float Measures::GetAirRelativeHumidity() { return this->airRelativeHumidity; }

} // namespace measurements