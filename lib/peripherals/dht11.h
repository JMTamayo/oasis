#ifndef DHT11_H
#define DHT11_H

#include <DHT.h>

namespace peripherals {

class AirProperties {
private:
  float temperature;
  float relativeHumidity;

public:
  AirProperties(float temperature, float relativeHumidity);

  ~AirProperties();

  float GetTemperature();

  float GetRelativeHumidity();
};

class Dht11 {
private:
  DHT dht;

  DHT getDht();

public:
  Dht11(unsigned int pin);

  ~Dht11();

  AirProperties Read();
};

} // namespace peripherals

#endif // DHT11_H