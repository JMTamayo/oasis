#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <WiFi.h>

#include "led.h"
#include "logger.h"

namespace services {

class MqttMessage {
private:
  String subject;
  String payload;

public:
  MqttMessage(String subject, const char *payload);

  MqttMessage(String subject, String payload);

  MqttMessage(String subject, bool payload);

  MqttMessage(String subject, float payload);

  MqttMessage(String subject, int payload);

  MqttMessage(String subject, long payload);

  ~MqttMessage();

  const char *GetSubject() const;

  const char *GetPayload() const;
};

class MqttService {
private:
  const char *server;
  const unsigned int port;
  const char *user;
  const char *password;
  const unsigned long maxRetryTimeMs;

  const char *projectName;
  const char *deviceId;
  const char *topicBaseSeparator;

  logging::Logger *logger;
  peripherals::Led *led;

  PubSubClient *client;

  const char *getServer() const;

  const unsigned int getPort() const;

  const char *getUser() const;

  const char *getPassword() const;

  const unsigned long getMaxRetryTimeMs() const;

  const char *getProjectName() const;

  const char *getDeviceId() const;

  const char *getTopicBaseSeparator() const;

  logging::Logger *getLogger();

  peripherals::Led *getLed();

  PubSubClient *getClient();

  String getBaseTopic() const;

  String getTopic(String subject) const;

public:
  MqttService(const char *server, const unsigned int port, const char *user,
              const char *password, const char *projectName,
              const char *deviceId, const char *topicBaseSeparator,
              const unsigned long maxRetryTimeMs, PubSubClient *client,
              logging::Logger *logger, peripherals::Led *led);

  ~MqttService();

  bool Connect();

  bool IsConnected();

  void Publish(MqttMessage *message);

  void Subscribe(String subject);

  String GetSubject(String topic) const;

  void Loop();
};

} // namespace services

#endif // MQTT_H