#include <Arduino.h>

#include "measurement.h"

namespace measuring {

Measure::Measure(float airTemperature, float airHumidity)
    : airTemperature(airTemperature), airHumidity(airHumidity) {}

Measure::~Measure() {}

float Measure::GetAirTemperature() const { return this->airTemperature; }

float Measure::GetAirHumidity() const { return this->airHumidity; }

} // namespace measuring