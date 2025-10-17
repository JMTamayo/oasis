#include "controller.h"

namespace control {

const unsigned long Controller::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Controller::getLastMeasurementTimeMs() const {
  return this->lastMeasurementTimeMs;
}

void Controller::resetMeasurementTime() {
  this->lastMeasurementTimeMs = millis();
}

peripherals::Dht22 *Controller::getDht22() { return this->dht22; }

peripherals::Hcsr04 *Controller::getHcsr04() { return this->hcsr04; }

peripherals::Sen0193 *Controller::getSen0193() { return this->sen0193; }

peripherals::Led *Controller::getWarningLedIndicator() {
  return this->warningLedIndicator;
}

peripherals::Lcd1602I2c *Controller::getLcd1602I2c() {
  return this->lcd1602I2c;
}

bool Controller::isMeasurementTimeReached() {
  return millis() - this->getLastMeasurementTimeMs() >= this->getIntervalMs();
}

Controller::Controller(const unsigned long intervalMs,
                       peripherals::Dht22 *dht22, peripherals::Hcsr04 *hcsr04,
                       peripherals::Sen0193 *sen0193,
                       peripherals::Led *warningLedIndicator,
                       peripherals::Lcd1602I2c *lcd1602I2c)
    : intervalMs(intervalMs), lastMeasurementTimeMs(millis() - intervalMs),
      dht22(dht22), hcsr04(hcsr04), sen0193(sen0193),
      warningLedIndicator(warningLedIndicator), lcd1602I2c(lcd1602I2c) {}

Controller::~Controller() {}

Measures *Controller::Measure(bool take) {
  bool resetMeasurementTime = false;
  if (this->isMeasurementTimeReached()) {
    logging::logger->Debug("Measurement time reached. Taking measurements.");
    resetMeasurementTime = true;

  } else if (take) {
    logging::logger->Debug("Taking measurements from sensors.");

  } else {
    return nullptr;
  }

  peripherals::AirProperties airProperties = this->getDht22()->Read();
  float airTemperature = airProperties.GetTemperature();
  float airRelativeHumidity = airProperties.GetRelativeHumidity();

  peripherals::SoilMoisture soilMoisture = this->getSen0193()->Read();
  float soilMoistureValue = soilMoisture.GetMoisture();

  peripherals::TankLevel tankLevel = this->getHcsr04()->Read();
  float tankLevelValue = tankLevel.GetLevel();

  this->getLcd1602I2c()->DisplayProcessStatus(
      airTemperature, airRelativeHumidity, soilMoistureValue, tankLevelValue);

  if (resetMeasurementTime) {
    logging::logger->Debug("Resetting measurement time.");
    this->resetMeasurementTime();
  }

  return new Measures(airTemperature, airRelativeHumidity, soilMoistureValue,
                      tankLevelValue);
}

void Controller::StartPump(bool state) {
  logging::logger->Debug("Setting pump state to: " +
                         String(state ? "ON" : "OFF"));
  // TODO: Implement start pump
}

void Controller::DisplayBacklightOn(bool state) {
  logging::logger->Debug("Setting display backlight state to: " +
                         String(state ? "ON" : "OFF"));
  this->getLcd1602I2c()->BacklightOn(state);
}

void Controller::Loop() {
  float moisture = this->getSen0193()->Read().GetMoisture();
  float tankLevelValue = this->getHcsr04()->Read().GetLevel();
}

} // namespace control