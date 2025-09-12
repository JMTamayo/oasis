#include <Arduino.h>

#include "collector.h"

namespace sampling {

const unsigned long Collector::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Collector::getLastSampleTimeMs() const {
  return this->lastSampleTimeMs;
}

void Collector::setLastSampleTimeMs(const unsigned long lastSampleTimeMs) {
  this->lastSampleTimeMs = lastSampleTimeMs;
}

Collector::Collector(const unsigned long intervalMs)
    : intervalMs(intervalMs), lastSampleTimeMs(millis()) {}

Collector::~Collector() {}

bool Collector::IsSamplingTimeReached() {
  return millis() - this->getLastSampleTimeMs() >= this->getIntervalMs();
}

Sample Collector::Collect() {
  float temperature = random(2400, 3000) / 100.0;
  float humidity = random(4000, 6000) / 100.0;

  this->setLastSampleTimeMs(millis());

  return Sample(temperature, humidity);
}

} // namespace sampling