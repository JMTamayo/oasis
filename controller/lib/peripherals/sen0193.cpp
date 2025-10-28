#include "sen0193.h"

namespace peripherals {

const unsigned int Sen0193::getPin() const { return this->pin; }

unsigned long Sen0193::getAirValue() const { return this->airValue; }

unsigned long Sen0193::getWaterValue() const { return this->waterValue; }

Sen0193::Sen0193(const unsigned int pin, unsigned long waterValue,
                 unsigned long airValue)
    : pin(pin) {
  const unsigned long resolution = 4095;

  if (waterValue < airValue && airValue <= resolution) {
    this->airValue = airValue;
    this->waterValue = waterValue;

  } else {
    this->airValue = resolution;
    this->waterValue = 0;
  }

  pinMode(pin, INPUT);
}

Sen0193::~Sen0193() {}

float Sen0193::Read() {
  unsigned long value = analogRead(this->getPin());

  const float airValue = (float)this->getAirValue();
  const float waterValue = (float)this->getWaterValue();
  const float range = airValue - waterValue;

  float moisture =
      100.0f *
      constrain(1.0f - ((float)value - waterValue) / range, 0.0f, 1.0f);

  return moisture;
}
} // namespace peripherals