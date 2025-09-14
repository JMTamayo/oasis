#include <Arduino.h>

#include "controller.h"

namespace control {

const unsigned long Controller::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Controller::getLastMeasurementTimeMs() const {
  return this->lastMeasurementTimeMs;
}

void Controller::setLastMeasurementTimeMs(
    const unsigned long lastMeasurementTimeMs) {
  this->lastMeasurementTimeMs = lastMeasurementTimeMs;
}

Controller::Controller(const unsigned long intervalMs)
    : intervalMs(intervalMs), lastMeasurementTimeMs(millis()) {}

Controller::~Controller() {}

bool Controller::IsMeasurementTimeReached() {
  return millis() - this->getLastMeasurementTimeMs() >= this->getIntervalMs();
}

measurements::Measures Controller::Measure() {
  float temperature = random(2400, 3000) / 100.0; // TODO: Implement measure
  float humidity = random(4000, 6000) / 100.0;    // TODO: Implement measure

  bool pumpState = random(0, 2) == 1; // TODO: Implement measure pump state

  this->setLastMeasurementTimeMs(millis());

  return measurements::Measures(temperature, humidity, pumpState);
}

void Controller::Loop() {
  // TODO: Implement control loop
}

} // namespace control