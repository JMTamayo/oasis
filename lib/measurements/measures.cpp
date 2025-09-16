#include <Arduino.h>

#include "measures.h"

namespace measurements {

Measures::Measures(float airTemperature, float airRelativeHumidity,
                   float waterFlowRate, float waterVolume, bool pumpState)
    : airTemperature(airTemperature), airRelativeHumidity(airRelativeHumidity),
      waterFlowRate(waterFlowRate), waterVolume(waterVolume),
      pumpState(pumpState) {}

Measures::~Measures() {}

float Measures::GetAirTemperature() { return this->airTemperature; }

float Measures::GetAirRelativeHumidity() { return this->airRelativeHumidity; }

float Measures::GetWaterFlowRate() { return this->waterFlowRate; }

float Measures::GetWaterVolume() { return this->waterVolume; }

bool Measures::GetPumpState() { return this->pumpState; }

} // namespace measurements