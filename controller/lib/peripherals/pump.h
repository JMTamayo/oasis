#ifndef PUMP_H
#define PUMP_H

#include <Arduino.h>

namespace peripherals {

class Pump {
private:
  const unsigned int pin;
  const unsigned int resolution;

  const unsigned int getPin() const;

  const unsigned int getResolution() const;

public:
  Pump(unsigned int pin, unsigned int resolution);

  ~Pump();

  void Run(float speed);
};

} // namespace peripherals

#endif // PUMP_H