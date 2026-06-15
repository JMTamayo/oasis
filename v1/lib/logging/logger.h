#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

namespace logging {

class Logger {
private:
  const unsigned long baudRate;

  const unsigned long getBaudRate() const;

public:
  Logger();

  ~Logger();

  void Begin();

  void Error(String source, String message);

  void Warning(String source, String message);

  void Debug(String source, String message);

  void Info(String source, String message);
};

} // namespace logging

#endif // LOGGER_H