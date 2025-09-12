#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>

#include "measurement.h"

namespace services {

class MqttService {
private:
  const char *server;
  const unsigned int port;
  const char *user;
  const char *password;
  const char *clientId;
  const unsigned long maxRetryTimeMs;

  const char *topicBase;

  const char *topicMeasurements;
  const char *topicMeasurementsAir;
  const char *topicMeasurementsAirTemperature;
  const char *topicMeasurementsAirHumidity;

  PubSubClient *client;

  const char *getServer() const;

  const unsigned int getPort() const;

  const char *getUser() const;

  const char *getPassword() const;

  const char *getClientId() const;

  const unsigned long getMaxRetryTimeMs() const;

  const char *getTopicBase() const;

  const char *getTopicMeasurements() const;

  const char *getTopicMeasurementsAir() const;

  const char *getTopicMeasurementsAirTemperature() const;

  const char *getTopicMeasurementsAirHumidity() const;

  PubSubClient *getClient();

public:
  MqttService(const char *server, const unsigned int port, const char *user,
              const char *password, const char *clientId,
              const unsigned long maxRetryTimeMs, const char *topicBase,
              const char *topicMeasurements, const char *topicMeasurementsAir,
              const char *topicMeasurementsAirTemperature,
              const char *topicMeasurementsAirHumidity, PubSubClient *client);

  ~MqttService();

  bool IsConnected();

  void Connect();

  void Loop();

  void SendMeasurement(measuring::Measure *measure);
};

} // namespace services

#endif // MQTT_H