#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "config.h"

#include "controller.h"
#include "logger.h"
#include "mqtt.h"
#include "wifi_.h"

TaskHandle_t NetworksHandlingTask;
TaskHandle_t ControlHandlingTask;

QueueHandle_t MqttPublishingEventQueue;
QueueHandle_t MqttSubscriptionEventQueue;

void mqttSubscriptionCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadString = (char *)payload;
  payloadString.remove(length);

  services::MqttMessage *mqttMessage =
      new services::MqttMessage(topicStr, payloadString);

  logging::logger->Debug("Received message from MQTT server. Topic: " +
                         topicStr + ". Payload: " + payloadString);
  xQueueSend(MqttSubscriptionEventQueue, &mqttMessage, pdMS_TO_TICKS(1000));
}

void networksHandling(void *parameter) {
  WiFiClient *espClient = new WiFiClient();
  PubSubClient *mqttClient = new PubSubClient(*espClient);

  mqttClient->setCallback(mqttSubscriptionCallback);

  services::WifiService wifi = services::WifiService(
      WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_RETRY_TIME_MILLISECONDS);

  services::MqttService mqtt = services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENT_ID,
      MQTT_MAX_RETRY_TIME_MILLISECONDS, mqttClient);

  String topicMeasurements =
      String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_MEASUREMENTS);

  String topicControl =
      String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_CONTROL) + "/#";

  while (true) {
    if (!wifi.IsConnected()) {
      wifi.Connect();

    } else if (!mqtt.IsConnected()) {
      mqtt.Connect();
      mqtt.Subscribe(topicControl.c_str());

    } else {
      mqtt.Loop();

      services::MqttMessage *mqttMessage;
      while (xQueueReceive(MqttPublishingEventQueue, &mqttMessage,
                           pdMS_TO_TICKS(10)) == pdPASS) {
        mqtt.Publish(mqttMessage);
        delete mqttMessage;
      }
    }
  }
}

void controlHandling(void *parameter) {
  control::Controller controller =
      control::Controller(MEASURING_INTERVAL_MILLISECONDS);

  String topicMeasurements =
      String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_MEASUREMENTS);

  while (true) {
    services::MqttMessage *mqttMessage;
    while (xQueueReceive(MqttSubscriptionEventQueue, &mqttMessage,
                         pdMS_TO_TICKS(10)) == pdPASS) {
      // TODO: Handle control messages

      delete mqttMessage;
    }

    controller.Loop();

    if (controller.IsMeasurementTimeReached()) {
      measurements::Measures measure = controller.Measure();

      services::MqttMessage *airTemperatureMessage = new services::MqttMessage(
          topicMeasurements + MQTT_TOPIC_MEASUREMENTS_AIR +
              MQTT_TOPIC_MEASUREMENTS_AIR_TEMPERATURE,
          measure.GetAirTemperature());
      xQueueSend(MqttPublishingEventQueue, &airTemperatureMessage,
                 pdMS_TO_TICKS(10));

      services::MqttMessage *airHumidityMessage = new services::MqttMessage(
          topicMeasurements + MQTT_TOPIC_MEASUREMENTS_AIR +
              MQTT_TOPIC_MEASUREMENTS_AIR_HUMIDITY,
          measure.GetAirHumidity());
      xQueueSend(MqttPublishingEventQueue, &airHumidityMessage,
                 pdMS_TO_TICKS(10));
    }
  }
}

void setup() {
  xTaskCreatePinnedToCore(networksHandling, "networksHandling", 10000, NULL, 1,
                          &NetworksHandlingTask, 0);
  xTaskCreatePinnedToCore(controlHandling, "controlHandling", 10000, NULL, 1,
                          &ControlHandlingTask, 1);

  MqttPublishingEventQueue = xQueueCreate(2, sizeof(services::MqttMessage *));
  MqttSubscriptionEventQueue = xQueueCreate(1, sizeof(services::MqttMessage *));
}

void loop() {}
