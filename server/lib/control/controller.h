#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <Arduino.h>

#include "measurements.h"

#include "dht22.h"
#include "hcsr04.h"
#include "lcd1602_i2c.h"
#include "led.h"
#include "sen0193.h"

#include "logger.h"

namespace control {

class Controller {
private:
  const unsigned long intervalMs;
  unsigned long lastMeasurementTimeMs;

  peripherals::Dht22 *dht22;
  peripherals::Hcsr04 *hcsr04;
  peripherals::Sen0193 *sen0193;

  peripherals::Led *warningLedIndicator;
  peripherals::Lcd1602I2c *lcd1602I2c;

  const unsigned long getIntervalMs() const;

  unsigned long getLastMeasurementTimeMs() const;

  void resetMeasurementTime();

  peripherals::Dht22 *getDht22();

  peripherals::Hcsr04 *getHcsr04();

  peripherals::Sen0193 *getSen0193();

  peripherals::Led *getWarningLedIndicator();

  peripherals::Lcd1602I2c *getLcd1602I2c();

  bool isMeasurementTimeReached();

public:
  Controller(unsigned long intervalMs, peripherals::Dht22 *dht22,
             peripherals::Hcsr04 *hcsr04, peripherals::Sen0193 *sen0193,
             peripherals::Led *warningLedIndicator,
             peripherals::Lcd1602I2c *lcd1602I2c);

  ~Controller();

  Measures *Measure(bool take);

  void StartPump(bool state);

  void DisplayBacklightOn(bool state);

  void Loop();
};

} // namespace control

#endif // CONTROLLER_H