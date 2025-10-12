#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

#include "measurements.h"

#include "dht22.h"
#include "lcd1602_i2c.h"
#include "led.h"
#include "sen0193.h"

namespace control {

class Controller {
private:
  const unsigned long intervalMs;
  unsigned long lastMeasurementTimeMs;

  peripherals::Dht22 *dht22;
  peripherals::Sen0193 *sen0193;

  peripherals::Led *lowMoistureLedIndicator;
  peripherals::Lcd1602I2c *lcd1602I2c;

  const unsigned long getIntervalMs() const;

  unsigned long getLastMeasurementTimeMs() const;

  void setLastMeasurementTimeMs(const unsigned long lastMeasurementTimeMs);

  peripherals::Dht22 *getDht22();

  peripherals::Sen0193 *getSen0193();

  peripherals::Led *getLowMoistureLedIndicator();

  peripherals::Lcd1602I2c *getLcd1602I2c();

public:
  Controller(unsigned long intervalMs, peripherals::Dht22 *dht22,
             peripherals::Sen0193 *sen0193,
             peripherals::Led *lowMoistureLedIndicator,
             peripherals::Lcd1602I2c *lcd1602I2c);

  ~Controller();

  bool IsMeasurementTimeReached();

  Measures Measure();

  void StartPump(bool state);

  void Loop();
};

} // namespace control

#endif // CONTROLLER_H