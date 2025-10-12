#include "controller.h"

namespace control {

const unsigned long Controller::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Controller::getLastMeasurementTimeMs() const {
  return this->lastMeasurementTimeMs;
}

peripherals::Dht22 *Controller::getDht22() { return this->dht22; }

peripherals::Sen0193 *Controller::getSen0193() { return this->sen0193; }

void Controller::setLastMeasurementTimeMs(
    const unsigned long lastMeasurementTimeMs) {
  this->lastMeasurementTimeMs = lastMeasurementTimeMs;
}

peripherals::Led *Controller::getLowMoistureLedIndicator() {
  return this->lowMoistureLedIndicator;
}

peripherals::Lcd1602I2c *Controller::getLcd1602I2c() {
  return this->lcd1602I2c;
}

Controller::Controller(const unsigned long intervalMs,
                       peripherals::Dht22 *dht22, peripherals::Sen0193 *sen0193,
                       peripherals::Led *lowMoistureLedIndicator,
                       peripherals::Lcd1602I2c *lcd1602I2c)
    : intervalMs(intervalMs), lastMeasurementTimeMs(millis() - intervalMs),
      dht22(dht22), sen0193(sen0193),
      lowMoistureLedIndicator(lowMoistureLedIndicator), lcd1602I2c(lcd1602I2c) {
}

Controller::~Controller() {}

bool Controller::IsMeasurementTimeReached() {
  return millis() - this->getLastMeasurementTimeMs() >= this->getIntervalMs();
}

Measures Controller::Measure() {
  peripherals::AirProperties airProperties = this->getDht22()->Read();
  float airTemperature = airProperties.GetTemperature();
  float airRelativeHumidity = airProperties.GetRelativeHumidity();

  peripherals::SoilMoisture soilMoisture = this->getSen0193()->Read();
  float soilMoistureValue = soilMoisture.GetMoisture();
  String soilMoistureLevel = soilMoisture.GetLevelString();

  // TODO: Implement tank level measurement
  float tankLevel = NAN;

  this->getLcd1602I2c()->DisplayProcessStatus(
      airTemperature, airRelativeHumidity, soilMoistureValue, tankLevel);

  this->setLastMeasurementTimeMs(millis());

  return Measures(airTemperature, airRelativeHumidity, soilMoistureValue,
                  soilMoistureLevel, tankLevel);
}

void Controller::StartPump(bool state) {
  // TODO: Implement start pump
}

void Controller::Loop() {
  peripherals::SoilMoisture soilMoisture = this->getSen0193()->Read();
  float moisture = soilMoisture.GetMoisture();
  peripherals::SoilMoistureLevel moistureLevel = soilMoisture.GetLevel();

  if (moistureLevel == peripherals::SoilMoistureLevel::EXTREMELY_DRY ||
      moistureLevel == peripherals::SoilMoistureLevel::DRY) {
    this->getLowMoistureLedIndicator()->High();
  } else {
    this->getLowMoistureLedIndicator()->Low();
  }
}

} // namespace control