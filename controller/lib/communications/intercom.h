#ifndef INTERCOM_H
#define INTERCOM_H

#include <Arduino.h>

#include "led.h"

#include "message.h"

namespace communications {

class Intercom {
private:
  unsigned long baudRate;
  const char *commandDelimiter;

  peripherals::Led *led;

  unsigned long getBaudRate();

  const char *getCommandDelimiter();

  peripherals::Led *getLed();

public:
  Intercom(unsigned long baudRate, const char *commandDelimiter,
           peripherals::Led *led);

  ~Intercom();

  void Begin();

  models::Message *Loop();

  void Send(models::Message *input);
};

} // namespace communications

#endif // INTERCOM_H