#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

#include "dht11.h"
#include "sen0193.h"
#include "yf_s401.h"

namespace control {

class Measures {
private:
  peripherals::AirProperties airProperties;
  peripherals::FlowRate waterFlowRate;
  peripherals::SoilMoisture soilMoisture;
  bool pumpState;

public:
  Measures(peripherals::AirProperties airProperties,
           peripherals::FlowRate waterFlowRate,
           peripherals::SoilMoisture soilMoisture, bool pumpState);

  ~Measures();

  peripherals::AirProperties GetAirProperties();

  peripherals::FlowRate GetWaterFlowRate();

  peripherals::SoilMoisture GetSoilMoisture();

  bool GetPumpState();
};

class Controller {
private:
  const unsigned long intervalMs;
  unsigned long lastMeasurementTimeMs;

  peripherals::Dht11 *dht11;
  peripherals::YfS401 *yfS401;
  peripherals::Sen0193 *sen0193;

  const unsigned long getIntervalMs() const;

  unsigned long getLastMeasurementTimeMs() const;

  void setLastMeasurementTimeMs(const unsigned long lastMeasurementTimeMs);

  peripherals::Dht11 *getDht11();

  peripherals::YfS401 *getYfS401();

  peripherals::Sen0193 *getSen0193();

public:
  Controller(unsigned long intervalMs, peripherals::Dht11 *dht11,
             peripherals::YfS401 *yfS401, peripherals::Sen0193 *sen0193);

  ~Controller();

  bool IsMeasurementTimeReached();

  Measures Measure();

  void StartPump(bool state);

  void Begin();

  void Loop();
};

} // namespace control

#endif // CONTROLLER_H