#include <Arduino.h>

#include "parameters.h"
#include "secrets.h"

#include "led.h"

#include "logger.h"
#include "mqtt.h"
#include "wifi_.h"

String MQTT_TOPIC_BASE = "/" + String(PROJECT_NAME) + "/" + String(DEVICE_ID);

String MQTT_TOPIC_RESET_CONTROLLER =
    MQTT_TOPIC_BASE + "/" + String(COMMAND_RESET_CONTROLLER);
String MQTT_TOPIC_RESET_SERVER =
    MQTT_TOPIC_BASE + "/" + String(COMMAND_RESET_SERVER);

WiFiClient *espClient;
PubSubClient *mqttClient;

peripherals::Led *builtinLed;

logging::Logger *logger;
services::WifiService *wifi;
services::MqttService *mqtt;

void mqttCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr = (char *)payload;
  payloadStr.remove(length);

  logger->Info("[MQTT] Message received. Topic: " + topicStr + ". Payload: " +
               (payloadStr.length() > 0 ? payloadStr : "(empty)"));

  if (topicStr.equals(MQTT_TOPIC_RESET_SERVER)) {
    ESP.restart();

  } else {
    logger->Warning("[MQTT] Received message is not supported. Topic: " +
                    topicStr);
  }
}

void setup() {
  logger = new logging::Logger();
  logger->Begin();
  logger->Info("[SETUP] Device setup started");

  builtinLed = new peripherals::Led(BUILTIN_LED_PIN);
  builtinLed->LightUp(!false);

  wifi = new services::WifiService(WIFI_SSID, WIFI_PASSWORD,
                                   WIFI_MAX_RETRY_TIME_MILLISECONDS, logger,
                                   builtinLed);

  espClient = new WiFiClient();
  mqttClient = new PubSubClient(*espClient);

  mqtt = new services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, DEVICE_ID,
      MQTT_MAX_RETRY_TIME_MILLISECONDS, mqttClient, logger, builtinLed);
  mqttClient->setCallback(mqttCallback);

  logger->Info("[SETUP] Device setup completed");
}

void loop() {
  if (!wifi->IsConnected()) {
    wifi->Connect();

  } else if (!mqtt->IsConnected()) {
    mqtt->Connect();
    mqtt->Subscribe(MQTT_TOPIC_RESET_CONTROLLER);
    mqtt->Subscribe(MQTT_TOPIC_RESET_SERVER);

  } else {
    mqtt->Loop();
  }
}