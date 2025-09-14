#ifndef MEASURES_H
#define MEASURES_H

#include <Arduino.h>

namespace measurements {

class Measures {
private:
  float airTemperature;
  float airRelativeHumidity;

public:
  Measures(float airTemperature, float airRelativeHumidity);

  ~Measures();

  float GetAirTemperature();

  float GetAirRelativeHumidity();
};

} // namespace measurements

#endif // MEASURES_H