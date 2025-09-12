#include <PubSubClient.h>
#include <WiFi.h>

#include "logger.h"
#include "mqtt.h"
#include "sample.h"

namespace services {

const char *MqttService::getClientId() const { return this->clientId; }

const char *MqttService::getServer() const { return this->server; }

const unsigned int MqttService::getPort() const { return this->port; }

const char *MqttService::getUser() const { return this->user; }

const char *MqttService::getPassword() const { return this->password; }

const unsigned long MqttService::getMaxRetryTimeMs() const {
  return this->maxRetryTimeMs;
}

const char *MqttService::getTopicBase() const { return this->topicBase; }

const char *MqttService::getTopicMeasurements() const {
  return this->topicMeasurements;
}

const char *MqttService::getTopicMeasurementsAir() const {
  return this->topicMeasurementsAir;
}

const char *MqttService::getTopicMeasurementsAirTemperature() const {
  return this->topicMeasurementsAirTemperature;
}

const char *MqttService::getTopicMeasurementsAirHumidity() const {
  return this->topicMeasurementsAirHumidity;
}

PubSubClient *MqttService::getClient() { return this->client; }

MqttService::MqttService(const char *server, const unsigned int port,
                         const char *user, const char *password,
                         const char *clientId,
                         const unsigned long maxRetryTimeMs,
                         const char *topicBase, const char *topicMeasurements,
                         const char *topicMeasurementsAir,
                         const char *topicMeasurementsAirTemperature,
                         const char *topicMeasurementsAirHumidity,
                         PubSubClient *client)
    : server(server), port(port), user(user), password(password),
      clientId(clientId), maxRetryTimeMs(maxRetryTimeMs), topicBase(topicBase),
      topicMeasurements(topicMeasurements),
      topicMeasurementsAir(topicMeasurementsAir),
      topicMeasurementsAirTemperature(topicMeasurementsAirTemperature),
      topicMeasurementsAirHumidity(topicMeasurementsAirHumidity),
      client(client) {
  client->setServer(this->getServer(), this->getPort());
}

MqttService::~MqttService() { delete this->client; }

bool MqttService::IsConnected() { return this->getClient()->connected(); }

void MqttService::Connect() {
  logging::logger->Info("Connecting to MQTT server: " +
                        String(this->getServer()));

  unsigned long startTimeMs = millis();
  unsigned long retryTimeMs = 0;

  while (!this->IsConnected()) {
    if (this->getClient()->connect(this->getClientId(), this->getUser(),
                                   this->getPassword())) {
      break;
    }

    if (retryTimeMs >= this->getMaxRetryTimeMs()) {
      logging::logger->Error("Connection to MQTT server failed");
      return;
    }

    retryTimeMs = millis() - startTimeMs;
  }

  logging::logger->Info(
      "MQTT serverconnection successfully established. Client ID " +
      String(this->getClientId()));
}

void MqttService::Loop() { this->getClient()->loop(); }

void MqttService::SendSample(sampling::Sample *sample) {
  const String topicBase = String(this->getTopicBase()) +
                           String(this->getTopicMeasurements()) +
                           String(this->getTopicMeasurementsAir());

  this->getClient()->publish(
      (topicBase + String(this->getTopicMeasurementsAirTemperature())).c_str(),
      String(sample->GetAirTemperature()).c_str());

  this->getClient()->publish(
      (topicBase + String(this->getTopicMeasurementsAirHumidity())).c_str(),
      String(sample->GetAirHumidity()).c_str());
}

} // namespace services