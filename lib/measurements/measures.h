#ifndef MEASURES_H
#define MEASURES_H

#include <Arduino.h>

namespace measurements {

class Measures {
private:
  float airTemperature;
  float airHumidity;

public:
  Measures(float airTemperature, float airHumidity);

  ~Measures();

  float GetAirTemperature();

  float GetAirHumidity();
};

} // namespace measurements

#endif // MEASURES_H