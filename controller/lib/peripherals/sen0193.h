#ifndef SEN0193_H
#define SEN0193_H

#include <Arduino.h>

namespace peripherals {

class Sen0193 {
private:
  const unsigned int pin;

  unsigned long airValue;
  unsigned long waterValue;

  const unsigned int getPin() const;

  unsigned long getAirValue() const;

  unsigned long getWaterValue() const;

public:
  Sen0193(const unsigned int pin, const unsigned int resolution,
          unsigned long waterValue, unsigned long airValue);

  ~Sen0193();

  float Read();
};

} // namespace peripherals

#endif // SEN0193_H