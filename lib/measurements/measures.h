#ifndef MEASURES_H
#define MEASURES_H

#include <Arduino.h>

namespace measurements {

class Measures {
private:
  float airTemperature;
  float airRelativeHumidity;

  bool pumpState;

public:
  Measures(float airTemperature, float airRelativeHumidity, bool pumpState);

  ~Measures();

  float GetAirTemperature();

  float GetAirRelativeHumidity();

  bool GetPumpState();
};

} // namespace measurements

#endif // MEASURES_H