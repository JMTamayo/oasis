#include <Arduino.h>

#include "sample.h"

namespace sampling {

Sample::Sample(float airTemperature, float airHumidity)
    : airTemperature(airTemperature), airHumidity(airHumidity) {}

Sample::~Sample() {}

float Sample::GetAirTemperature() const { return this->airTemperature; }

float Sample::GetAirHumidity() const { return this->airHumidity; }

} // namespace sampling