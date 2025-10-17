#ifndef MEASUREMENTS_H
#define MEASUREMENTS_H

#include <Arduino.h>

namespace control {

class Measures {
private:
  float airTemperature;
  float airRelativeHumidity;

  float soilMoisture;

  float tankLevel;

public:
  Measures(float airTemperature, float airRelativeHumidity, float soilMoisture,
           float tankLevel);

  ~Measures();

  float GetAirTemperature();

  float GetAirRelativeHumidity();

  float GetSoilMoisture();

  float GetTankLevel();
};

} // namespace control

#endif // MEASUREMENTS_H