#ifndef OPERATIONS_H
#define OPERATIONS_H

#include <Arduino.h>

namespace control {

enum class Operation { UNKNOWN, RESTART, PING };

Operation OperationFromString(String command);

} // namespace control

#endif // OPERATIONS_H