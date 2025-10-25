#include "controller.h"

namespace control {

void Controller::restartDevice() { ESP.restart(); }

Controller::Controller() {}

Controller::~Controller() {}

void Controller::Begin() {
  // TODO: Implement
}

models::Message *Controller::Loop(models::Message *message) {
  if (message == nullptr)
    return nullptr;

  models::Message *response = nullptr;
  Operation operation = OperationFromString(message->getCommand());

  switch (operation) {
  case Operation::RESTART:
    this->restartDevice();

  case Operation::PING:
    response = new models::Message("pong", "");
    break;

  default:
    response =
        new models::Message("error", String("command not implemented (") +
                                         message->getCommand() + ")");
    break;
  }

  return response;
}

} // namespace control