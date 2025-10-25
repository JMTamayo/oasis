#include "operations.h"

namespace control {

Operation OperationFromString(String command) {
  if (command == "restart") {
    return Operation::RESTART;
  } else if (command == "ping") {
    return Operation::PING;
  } else {
    return Operation::UNKNOWN;
  }
}

} // namespace control