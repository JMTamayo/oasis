#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <vector>

#include "config.h"

#include "controller.h"
#include "logger.h"
#include "mqtt.h"
#include "wifi_.h"

TaskHandle_t NetworksHandlingTask;
TaskHandle_t ControlHandlingTask;

QueueHandle_t MqttMeasuresEventQueue;

void mqttSubscriptionCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadString = (char *)payload;
  payloadString.remove(length);

  logging::logger->Info("Topic: " + topicStr + ". Payload: " + payloadString);
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

  std::vector<services::MqttMessage> *measures;

  while (true) {
    if (!wifi.IsConnected()) {
      wifi.Connect();

    } else if (!mqtt.IsConnected()) {
      mqtt.Connect();
      mqtt.Subscribe(topicControl.c_str());

    } else {
      mqtt.Loop();

      while (xQueueReceive(MqttMeasuresEventQueue, &measures,
                           pdMS_TO_TICKS(10)) == pdPASS) {
        if (measures) {
          for (const auto &measure : *measures) {
            mqtt.Publish(measure);
          }
          delete measures;
        }
      }
    }
  }
}

void controlHandling(void *parameter) {
  control::Controller controller =
      control::Controller(MEASURING_INTERVAL_MILLISECONDS);

  String topicMeasurements =
      String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_MEASUREMENTS);

  std::vector<services::MqttMessage> *measures;

  while (true) {
    controller.Loop();

    // TODO: Handle new subscription messages

    if (controller.IsMeasurementTimeReached()) {
      measurements::Measures measure = controller.Measure();

      measures = new std::vector<services::MqttMessage>();

      services::MqttMessage airTemperatureMessage = services::MqttMessage(
          topicMeasurements + MQTT_TOPIC_MEASUREMENTS_AIR +
              MQTT_TOPIC_MEASUREMENTS_AIR_TEMPERATURE,
          measure.GetAirTemperature());
      measures->push_back(airTemperatureMessage);

      services::MqttMessage airHumidityMessage = services::MqttMessage(
          topicMeasurements + MQTT_TOPIC_MEASUREMENTS_AIR +
              MQTT_TOPIC_MEASUREMENTS_AIR_HUMIDITY,
          measure.GetAirHumidity());
      measures->push_back(airHumidityMessage);

      xQueueSend(MqttMeasuresEventQueue, &measures, pdMS_TO_TICKS(10));
    }
  }
}

void setup() {
  xTaskCreatePinnedToCore(networksHandling, "networksHandling", 10000, NULL, 1,
                          &NetworksHandlingTask, 0);
  xTaskCreatePinnedToCore(controlHandling, "controlHandling", 10000, NULL, 1,
                          &ControlHandlingTask, 1);

  MqttMeasuresEventQueue =
      xQueueCreate(1, sizeof(std::vector<services::MqttMessage> *));
}

void loop() {}
