#ifndef LED_H
#define LED_H

#include <Arduino.h>

namespace peripherals {

/**
  Led class.

  This class is used to represent a LED that can be controlled in ON/OFF mode.
 */
class Led {
private:
  const unsigned int pin; // The pin number of the LED.

  /**
    Get the pin number of the LED.

    Returns:
      The pin number of the LED.
   */
  const unsigned int getPin() const;

public:
  /**
    Constructor.

    Parameters:
      pin - The pin number of the LED.
   */
  Led(unsigned int pin);

  /**
    Destructor.
   */
  ~Led();

  /**
    Light up the LED.

    Parameters:
      on - True to light up the LED, false to turn it off.
   */
  void LightUp(bool on);

  /**
    Blink the LED.

    This function blinks the LED on and off.
   */
  void Blink();
};

} // namespace peripherals

#endif // LED_H