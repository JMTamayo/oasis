#include <DHT.h>

#include "dht11.h"

namespace peripherals {

AirProperties::AirProperties(float temperature, float relativeHumidity)
    : temperature(temperature), relativeHumidity(relativeHumidity) {}

AirProperties::~AirProperties() {}

float AirProperties::GetTemperature() { return this->temperature; }

float AirProperties::GetRelativeHumidity() { return this->relativeHumidity; }

DHT Dht11::getDht() { return this->dht; }

Dht11::Dht11(const unsigned int pin) : dht(pin, DHT11) {}

Dht11::~Dht11() {}

AirProperties Dht11::Read() {
  return AirProperties(this->dht.readTemperature(), this->dht.readHumidity());
}

} // namespace peripherals