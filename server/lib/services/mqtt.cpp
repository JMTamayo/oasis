#include "mqtt.h"

namespace services {

const char *MqttMessage::GetSubject() const { return this->subject.c_str(); }

const char *MqttMessage::GetPayload() const { return this->payload.c_str(); }

MqttMessage::MqttMessage(String subject, const char *payload)
    : subject(subject), payload(String(payload)) {}

MqttMessage::MqttMessage(String subject, String payload)
    : subject(subject), payload(payload) {}

MqttMessage::MqttMessage(String subject, bool payload)
    : subject(subject), payload(payload ? "1" : "0") {}

MqttMessage::MqttMessage(String subject, float payload)
    : subject(subject), payload(String(payload)) {}

MqttMessage::MqttMessage(String subject, int payload)
    : subject(subject), payload(String(payload)) {}

MqttMessage::MqttMessage(String subject, long payload)
    : subject(subject), payload(String(payload)) {}

MqttMessage::~MqttMessage() {}

const char *MqttService::getServer() const { return this->server; }

const unsigned int MqttService::getPort() const { return this->port; }

const char *MqttService::getUser() const { return this->user; }

const char *MqttService::getPassword() const { return this->password; }

const char *MqttService::getProjectName() const { return this->projectName; }

const char *MqttService::getDeviceId() const { return this->deviceId; }

const char *MqttService::getTopicBaseSeparator() const {
  return this->topicBaseSeparator;
}

const unsigned long MqttService::getMaxRetryTimeMs() const {
  return this->maxRetryTimeMs;
}

logging::Logger *MqttService::getLogger() { return this->logger; }

peripherals::Led *MqttService::getLed() { return this->led; }

PubSubClient *MqttService::getClient() { return this->client; }

String MqttService::getBaseTopic() const {
  return String(this->getTopicBaseSeparator()) +
         String(this->getProjectName()) +
         String(this->getTopicBaseSeparator()) + String(this->getDeviceId()) +
         String(this->getTopicBaseSeparator());
}

String MqttService::getTopic(String subject) const {
  return this->getBaseTopic() + subject;
}

MqttService::MqttService(const char *server, const unsigned int port,
                         const char *user, const char *password,
                         const char *projectName, const char *deviceId,
                         const char *topicBaseSeparator,
                         const unsigned long maxRetryTimeMs,
                         PubSubClient *client, logging::Logger *logger,
                         peripherals::Led *led)
    : server(server), port(port), user(user), password(password),
      projectName(projectName), deviceId(deviceId),
      topicBaseSeparator(topicBaseSeparator), maxRetryTimeMs(maxRetryTimeMs),
      client(client), logger(logger), led(led) {
  client->setServer(this->getServer(), this->getPort());
}

MqttService::~MqttService() { delete this->client; }

bool MqttService::Connect() {
  unsigned long startTimeMs = millis();
  unsigned long retryTimeMs = 0;

  while (!this->IsConnected()) {
    if (this->getClient()->connect(this->getDeviceId(), this->getUser(),
                                   this->getPassword())) {
      break;
    }

    if (retryTimeMs >= this->getMaxRetryTimeMs()) {
      this->getLogger()->Error(
          "[MQTT] Connection to MQTT server failed. Server: " +
          String(this->getServer()));
      return false;
    }

    retryTimeMs = millis() - startTimeMs;
  }

  this->getLogger()->Info(
      "[MQTT] Connected to MQTT server. Server: " + String(this->getServer()) +
      ". Client ID: " + String(this->getDeviceId()));

  return true;
}

bool MqttService::IsConnected() {
  bool connected = this->getClient()->connected();
  this->getLed()->LightUp(!connected);
  return connected;
}

void MqttService::Publish(MqttMessage *message) {
  if (message == nullptr)
    return;

  String topic = this->getTopic(message->GetSubject());
  bool published =
      this->getClient()->publish(topic.c_str(), message->GetPayload());
  if (published) {
    this->getLogger()->Debug("[MQTT] Data published. Topic: " + topic +
                             ". Payload: " + message->GetPayload());
  } else {
    this->getLogger()->Error("[MQTT] Failed to publish the data. Topic: " +
                             topic + ". Payload: " + message->GetPayload());
  }
}

void MqttService::Subscribe(String subject) {
  String topic = this->getTopic(subject);
  bool subscribed = this->getClient()->subscribe(topic.c_str());
  if (subscribed) {
    this->getLogger()->Debug("[MQTT] Subscribed to the topic: " + topic);
  } else {
    this->getLogger()->Error("[MQTT] Failed to subscribe to the topic: " +
                             topic);
  }
}

String MqttService::GetSubject(String topic) const {
  String separator = String(this->getTopicBaseSeparator());
  String expectedPrefix = separator + String(this->getProjectName()) +
                          separator + String(this->getDeviceId()) + separator;

  if (topic.startsWith(expectedPrefix))
    return topic.substring(expectedPrefix.length());
  return String("");
}

void MqttService::Loop() { this->getClient()->loop(); }

} // namespace services