#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "message.h"
#include "operations.h"

namespace control {

class Controller {
private:
  void restartDevice();

public:
  Controller();

  ~Controller();

  void Begin();

  models::Message *Loop(models::Message *message);
};

} // namespace control

#endif // CONTROLLER_H