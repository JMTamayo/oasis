#include "parameters.h"

#include "led.h"

#include "controller.h"
#include "intercom.h"

#include "message.h"

peripherals::Led *led = new peripherals::Led(LED_PIN);
communications::Intercom *intercom =
    new communications::Intercom(BAUD_RATE, COMMAND_DELIMITER, led);

control::Controller *controller = new control::Controller();

models::Message *incomingMessage = nullptr;

void setup() {
  controller->Begin();
  intercom->Begin();
}

void loop() {
  incomingMessage = intercom->Loop();

  if (incomingMessage != nullptr) {
    models::Message *response = controller->Loop(incomingMessage);
    if (response != nullptr) {
      intercom->Send(response);
      delete response;
    }

    delete incomingMessage;
  }
}
