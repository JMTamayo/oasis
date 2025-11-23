#include "pump.h"

namespace peripherals {

const unsigned int Pump::getPin() const { return this->pin; }

const unsigned int Pump::getResolution() const { return this->resolution; }

Pump::Pump(unsigned int pin, unsigned int resolution)
    : pin(pin), resolution(resolution) {
  pinMode(this->getPin(), OUTPUT);
}

Pump::~Pump() {}

void Pump::Run(float speed) {
  unsigned int signal = map(speed, 0.0f, 100.0f, 0, this->getResolution());
  analogWrite(this->getPin(), signal);
}

} // namespace peripherals