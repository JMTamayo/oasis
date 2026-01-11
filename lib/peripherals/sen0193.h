#ifndef SEN0193_H
#define SEN0193_H

#include <Arduino.h>

namespace peripherals {

/**
  Sen0193 class.

  This class is used to represent a SEN0193 sensor that can be used to measure
  the soil moisture.
 */
class Sen0193 {
private:
  const unsigned int pin; // The pin number of the SEN0193 sensor.

  unsigned long airValue; // The measured value when the sensor is in the air.
  unsigned long
      waterValue; // The measured value when the sensor is in the water.

  /**
    Get the pin number.

    Returns:
      The pin number of the SEN0193 sensor.
   */
  const unsigned int getPin() const;

  /**
    Get the air value.

    Returns:
      The measured value when the sensor is in the air.
   */
  unsigned long getAirValue() const;

  /**
    Get the water value.

    Returns:
      The measured value when the sensor is in the water.
   */
  unsigned long getWaterValue() const;

public:
  /**
    Constructor.

    Parameters:
      pin - The pin number of the SEN0193 sensor.
      resolution - The resolution of the SEN0193 sensor.
      waterValue - The measured value when the sensor is in the water.
      airValue - The measured value when the sensor is in the air.
   */
  Sen0193(const unsigned int pin, const unsigned int resolution,
          unsigned long waterValue, unsigned long airValue);

  /**
    Destructor.
   */
  ~Sen0193();

  /**
    Read the soil moisture.

    Returns:
      The soil moisture as a float.
   */
  float Read();
};

} // namespace peripherals

#endif // SEN0193_H