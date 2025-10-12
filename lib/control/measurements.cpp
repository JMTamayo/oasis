#include "measurements.h"

namespace control {

Measures::Measures(float airTemperature, float airRelativeHumidity,
                   float soilMoisture, String soilMoistureLevel,
                   float tankLevel)
    : airTemperature(airTemperature), airRelativeHumidity(airRelativeHumidity),
      soilMoisture(soilMoisture), soilMoistureLevel(soilMoistureLevel),
      tankLevel(tankLevel) {}

Measures::~Measures() {}

float Measures::GetAirTemperature() { return this->airTemperature; }

float Measures::GetAirRelativeHumidity() { return this->airRelativeHumidity; }

float Measures::GetSoilMoisture() { return this->soilMoisture; }

String Measures::GetSoilMoistureLevel() { return this->soilMoistureLevel; }

float Measures::GetTankLevel() { return this->tankLevel; }
} // namespace control