#ifndef LED_H
#define LED_H

#include <Arduino.h>

namespace peripherals {

class Led {
private:
  const unsigned int pin;

  const unsigned int getPin() const;

public:
  Led(unsigned int pin);

  ~Led();

  void Begin();

  void High();

  void Low();
};

} // namespace peripherals

#endif // LED_H