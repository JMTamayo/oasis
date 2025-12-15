#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "logger.h"

#include "mqtt.h"

namespace services {

class Geolocation {
private:
  HTTPClient *httpClient;

  const unsigned long intervalMs;
  unsigned long lastLocalizationTimeMs;

  logging::Logger *logger;

  HTTPClient *getHttpClient() const;

  const unsigned long getIntervalMs() const;

  unsigned long getLastLocalizationTimeMs() const;

  void setLastLocalizationTimeMs(unsigned long lastLocalizationTimeMs);

  logging::Logger *getLogger();

public:
  Geolocation(const char *requestUrl, const unsigned long requestTimeoutMs,
              const unsigned long intervalMs, logging::Logger *logger);

  ~Geolocation();

  bool IsLocalizationTimeReached();

  String Localize();

  services::MqttMessage *Loop();
};

} // namespace services

#endif // GEOLOCATION_H