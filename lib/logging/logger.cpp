#include "logger.h"

namespace logging {

const unsigned long Logger::getBaudRate() const { return this->baudRate; }

Logger::Logger() : baudRate(115200) {}

void Logger::Begin() {
  Serial.begin(this->getBaudRate());
  while (!Serial)
    continue;

  this->Info("LOGGER", "Logger initialized successfully");
}

Logger::~Logger() {}

void Logger::Error(String source, String message) {
  Serial.println("level=\"ERROR\" source=\"" + source + "\" message=\"" +
                 message + "\"");
}

void Logger::Warning(String source, String message) {
  Serial.println("level=\"WARNING\" source=\"" + source + "\" message=\"" +
                 message + "\"");
}

void Logger::Debug(String source, String message) {
  Serial.println("level=\"DEBUG\" source=\"" + source + "\" message=\"" +
                 message + "\"");
}

void Logger::Info(String source, String message) {
  Serial.println("level=\"INFO\" source=\"" + source + "\" message=\"" +
                 message + "\"");
}

} // namespace logging