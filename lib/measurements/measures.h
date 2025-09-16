#ifndef MEASURES_H
#define MEASURES_H

#include <Arduino.h>

namespace measurements {

class Measures {
private:
  float airTemperature;
  float airRelativeHumidity;

  float waterFlowRate;
  float waterVolume;

  bool pumpState;

public:
  Measures(float airTemperature, float airRelativeHumidity, float waterFlowRate,
           float waterVolume, bool pumpState);

  ~Measures();

  float GetAirTemperature();

  float GetAirRelativeHumidity();

  float GetWaterFlowRate();

  float GetWaterVolume();

  bool GetPumpState();
};

} // namespace measurements

#endif // MEASURES_H