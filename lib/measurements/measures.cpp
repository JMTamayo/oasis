#include <Arduino.h>

#include "measures.h"

namespace measurements {

Measures::Measures(float airTemperature, float airRelativeHumidity,
                   bool pumpState)
    : airTemperature(airTemperature), airRelativeHumidity(airRelativeHumidity),
      pumpState(pumpState) {}

Measures::~Measures() {}

float Measures::GetAirTemperature() { return this->airTemperature; }

float Measures::GetAirRelativeHumidity() { return this->airRelativeHumidity; }

bool Measures::GetPumpState() { return this->pumpState; }

} // namespace measurements