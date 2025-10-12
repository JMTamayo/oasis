#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <Arduino.h>

namespace control {

class Measures {
private:
  float airTemperature;
  float airRelativeHumidity;

  float soilMoisture;
  String soilMoistureLevel;

  float tankLevel;

public:
  Measures(float airTemperature, float airRelativeHumidity, float soilMoisture,
           String soilMoistureLevel, float tankLevel);

  ~Measures();

  float GetAirTemperature();

  float GetAirRelativeHumidity();

  float GetSoilMoisture();

  String GetSoilMoistureLevel();

  float GetTankLevel();
};

} // namespace control

#endif // MEASUREMENTS_H