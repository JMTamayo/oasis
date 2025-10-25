#include "message.h"

namespace models {

Message::Message(String command, String payload)
    : command(command), payload(payload) {}

Message::~Message() {}

String Message::getCommand() { return this->command; }

String Message::getPayload() { return this->payload; }

} // namespace models