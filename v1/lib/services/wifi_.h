#ifndef WIFI_H
#define WIFI_H

#include <WiFi.h>

#include "logger.h"

namespace services {

/**
  WiFi service class.

  Utilities to manage the WiFi connection in the context of the application. It
  contains the methods to connect to the WiFi network, check if the connection
  is established and get the LED to use to indicate the WiFi connection status.
 */
class WifiService {
private:
  const char *ssid;                   // The WiFi network identifier (SSID).
  const char *password;               // The WiFi network password (PSK).
  const unsigned long maxRetryTimeMs; // The maximum retry time to connect to
                                      // the WiFi network in milliseconds.

  logging::Logger *logger; // The logger to use to log important information
                           // about the WiFi connection.

  /**
    Get the WiFi network identifier (SSID).

    Returns:
      The WiFi network identifier.
   */
  const char *getSsid() const;

  /**
    Get the WiFi network password (PSK).

    Returns:
      The WiFi network password.
   */
  const char *getPassword() const;

  /**
    Get the maximum retry time.

    Returns:
      The maximum retry time to connect to the WiFi network in milliseconds.
   */
  const unsigned long getMaxRetryTimeMs() const;

  /**
    Get the logger.

    Returns:
      The logger to use to log important information about the WiFi connection.
   */
  logging::Logger *getLogger();

public:
  /**
    Constructor.

    Parameters:
      ssid - The WiFi network identifier (SSID).
      password - The WiFi network password (PSK).
      maxRetryTimeMs - The maximum retry time to connect to the WiFi network in
        milliseconds.
      logger - The logger to use to log important information about
        the WiFi connection.
   */
  WifiService(const char *ssid, const char *password,
              const unsigned long maxRetryTimeMs, logging::Logger *logger);

  /**
    Destructor.
   */
  ~WifiService();

  /**
    Connect to the WiFi network.

    This method connects to the WiFi network using the SSID and password
    provided in the constructor.
   */
  void Connect();

  /**
    Check if the WiFi network is connected.

    Returns:
      True if the WiFi network is connected, false otherwise.
   */
  bool IsConnected();
};

} // namespace services

#endif // WIFI_H