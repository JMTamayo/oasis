#include "led.h"

namespace peripherals {

const unsigned int Led::getPin() const { return this->pin; }

Led::Led(unsigned int pin) : pin(pin) {
  pinMode(this->getPin(), OUTPUT);
  this->Low();
}

Led::~Led() {}

void Led::High() { digitalWrite(this->getPin(), LOW); }

void Led::Low() { digitalWrite(this->getPin(), HIGH); }

} // namespace peripherals