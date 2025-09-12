#ifndef MEASUREMENT_H
#define MEASUREMENT_H

#include <Arduino.h>

namespace measuring {

class Measure {
private:
  float airTemperature;
  float airHumidity;

public:
  Measure(float airTemperature, float airHumidity);

  ~Measure();

  float GetAirTemperature() const;

  float GetAirHumidity() const;
};

} // namespace measuring

#endif // MEASUREMENT_H