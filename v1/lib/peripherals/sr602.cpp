#include "sr602.h"

namespace peripherals {

const unsigned int Sr602::getPin() const { return this->pin; }

Sr602::Sr602(unsigned int pin) : pin(pin) { pinMode(this->getPin(), INPUT); }

Sr602::~Sr602() {}

bool Sr602::Read() { return digitalRead(this->getPin()); }

} // namespace peripherals