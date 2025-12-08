#include "logger.h"

namespace logging {

const unsigned long Logger::getBaudRate() const { return this->baudRate; }

Logger::Logger() : baudRate(115200) {}

void Logger::Begin() {
  Serial.begin(this->getBaudRate());
    while (!Serial)
      continue;

  this->Info("[LOGGER] Logger initialized successfully");
}

Logger::~Logger() {}

void Logger::Error(String message) {
  Serial.println("level=\"ERROR\"; message=\"" + message + "\"");
}

void Logger::Warning(String message) {
  Serial.println("level=\"WARNING\"; message=\"" + message + "\"");
}

void Logger::Debug(String message) {
  Serial.println("level=\"DEBUG\"; message=\"" + message + "\"");
}

void Logger::Info(String message) {
  Serial.println("level=\"INFO\"; message=\"" + message + "\"");
}

} // namespace logging