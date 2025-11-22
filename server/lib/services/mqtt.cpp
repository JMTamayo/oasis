#include "mqtt.h"

namespace services {

const char *MqttMessage::GetTopic() const { return this->topic.c_str(); }

const char *MqttMessage::GetPayload() const { return this->payload.c_str(); }

MqttMessage::MqttMessage(String topic, const char *payload)
    : topic(topic), payload(String(payload)) {}

MqttMessage::MqttMessage(String topic, String payload)
    : topic(topic), payload(payload) {}

MqttMessage::MqttMessage(String topic, bool payload)
    : topic(topic), payload(payload ? "1" : "0") {}

MqttMessage::MqttMessage(String topic, float payload)
    : topic(topic), payload(String(payload)) {}

MqttMessage::MqttMessage(String topic, int payload)
    : topic(topic), payload(String(payload)) {}

MqttMessage::MqttMessage(String topic, long payload)
    : topic(topic), payload(String(payload)) {}

MqttMessage::~MqttMessage() {}

const char *MqttService::getClientId() const { return this->clientId; }

const char *MqttService::getServer() const { return this->server; }

const unsigned int MqttService::getPort() const { return this->port; }

const char *MqttService::getUser() const { return this->user; }

const char *MqttService::getPassword() const { return this->password; }

const unsigned long MqttService::getMaxRetryTimeMs() const {
  return this->maxRetryTimeMs;
}

logging::Logger *MqttService::getLogger() { return this->logger; }

peripherals::Led *MqttService::getLed() { return this->led; }

PubSubClient *MqttService::getClient() { return this->client; }

MqttService::MqttService(const char *server, const unsigned int port,
                         const char *user, const char *password,
                         const char *clientId,
                         const unsigned long maxRetryTimeMs,
                         PubSubClient *client, logging::Logger *logger,
                         peripherals::Led *led)
    : server(server), port(port), user(user), password(password),
      clientId(clientId), maxRetryTimeMs(maxRetryTimeMs), client(client),
      logger(logger), led(led) {
  client->setServer(this->getServer(), this->getPort());
}

MqttService::~MqttService() { delete this->client; }

bool MqttService::Connect() {
  unsigned long startTimeMs = millis();
  unsigned long retryTimeMs = 0;

  while (!this->IsConnected()) {
    if (this->getClient()->connect(this->getClientId(), this->getUser(),
                                   this->getPassword())) {
      break;
    }

    if (retryTimeMs >= this->getMaxRetryTimeMs()) {
      this->getLogger()->Error(
          "[MQTT] Connection to MQTT server failed. Server: " +
          String(this->getServer()));
      return false;
    }

    vTaskDelay(pdMS_TO_TICKS(100));
    retryTimeMs = millis() - startTimeMs;
  }

  this->getLogger()->Info(
      "[MQTT] Connected to MQTT server. Server: " + String(this->getServer()) +
      ". Client ID: " + String(this->getClientId()));

  return true;
}

bool MqttService::IsConnected() {
  bool connected = this->getClient()->connected();
  this->getLed()->LightUp(!connected);
  return connected;
}

void MqttService::Publish(MqttMessage *message) {
  this->getLogger()->Debug(
      "[MQTT] Data published. Topic: " + String(message->GetTopic()) +
      ". Payload: " + String(message->GetPayload()));
  this->getClient()->publish(message->GetTopic(), message->GetPayload());
}

void MqttService::Subscribe(String topic) {
  this->getClient()->subscribe(topic.c_str());
  this->getLogger()->Debug("[MQTT] Subscribed to the topic: " + topic);
}

void MqttService::Loop() { this->getClient()->loop(); }

} // namespace services