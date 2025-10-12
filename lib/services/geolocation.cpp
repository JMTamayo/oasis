#include "geolocation.h"

namespace services {

HTTPClient *Geolocation::getHttpClient() const { return this->httpClient; }

const unsigned long Geolocation::getIntervalMs() const {
  return this->intervalMs;
}

unsigned long Geolocation::getLastLocalizationTimeMs() const {
  return this->lastLocalizationTimeMs;
}

void Geolocation::setLastLocalizationTimeMs(
    unsigned long lastLocalizationTimeMs) {
  this->lastLocalizationTimeMs = lastLocalizationTimeMs;
}

Geolocation::Geolocation(const char *requestUrl,
                         const unsigned long requestTimeoutMs,
                         const unsigned long intervalMs)
    : intervalMs(intervalMs), lastLocalizationTimeMs(millis() - intervalMs) {
  HTTPClient *httpClient = new HTTPClient();

  httpClient->begin(requestUrl);
  httpClient->setTimeout(requestTimeoutMs);

  this->httpClient = httpClient;
}

Geolocation::~Geolocation() { delete this->httpClient; }

bool Geolocation::IsLocalizationTimeReached() {
  return millis() - this->getLastLocalizationTimeMs() >= this->getIntervalMs();
}

String Geolocation::Localize() {
  String emptyString = String("");

  int statusCode = this->httpClient->GET();
  if (statusCode != HTTP_CODE_OK) {
    logging::logger->Error("Failed to get geolocation. Status code: " +
                           String(statusCode));
    return emptyString;
  }

  JsonDocument payloadDoc;
  deserializeJson(payloadDoc, this->httpClient->getString());

  JsonDocument json;

  if (payloadDoc["lat"].is<float>()) {
    json["lat"] = payloadDoc["lat"];
  }
  if (payloadDoc["lon"].is<float>()) {
    json["lon"] = payloadDoc["lon"];
  }
  if (payloadDoc["city"].is<String>()) {
    json["city"] = payloadDoc["city"];
  }
  if (payloadDoc["country"].is<String>()) {
    json["country"] = payloadDoc["country"];
  }
  if (payloadDoc["regionName"].is<String>()) {
    json["state"] = payloadDoc["regionName"];
  }
  if (payloadDoc["zip"].is<String>()) {
    json["zip_code"] = payloadDoc["zip"];
  }
  if (payloadDoc["timezone"].is<String>()) {
    json["timezone"] = payloadDoc["timezone"];
  }
  if (payloadDoc["org"].is<String>()) {
    json["organization"] = payloadDoc["org"];
  }

  String jsonString;
  serializeJson(json, jsonString);

  this->setLastLocalizationTimeMs(millis());

  return jsonString;
}

} // namespace services
