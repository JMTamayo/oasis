#include "controller.h"

namespace control {

Measures::Measures(peripherals::AirProperties airProperties,
                   peripherals::FlowRate waterFlowRate,
                   peripherals::SoilMoisture soilMoisture, bool pumpState)
    : airProperties(airProperties), waterFlowRate(waterFlowRate),
      soilMoisture(soilMoisture), pumpState(pumpState) {}

Measures::~Measures() {}

peripherals::AirProperties Measures::GetAirProperties() {
  return this->airProperties;
}

peripherals::FlowRate Measures::GetWaterFlowRate() {
  return this->waterFlowRate;
}

peripherals::SoilMoisture Measures::GetSoilMoisture() {
  return this->soilMoisture;
}

bool Measures::GetPumpState() { return this->pumpState; }

const unsigned long Controller::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Controller::getLastMeasurementTimeMs() const {
  return this->lastMeasurementTimeMs;
}

peripherals::Dht11 *Controller::getDht11() { return this->dht11; }

peripherals::YfS401 *Controller::getYfS401() { return this->yfS401; }

peripherals::Sen0193 *Controller::getSen0193() { return this->sen0193; }

void Controller::setLastMeasurementTimeMs(
    const unsigned long lastMeasurementTimeMs) {
  this->lastMeasurementTimeMs = lastMeasurementTimeMs;
}

Controller::Controller(const unsigned long intervalMs,
                       peripherals::Dht11 *dht11, peripherals::YfS401 *yfS401,
                       peripherals::Sen0193 *sen0193)
    : intervalMs(intervalMs), lastMeasurementTimeMs(millis()), dht11(dht11),
      yfS401(yfS401), sen0193(sen0193) {}

Controller::~Controller() {}

bool Controller::IsMeasurementTimeReached() {
  return millis() - this->getLastMeasurementTimeMs() >= this->getIntervalMs();
}

Measures Controller::Measure() {
  peripherals::AirProperties airProperties = this->getDht11()->Read();
  peripherals::FlowRate waterFlowRate = this->getYfS401()->Read();
  peripherals::SoilMoisture soilMoisture = this->getSen0193()->Read();

  bool pumpState = false; // TODO: Implement measure pump state

  this->setLastMeasurementTimeMs(millis());

  return Measures(airProperties, waterFlowRate, soilMoisture, pumpState);
}

void Controller::StartPump(bool state) {
  // TODO: Implement start pump
}

void Controller::Begin() { this->yfS401->Restart(); }

void Controller::Loop() {
  // TODO: Implement control loop
}

} // namespace control