#include <Arduino.h>

#include "parameters.h"
#include "secrets.h"

#include "logger.h"

#include "intercom.h"

#include "led.h"

#include "geolocation.h"
#include "mqtt.h"
#include "wifi_.h"

intercom::Intercom *comm;

WiFiClient *espClient;
PubSubClient *mqttClient;

logging::Logger *logger;

peripherals::Led *builtinLed;

services::Geolocation *geolocation;
services::WifiService *wifi;
services::MqttService *mqtt;

services::MqttMessage *message;

void resetServer() {
  logger->Info("[RESET] Resetting server");
  ESP.restart();
}

void resetController() {
  logger->Info("[RESET] Resetting controller");
  digitalWrite(RESET_CONTROLLER_PIN, LOW);
  delayMicroseconds(RESET_CONTROLLER_DELAY_MICROSECONDS);
  digitalWrite(RESET_CONTROLLER_PIN, HIGH);
}

void mqttCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr = (char *)payload;
  payloadStr.remove(length);

  logger->Info("[MQTT] Message received. Topic: " + topicStr + ". Payload: " +
               (payloadStr.length() > 0 ? payloadStr : "(empty)"));

  String subject = mqtt->GetSubject(topicStr);
  if (subject.equals(COMMAND_RESET_SERVER)) {
    resetServer();

  } else if (subject.equals(COMMAND_RESET_CONTROLLER)) {
    resetController();

  } else {
    comm->Transmit(subject, payloadStr);
  }
}

void setup() {
  logger = new logging::Logger();
  logger->Begin();

  pinMode(RESET_CONTROLLER_PIN, OUTPUT);
  digitalWrite(RESET_CONTROLLER_PIN, HIGH);

  builtinLed = new peripherals::Led(BUILTIN_LED_PIN);
  builtinLed->LightUp(!false);

  wifi = new services::WifiService(WIFI_SSID, WIFI_PASSWORD,
                                   WIFI_MAX_RETRY_TIME_MILLISECONDS, logger,
                                   builtinLed);

  espClient = new WiFiClient();
  mqttClient = new PubSubClient(*espClient);

  mqtt = new services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, PROJECT_NAME, DEVICE_ID,
      MQTT_TOPIC_BASE_SEPARATOR, MQTT_MAX_RETRY_TIME_MILLISECONDS, mqttClient,
      logger, builtinLed);
  mqttClient->setCallback(mqttCallback);

  geolocation = new services::Geolocation(
      GEOLOCATION_REQUEST_URL, GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS,
      GEOLOCATION_INTERVAL_MILLISECONDS, logger);

  comm = new intercom::Intercom(INTERCOM_TX_PIN, INTERCOM_RX_PIN,
                                INTERCOM_BAUD_RATE, INTERCOM_COMMAND_SEPARATOR,
                                logger);
  comm->Begin();

  logger->Info("[SETUP] Device setup completed");
}

void loop() {
  if (!wifi->IsConnected()) {
    wifi->Connect();

  } else if (!mqtt->IsConnected()) {
    mqtt->Connect();

    mqtt->Subscribe(COMMAND_RESET_SERVER);
    mqtt->Subscribe(COMMAND_RESET_CONTROLLER);
    mqtt->Subscribe(COMMAND_PING);
    mqtt->Subscribe(COMMAND_SET_SAMPLING_TIME);
    mqtt->Subscribe(COMMAND_GET_SAMPLING_TIME);
    mqtt->Subscribe(COMMAND_GET_PENDING_TIME_FOR_SAMPLING);
    mqtt->Subscribe(COMMAND_GET_MEASUREMENTS);

  } else {
    mqtt->Loop();

    message = comm->Loop();
    if (message != nullptr)
      mqtt->Publish(message);

    message = geolocation->Loop();
    if (message != nullptr)
      mqtt->Publish(message);
  }
}