#ifndef INTERCOM_H
#define INTERCOM_H

#include <Arduino.h>
#include <HardwareSerial.h>

#include "logger.h"

#include "mqtt.h"

namespace intercom {

class Intercom {
private:
  int txPin;
  int rxPin;
  unsigned int baudRate;
  const char *commandSeparator;

  HardwareSerial *serial;

  logging::Logger *logger;

  int getTxPin() const;

  int getRxPin() const;

  unsigned int getBaudRate() const;

  const char *getCommandSeparator() const;

  HardwareSerial *getSerial();

  logging::Logger *getLogger();

public:
  Intercom(int txPin, int rxPin, unsigned int baudRate,
           const char *commandSeparator, logging::Logger *logger);

  ~Intercom();

  void Begin();

  void Transmit(String command, String payload);

  services::MqttMessage *Loop();
};

} // namespace intercom

#endif // INTERCOM_H