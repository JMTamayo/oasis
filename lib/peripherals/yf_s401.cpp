#include "yf_s401.h"

namespace peripherals {

FlowRate::FlowRate(float flowRate, float volume,
                   unsigned long sampleTimeSeconds)
    : flowRate(flowRate), volume(volume), sampleTimeSeconds(sampleTimeSeconds) {
}

FlowRate::~FlowRate() {}

float FlowRate::GetFlowRate() { return this->flowRate; }

float FlowRate::GetVolume() { return this->volume; }

unsigned long FlowRate::GetSampleTimeSeconds() {
  return this->sampleTimeSeconds;
}

volatile unsigned long YfS401PulseCount = 0;

void IRAM_ATTR incrementPulseCount() { YfS401PulseCount++; }

const unsigned int YfS401::getPin() const { return this->pin; }

float YfS401::getVolumePerPulse() const { return this->volumePerPulse; }

unsigned long YfS401::getLastMeasurementTimeMs() const {
  return this->lastMeasurementTimeMs;
}

void YfS401::setLastMeasurementTimeMs(unsigned long lastMeasurementTimeMs) {
  this->lastMeasurementTimeMs = lastMeasurementTimeMs;
}

YfS401::YfS401(const unsigned int pin, const float volumePerPulse)
    : pin(pin), volumePerPulse(volumePerPulse),
      lastMeasurementTimeMs(millis()) {
  pinMode(pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(pin), incrementPulseCount, RISING);
}

YfS401::~YfS401() {}

void YfS401::Restart() {
  YfS401PulseCount = 0;
  this->setLastMeasurementTimeMs(millis());
}

FlowRate YfS401::Read() {
  unsigned long sampleTimeSeconds =
      (millis() - this->getLastMeasurementTimeMs()) / 1000;

  float volume = YfS401PulseCount * this->getVolumePerPulse();
  float flowRate = volume / sampleTimeSeconds;

  this->Restart();

  return FlowRate(flowRate, volume, sampleTimeSeconds);
}

} // namespace peripherals