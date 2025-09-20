#ifndef YF_S401_H
#define YF_S401_H

#include <Arduino.h>

namespace peripherals {

class FlowRate {
private:
  float flowRate;
  float volume;
  unsigned long sampleTimeSeconds;

public:
  FlowRate(float flowRate, float volume, unsigned long sampleTimeSeconds);

  ~FlowRate();

  float GetFlowRate();

  float GetVolume();

  unsigned long GetSampleTimeSeconds();
};

extern volatile unsigned long YfS401PulseCount;

void IRAM_ATTR incrementPulseCount();

class YfS401 {
private:
  const unsigned int pin;

  const float volumePerPulse;
  unsigned long lastMeasurementTimeMs;

  const unsigned int getPin() const;

  float getVolumePerPulse() const;

  unsigned long getLastMeasurementTimeMs() const;

  void setLastMeasurementTimeMs(unsigned long lastMeasurementTimeMs);

public:
  YfS401(const unsigned int pin, const float volumePerPulse);

  ~YfS401();

  void Restart();

  FlowRate Read();
};

} // namespace peripherals

#endif // YF_S401_H