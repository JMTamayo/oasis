#ifndef GEOLOCATION_H
#define GEOLOCATION_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

#include "logger.h"

#include "mqtt.h"

namespace services {

/**
  Geolocation service class.

  Utilities to manage the geolocation in the context of the application. It
  contains the methods to verify if the localization time is reached and get the
  device's geolocation.
 */
class Geolocation {
private:
  HTTPClient
      *httpClient; // The HTTP client to use to get the device's geolocation.

  const char *mqttSubjectGeolocation; // The subject of the MQTT message for the
                                      // geolocation.

  const unsigned long intervalMs; // The interval time in milliseconds to get
                                  // the device's geolocation.
  unsigned long lastLocalizationTimeMs; // The last time the device's
                                        // geolocation was obtained.

  logging::Logger *logger; // The logger to use to log important information
                           // about the geolocation.

  /**
    Get the HTTP client.

    Returns:
      The HTTP client.
   */
  HTTPClient *getHttpClient() const;

  /**
    Get the subject of the MQTT message.

    Returns:
      The subject of the MQTT message for the geolocation.
   */
  const char *getMqttSubjectGeolocation() const;

  /**
    Get the interval time in milliseconds.

    Returns:
      The interval time in milliseconds to get the device's geolocation.
   */
  const unsigned long getIntervalMs() const;

  /**
    Get the last localization time.

    Returns:
      The last time the device's geolocation was obtained.
   */
  unsigned long getLastLocalizationTimeMs() const;

  /**
    Set the last localization time.

    Parameters:
      lastLocalizationTimeMs - The last time the device's geolocation was
        obtained.
   */
  void setLastLocalizationTimeMs(unsigned long lastLocalizationTimeMs);

  /**
    Get the logger.

    Returns:
      The logger to use to log important information about the geolocation.
   */
  logging::Logger *getLogger();

public:
  /**
    Constructor.

    Parameters:
      requestUrl - The URL to get the device's geolocation.
      requestTimeoutMs - The timeout in milliseconds to get the device's
        geolocation.
      intervalMs - The interval time in milliseconds to get the device's
        geolocation.
      mqttSubjectGeolocation - The subject of the MQTT message for the
        geolocation.
      logger - The logger to use to log important information about the
        geolocation.
   */
  Geolocation(const char *requestUrl, const unsigned long requestTimeoutMs,
              const unsigned long intervalMs,
              const char *mqttSubjectGeolocation, logging::Logger *logger);

  /**
    Destructor.
   */
  ~Geolocation();

  /**
    Check if the localization time is reached.

    Returns:
      True if the localization time is reached, false otherwise.
   */
  bool IsLocalizationTimeReached();

  /**
    Get the device's geolocation.

    Returns:
      The device's geolocation.
   */
  String Localize();

  /**
    Loop the geolocation.

    Returns:
      The MQTT message for the geolocation.
   */
  services::MqttMessage *Loop();
};

} // namespace services

#endif // GEOLOCATION_H