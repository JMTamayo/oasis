#include "intercom.h"

namespace intercom {

HardwareSerial *Intercom::getSerial() { return this->serial; }

int Intercom::getTxPin() const { return this->txPin; }

int Intercom::getRxPin() const { return this->rxPin; }

unsigned int Intercom::getBaudRate() const { return this->baudRate; }

const char *Intercom::getCommandSeparator() const {
  return this->commandSeparator;
}

logging::Logger *Intercom::getLogger() { return this->logger; }

Intercom::Intercom(int txPin, int rxPin, unsigned int baudRate,
                   const char *commandSeparator, logging::Logger *logger)
    : txPin(txPin), rxPin(rxPin), baudRate(baudRate),
      commandSeparator(commandSeparator), logger(logger) {
  this->serial = new HardwareSerial(1);
}

Intercom::~Intercom() {}

void Intercom::Begin() {
  this->getSerial()->begin(this->getBaudRate(), SERIAL_8N1, this->getRxPin(),
                           this->getTxPin());
  while (!this->getSerial())
    continue;

  this->getLogger()->Info("INTERCOM",
                          "Intercom system initialized successfully");
}

void Intercom::Transmit(String command, String payload) {
  this->getSerial()->println(command + ":" + payload);
}

services::MqttMessage *Intercom::Loop() {
  if (this->getSerial()->available() > 0) {
    String data = this->getSerial()->readStringUntil('\n');
    data.trim();

    this->getLogger()->Debug(
        "INTERCOM", "Data received from the controller: '" + data + "'");

    int index = data.indexOf(this->getCommandSeparator());

    String command = data.substring(0, index);
    command.trim();

    String value = index == -1 ? "" : data.substring(index + 1);
    value.trim();

    return new services::MqttMessage(command, value);
  }

  return nullptr;
}

} // namespace intercom