#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>

#include "led.h"
#include "logger.h"

namespace services {

class WifiService {
private:
  const char *ssid;
  const char *password;
  const unsigned long maxRetryTimeMs;

  logging::Logger *logger;
  peripherals::Led *led;

  const char *getSsid() const;

  const char *getPassword() const;

  const unsigned long getMaxRetryTimeMs() const;

  logging::Logger *getLogger();

  peripherals::Led *getLed();

public:
  WifiService(const char *ssid, const char *password,
              const unsigned long maxRetryTimeMs, logging::Logger *logger,
              peripherals::Led *led);

  ~WifiService();

  void Connect();

  bool IsConnected();
};

} // namespace services

#endif // WIFI_H