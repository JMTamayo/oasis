#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "config.h"

#include "collector.h"
#include "logger.h"
#include "mqtt.h"
#include "sample.h"
#include "wifi_.h"

TaskHandle_t NetworkManagementTask;
TaskHandle_t SamplingTask;

QueueHandle_t SamplingQueue;

void NetworkHandling(void *parameter) {
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

  sampling::Sample *sample;

  while (true) {
    if (!wifi.IsConnected()) {
      wifi.Connect();

    } else if (!mqtt.IsConnected()) {
      mqtt.Connect();

    } else {
      mqtt.Loop();

      while (xQueueReceive(SamplingQueue, &sample, pdMS_TO_TICKS(10)) ==
             pdPASS) {
        mqtt.SendSample(sample);
      }
    }
  }
}

void Sampling(void *parameter) {
  sampling::Collector collector =
      sampling::Collector(SAMPLING_INTERVAL_MILLISECONDS);

  sampling::Sample *sample;

  while (true) {
    if (collector.IsSamplingTimeReached()) {
      sample = new sampling::Sample(collector.Collect());
      xQueueSend(SamplingQueue, &sample, pdMS_TO_TICKS(10));
    }
  }
}

void setup() {
  xTaskCreatePinnedToCore(NetworkHandling, "NetworkHandling", 10000, NULL, 1,
                          &NetworkManagementTask, 0);
  xTaskCreatePinnedToCore(Sampling, "Sampling", 10000, NULL, 1, &SamplingTask,
                          1);

  SamplingQueue = xQueueCreate(1, sizeof(sampling::Sample *));

  logging::logger->Initialize();
}

void loop() {}
