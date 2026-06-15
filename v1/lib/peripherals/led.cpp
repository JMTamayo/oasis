#include "led.h"

namespace peripherals {

const unsigned int Led::getPin() const { return this->pin; }

Led::Led(unsigned int pin) : pin(pin) { pinMode(this->getPin(), OUTPUT); }

Led::~Led() {}

void Led::LightUp(bool on) { digitalWrite(this->getPin(), on ? HIGH : LOW); }

void Led::Blink() {
  this->LightUp(true);
  vTaskDelay(pdMS_TO_TICKS(20));
  this->LightUp(false);
}

} // namespace peripherals