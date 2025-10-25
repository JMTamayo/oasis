#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "logger.h"

namespace services {

class Geolocation {
private:
  HTTPClient *httpClient;

  const unsigned long intervalMs;
  unsigned long lastLocalizationTimeMs;

  HTTPClient *getHttpClient() const;

  const unsigned long getIntervalMs() const;

  unsigned long getLastLocalizationTimeMs() const;

  void setLastLocalizationTimeMs(unsigned long lastLocalizationTimeMs);

public:
  Geolocation(const char *requestUrl, const unsigned long requestTimeoutMs,
              const unsigned long intervalMs);

  ~Geolocation();

  bool IsLocalizationTimeReached();

  String Localize();
};

} // namespace services

#endif // GEOLOCATION_H