#include "sen0193.h"

namespace peripherals {

const unsigned int Sen0193::getPin() const { return this->pin; }

unsigned long Sen0193::getAirValue() const { return this->airValue; }

unsigned long Sen0193::getWaterValue() const { return this->waterValue; }

Sen0193::Sen0193(const unsigned int pin, const unsigned int resolution,
                 unsigned long waterValue, unsigned long airValue)
    : pin(pin) {
  if (waterValue < airValue && airValue <= resolution) {
    this->airValue = airValue;
    this->waterValue = waterValue;

  } else {
    this->airValue = resolution;
    this->waterValue = 0;
  }

  pinMode(this->getPin(), INPUT);
}

Sen0193::~Sen0193() {}

float Sen0193::Read() {
  const float airValue = (float)this->getAirValue();
  const float waterValue = (float)this->getWaterValue();
  const float range = airValue - waterValue;

  const float value =
      constrain((float)analogRead(this->getPin()), waterValue, airValue);
  float moisture =
      100.0f * constrain(1.0f - (value - waterValue) / range, 0.0f, 1.0f);

  return moisture;
}
} // namespace peripherals