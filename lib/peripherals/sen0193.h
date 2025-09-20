#ifndef SEN0193_H
#define SEN0193_H

#include <Arduino.h>

#include "logger.h"

namespace peripherals {

enum SoilMoistureLevel {
  EXTREMELY_DRY = 0,
  DRY = 1,
  MOIST = 2,
  WET = 3,
  SATURATED = 4
};

class SoilMoisture {
private:
  float moisture;
  SoilMoistureLevel level;

public:
  SoilMoisture(float moisture, SoilMoistureLevel level);

  ~SoilMoisture();

  float GetMoisture();

  SoilMoistureLevel GetLevel();

  String GetLevelString();
};

class Sen0193 {
private:
  const unsigned int pin;

  unsigned long airValue;
  unsigned long waterValue;

  const unsigned int getPin() const;

  unsigned long getAirValue() const;

  unsigned long getWaterValue() const;

public:
  Sen0193(const unsigned int pin, unsigned long waterValue,
          unsigned long airValue);

  ~Sen0193();

  SoilMoisture Read();
};

} // namespace peripherals

#endif // SEN0193_H