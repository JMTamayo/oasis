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

logging::Logger *Geolocation::getLogger() { return this->logger; }

Geolocation::Geolocation(const char *requestUrl,
                         const unsigned long requestTimeoutMs,
                         const unsigned long intervalMs,
                         logging::Logger *logger)
    : intervalMs(intervalMs), lastLocalizationTimeMs(millis() - intervalMs),
      logger(logger) {
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

  int statusCode = this->getHttpClient()->GET();
  if (statusCode != HTTP_CODE_OK) {
    this->getLogger()->Error(
        "[GEOLOCATION] Failed to get geolocation. Status code: " +
        String(statusCode));
    return emptyString;
  }

  String responseBody = this->getHttpClient()->getString();
  if (responseBody.isEmpty()) {
    this->getLogger()->Error(
        "[GEOLOCATION] Failed to get geolocation. Response body is empty");
    return emptyString;

  } else if (responseBody == "null") {
    this->getLogger()->Error(
        "[GEOLOCATION] Failed to get geolocation. Response body is null");
    return emptyString;

  } else {
    this->getLogger()->Debug("[GEOLOCATION] Geolocation service response: " +
                             responseBody);
  }

  JsonDocument payloadDoc;
  deserializeJson(payloadDoc, responseBody);

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
  if (payloadDoc["isp"].is<String>()) {
    json["isp"] = payloadDoc["isp"];
  }
  if (payloadDoc["query"].is<String>()) {
    json["ip"] = payloadDoc["query"];
  }

  String jsonString;
  serializeJson(json, jsonString);

  this->setLastLocalizationTimeMs(millis());

  return jsonString;
}

services::MqttMessage *Geolocation::Loop() {
  if (this->IsLocalizationTimeReached()) {
    String json = this->Localize();
    if (!json.isEmpty()) {
      return new services::MqttMessage("geolocation", json);
    }
  }

  return nullptr;
}

} // namespace services