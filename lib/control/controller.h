#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "measures.h"

#include "dht11.h"
#include "yf_s401.h"

namespace control {

class Controller {
private:
  const unsigned long intervalMs;
  unsigned long lastMeasurementTimeMs;

  peripherals::Dht11 *dht11;
  peripherals::YfS401 *yfS401;

  const unsigned long getIntervalMs() const;

  unsigned long getLastMeasurementTimeMs() const;

  void setLastMeasurementTimeMs(const unsigned long lastMeasurementTimeMs);

  peripherals::Dht11 *getDht11();

  peripherals::YfS401 *getYfS401();

public:
  Controller(unsigned long intervalMs, peripherals::Dht11 *dht11,
             peripherals::YfS401 *yfS401);

  ~Controller();

  bool IsMeasurementTimeReached();

  measurements::Measures Measure();

  void StartPump(bool state);

  void Begin();

  void Loop();
};

} // namespace control

#endif // CONTROLLER_H