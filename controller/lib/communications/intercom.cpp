#include "intercom.h"

namespace communications {

unsigned long Intercom::getBaudRate() { return this->baudRate; }

const char *Intercom::getCommandDelimiter() { return this->commandDelimiter; }

peripherals::Led *Intercom::getLed() { return this->led; }

Intercom::Intercom(unsigned long baudRate, const char *commandDelimiter,
                   peripherals::Led *led)
    : baudRate(baudRate), commandDelimiter(commandDelimiter), led(led) {}

Intercom::~Intercom() {}

void Intercom::Begin() {
  Serial.begin(this->getBaudRate());
  while (!Serial)
    delay(100);

  this->getLed()->Begin();
}

models::Message *Intercom::Loop() {
  if (Serial.available() > 0) {
    this->getLed()->High();

    String request = Serial.readStringUntil('\n');
    request.trim();

    int separatorIndex = request.indexOf(this->getCommandDelimiter());

    String command = request.substring(0, separatorIndex);
    command.trim();

    String value =
        separatorIndex != -1 ? request.substring(separatorIndex + 1) : "";
    value.trim();

    this->getLed()->Low();

    return command.isEmpty() && value.isEmpty()
               ? nullptr
               : new models::Message(command, value);
  }

  return nullptr;
}

void Intercom::Send(models::Message *input) {
  if (input == nullptr)
    return;

  String command = input->getCommand();
  String payload = input->getPayload();

  if (command.isEmpty())
    return;

  String output = payload.isEmpty()
                      ? command
                      : command + this->getCommandDelimiter() + payload;

  Serial.println(output);
}

} // namespace communications