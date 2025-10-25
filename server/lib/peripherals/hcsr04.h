#ifndef HC_SR04_H
#define HC_SR04_H

#include <Arduino.h>

namespace peripherals {

class TankLevel {
private:
  float level;
  float height;

public:
  TankLevel(float level, float height);

  ~TankLevel();

  float GetLevel();

  float GetHeight();
};

class Hcsr04 {
private:
  const unsigned int echoPin;
  const unsigned int triggerPin;

  float sensorHeight;
  float maxLevelHeight;

  float signalDelayMicroseconds;

  float soundSpeedCmPerMicrosecond;

  const unsigned int getEchoPin() const;

  const unsigned int getTriggerPin() const;

  float getSensorHeight() const;

  float getMaxLevelHeight() const;

  float getSignalDelayMicroseconds() const;

  float getSoundSpeedCmPerMicrosecond() const;

public:
  Hcsr04(const unsigned int echoPin, const unsigned int triggerPin,
         float sensorHeight, float maxLevelHeight,
         float signalDelayMicroseconds, float soundSpeedCmPerMicrosecond);

  ~Hcsr04();

  TankLevel Read();
};

} // namespace peripherals

#endif