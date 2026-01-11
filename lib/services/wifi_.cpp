#include "wifi_.h"

namespace services {

const char *WifiService::getSsid() const { return this->ssid; }

const char *WifiService::getPassword() const { return this->password; }

const unsigned long WifiService::getMaxRetryTimeMs() const {
  return this->maxRetryTimeMs;
}

logging::Logger *WifiService::getLogger() { return this->logger; }

WifiService::WifiService(const char *ssid, const char *password,
                         const unsigned long maxRetryTimeMs,
                         logging::Logger *logger)
    : ssid(ssid), password(password), maxRetryTimeMs(maxRetryTimeMs),
      logger(logger) {}

WifiService::~WifiService() {}

void WifiService::Connect() {
  WiFi.begin(this->getSsid(), this->getPassword());
  WiFi.mode(WIFI_STA);
  WiFi.setTxPower(WIFI_POWER_8_5dBm);

  unsigned long startTimeMs = millis();
  unsigned long retryTimeMs = 0;

  while (!this->IsConnected()) {
    if (retryTimeMs >= this->getMaxRetryTimeMs()) {
      this->getLogger()->Error("WIFI",
                               "Connection to WiFi network failed. SSID: " +
                                   String(this->getSsid()));
      return;
    }

    vTaskDelay(pdMS_TO_TICKS(100));
    retryTimeMs = millis() - startTimeMs;
  }

  this->getLogger()->Info(
      "WIFI", "Connected to WiFi network. SSID: " + String(this->getSsid()) +
                  ". IP: " + String(WiFi.localIP().toString().c_str()));
}

bool WifiService::IsConnected() { return WiFi.status() == WL_CONNECTED; }

} // namespace services