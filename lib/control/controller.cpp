#include <Arduino.h>

#include "controller.h"

namespace control {

const unsigned long Controller::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Controller::getLastMeasurementTimeMs() const {
  return this->lastMeasurementTimeMs;
}

peripherals::Dht11 *Controller::getDht11() { return this->dht11; }

peripherals::YfS401 *Controller::getYfS401() { return this->yfS401; }

void Controller::setLastMeasurementTimeMs(
    const unsigned long lastMeasurementTimeMs) {
  this->lastMeasurementTimeMs = lastMeasurementTimeMs;
}

Controller::Controller(const unsigned long intervalMs,
                       peripherals::Dht11 *dht11, peripherals::YfS401 *yfS401)
    : intervalMs(intervalMs), lastMeasurementTimeMs(millis()), dht11(dht11),
      yfS401(yfS401) {}

Controller::~Controller() {}

bool Controller::IsMeasurementTimeReached() {
  return millis() - this->getLastMeasurementTimeMs() >= this->getIntervalMs();
}

measurements::Measures Controller::Measure() {
  peripherals::AirProperties airProperties = this->getDht11()->Read();
  peripherals::FlowRate waterFlowRate = this->getYfS401()->Read();

  bool pumpState = random(0, 2) == 1; // TODO: Implement measure pump state

  this->setLastMeasurementTimeMs(millis());

  return measurements::Measures(
      airProperties.GetTemperature(), airProperties.GetRelativeHumidity(),
      waterFlowRate.GetFlowRate(), waterFlowRate.GetVolume(), pumpState);
}

void Controller::StartPump(bool state) {
  // TODO: Implement start pump
}

void Controller::Begin() { this->yfS401->Restart(); }

void Controller::Loop() {
  // TODO: Implement control loop
}

} // namespace control