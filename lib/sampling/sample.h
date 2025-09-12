#ifndef SAMPLE_H
#define SAMPLE_H

#include <Arduino.h>

namespace sampling {

class Sample {
private:
  float airTemperature;
  float airHumidity;

public:
  Sample(float airTemperature, float airHumidity);

  ~Sample();

  float GetAirTemperature() const;

  float GetAirHumidity() const;
};

} // namespace sampling

#endif // SAMPLE_H