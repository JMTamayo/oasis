#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "measures.h"

namespace control {

class Controller {
private:
  const unsigned long intervalMs;
  unsigned long lastMeasurementTimeMs;

  const unsigned long getIntervalMs() const;

  unsigned long getLastMeasurementTimeMs() const;

  void setLastMeasurementTimeMs(const unsigned long lastMeasurementTimeMs);

public:
  Controller(unsigned long intervalMs);

  ~Controller();

  bool IsMeasurementTimeReached();

  measurements::Measures Measure();

  void Loop();
};

} // namespace control

#endif // CONTROLLER_H