#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "config.h"

#include "controller.h"
#include "logger.h"
#include "measurement.h"
#include "mqtt.h"
#include "wifi_.h"

TaskHandle_t NetworksHandlingTask;
TaskHandle_t ControlHandlingTask;

QueueHandle_t MeasurementsQueue;

void NetworksHandling(void *parameter) {
  WiFiClient *espClient = new WiFiClient();
  PubSubClient *mqttClient = new PubSubClient(*espClient);

  services::WifiService wifi = services::WifiService(
      WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_RETRY_TIME_MILLISECONDS);

  services::MqttService mqtt = services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENT_ID,
      MQTT_MAX_RETRY_TIME_MILLISECONDS, MQTT_TOPIC_BASE,
      MQTT_TOPIC_MEASUREMENTS, MQTT_TOPIC_MEASUREMENTS_AIR,
      MQTT_TOPIC_MEASUREMENTS_AIR_TEMPERATURE,
      MQTT_TOPIC_MEASUREMENTS_AIR_HUMIDITY, mqttClient);

  measuring::Measure *measure;

  while (true) {
    if (!wifi.IsConnected()) {
      wifi.Connect();

    } else if (!mqtt.IsConnected()) {
      mqtt.Connect();

    } else {
      mqtt.Loop();

      while (xQueueReceive(MeasurementsQueue, &measure, pdMS_TO_TICKS(10)) ==
             pdPASS) {
        mqtt.SendMeasurement(measure);
      }
    }
  }
}

void ControlHandling(void *parameter) {
  control::Controller controller =
      control::Controller(MEASURING_INTERVAL_MILLISECONDS);

  measuring::Measure *measure;

  while (true) {
    controller.Loop();

    if (controller.IsMeasurementTimeReached()) {
      measure = new measuring::Measure(controller.Measure());
      xQueueSend(MeasurementsQueue, &measure, pdMS_TO_TICKS(10));
    }
  }
}

void setup() {
  xTaskCreatePinnedToCore(NetworksHandling, "NetworksHandling", 10000, NULL, 1,
                          &NetworksHandlingTask, 0);
  xTaskCreatePinnedToCore(ControlHandling, "ControlHandling", 10000, NULL, 1,
                          &ControlHandlingTask, 1);

  MeasurementsQueue = xQueueCreate(1, sizeof(measuring::Measure *));
}

void loop() {}
