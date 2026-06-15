#include "geolocation.h"

namespace services {

HTTPClient *Geolocation::getHttpClient() const { return this->httpClient; }

const char *Geolocation::getMqttSubjectGeolocation() const {
  return this->mqttSubjectGeolocation;
}

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
                         const char *mqttSubjectGeolocation,
                         logging::Logger *logger)
    : intervalMs(intervalMs), lastLocalizationTimeMs(millis() - intervalMs),
      mqttSubjectGeolocation(mqttSubjectGeolocation), logger(logger) {
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
    this->getLogger()->Error("GEOLOCATION",
                             "Failed to get geolocation. Status code: " +
                                 String(statusCode));
    return emptyString;
  }

  String responseBody = this->getHttpClient()->getString();
  return responseBody;
}

services::MqttMessage *Geolocation::Loop() {
  if (this->IsLocalizationTimeReached()) {
    String json = this->Localize();
    if (!json.isEmpty()) {
      this->setLastLocalizationTimeMs(millis());
      return new services::MqttMessage(this->getMqttSubjectGeolocation(), json);
    }
  }

  return nullptr;
}

} // namespace services