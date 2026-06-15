#ifndef MQTT_H
#define MQTT_H

#include <PubSubClient.h>
#include <WiFi.h>

#include "logger.h"

namespace services {

/**
  Mqtt message class.

  This class is used to represent a MQTT message that can be published or
  subscribed to. It contains methods to connect to the MQTT server, verify the
  connection and publish and subscribe to topics.
 */
class MqttMessage {
private:
  String subject; // The subject of the MQTT message.
  String payload; // The payload of the MQTT message.

public:
  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as a constant character array.
   */
  MqttMessage(String subject, const char *payload);

  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as a string.
   */
  MqttMessage(String subject, String payload);

  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as a boolean.
   */
  MqttMessage(String subject, bool payload);

  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as a float.
   */
  MqttMessage(String subject, float payload);

  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as an integer.
   */
  MqttMessage(String subject, int payload);

  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as a long integer.
   */
  MqttMessage(String subject, long payload);

  /**
    Constructor.

    Parameters:
      subject - The subject of the MQTT message.
      payload - The payload of the MQTT message as an unsigned long integer.
   */
  MqttMessage(String subject, unsigned long payload);

  /**
    Destructor.
   */
  ~MqttMessage();

  /**
    Get the subject.

    Returns:
      The subject of the MQTT message.
   */
  const char *GetSubject() const;

  /**
    Get the payload.

    Returns:
      The payload of the MQTT message.
   */
  const char *GetPayload() const;
};

/**
  Mqtt service class.

  Utilities to manage the MQTT connection in the context of the application. It
  contains the methods to connect to the MQTT server, verify the connection and
  publish and subscribe to topics.
 */
class MqttService {
private:
  const char *server;                 // The MQTT server address.
  const unsigned int port;            // The MQTT server port.
  const char *user;                   // The MQTT username.
  const char *password;               // The MQTT password.
  const unsigned long maxRetryTimeMs; // The maximum retry time to connect to
                                      // the MQTT server in milliseconds.

  const char *projectName;        // The project name.
  const char *deviceId;           // The unique device ID.
  const char *topicBaseSeparator; // The separator for the topics.

  logging::Logger *logger; // The logger to use to log important information
                           // about the MQTT connection.

  PubSubClient *client; // The MQTT client.

  /**
    Get the MQTT server address.

    Returns:
      The MQTT server address.
   */
  const char *getServer() const;

  /**
    Get the MQTT server port.

    Returns:
      The MQTT server port.
   */
  const unsigned int getPort() const;

  /**
    Get the MQTT username.

    Returns:
      The MQTT username.
   */
  const char *getUser() const;

  /**
    Get the MQTT password.

    Returns:
      The MQTT password.
   */
  const char *getPassword() const;

  /**
    Get the maximum retry time.

    Returns:
      The maximum retry time to connect to the MQTT server in milliseconds.
   */
  const unsigned long getMaxRetryTimeMs() const;

  /**
    Get the project name.

    Returns:
      The project name.
   */
  const char *getProjectName() const;

  /**
    Get the unique device ID.

    Returns:
      The unique device ID.
   */
  const char *getDeviceId() const;

  /**
    Get the separator for the topics.

    Returns:
      The separator for the topics.
   */
  const char *getTopicBaseSeparator() const;

  /**
    Get the logger.

    Returns:
      The logger to use to log important information about the MQTT connection.
   */
  logging::Logger *getLogger();

  /**
    Get the MQTT client.

    Returns:
      The MQTT client.
   */
  PubSubClient *getClient();

  /**
    Get the base topic.

    Returns:
      The base topic.
   */
  String getBaseTopic() const;

  /**
    Get the topic for a given subject.

    Parameters:
      subject - The subject of the topic.

    Returns:
      The topic for the given subject, which is the base topic plus the subject.
   */
  String getTopic(String subject) const;

public:
  /**
    Constructor.

    Parameters:
      server - The MQTT server address.
      port - The MQTT server port.
      user - The MQTT username.
      password - The MQTT password.
      projectName - The name of the project.
      deviceId - The unique ID of the device.
      topicBaseSeparator - The separator for the topics.
      maxRetryTimeMs - The maximum retry time to connect to the MQTT server in
        milliseconds.
      client - The MQTT client.
      logger - The logger to use to log important information about the MQTT
        connection.
   */
  MqttService(const char *server, const unsigned int port, const char *user,
              const char *password, const char *projectName,
              const char *deviceId, const char *topicBaseSeparator,
              const unsigned long maxRetryTimeMs, PubSubClient *client,
              logging::Logger *logger);

  /**
    Destructor.
   */
  ~MqttService();

  /**
    Connect to the MQTT server.

    Returns:
      True if the connection is successful, false otherwise.
   */
  bool Connect();

  /**
    Check if the MQTT server is connected.

    Returns:
      True if the MQTT server is connected, false otherwise.
   */
  bool IsConnected();

  /**
    Publish a message to the MQTT server.

    Parameters:
      message - The message to publish.
   */
  void Publish(MqttMessage *message);

  /**
    Subscribe to a topic.

    Parameters:
      subject - The subject of the topic to subscribe to.
   */
  void Subscribe(String subject);

  /**
    Get the subject of a topic.

    Parameters:
      topic - The topic to get the subject from.

    Returns:
      The subject of the topic.
   */
  String GetSubject(String topic) const;

  /**
    Loop the MQTT client.

    This method is used to loop the MQTT client and handle the incoming
    messages.
   */
  void Loop();
};

} // namespace services

#endif // MQTT_H