#include "measurements.h"

namespace control {

Measures::Measures(float airTemperature, float airRelativeHumidity,
                   float soilMoisture, float tankLevel)
    : airTemperature(airTemperature), airRelativeHumidity(airRelativeHumidity),
      soilMoisture(soilMoisture), tankLevel(tankLevel) {}

Measures::~Measures() {}

float Measures::GetAirTemperature() { return this->airTemperature; }

float Measures::GetAirRelativeHumidity() { return this->airRelativeHumidity; }

float Measures::GetSoilMoisture() { return this->soilMoisture; }

float Measures::GetTankLevel() { return this->tankLevel; }
} // namespace control