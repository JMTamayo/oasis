#ifndef SR602_H
#define SR602_H

#include <Arduino.h>

namespace peripherals {

/**
  Sr602 class.

  This class is used to represent a SR602 infrared sensor that can be
  used to detect motion.
 */
class Sr602 {
private:
  const unsigned int pin; // The pin number of the SR602 infrared sensor.

  /**
    Get the pin number.

    Returns:
      The pin number of the SR602 infrared sensor.
   */
  const unsigned int getPin() const;

public:
  /**
    Constructor.

    Parameters:
    pin - The pin number of the SR602 infrared sensor.
   */
  Sr602(unsigned int pin);

  /**
    Destructor.
   */
  ~Sr602();

  /**
    Read the motion status.

    Returns:
      True if motion is detected, false otherwise.
   */
  bool Read();
};

} // namespace peripherals

#endif // SR602_H