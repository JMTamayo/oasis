#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "parameters.h"
#include "secrets.h"

#include "controller.h"
#include "dht11.h"
#include "sen0193.h"
#include "yf_s401.h"

#include "logger.h"
#include "mqtt.h"
#include "wifi_.h"

TaskHandle_t NetworksHandlingTask;
TaskHandle_t ControlHandlingTask;

QueueHandle_t MqttPublishingEventQueue;
QueueHandle_t MqttSubscriptionEventQueue;

String TOPIC_CONTROL = String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_CONTROL);
String TOPIC_CONTROL_PUMP = String(MQTT_TOPIC_BASE) +
                            String(MQTT_TOPIC_CONTROL) +
                            String(MQTT_TOPIC_CONTROL_PUMP);

String TOPIC_MEASUREMENTS_PUMP = String(MQTT_TOPIC_BASE) +
                                 String(MQTT_TOPIC_MEASUREMENTS) +
                                 String(MQTT_TOPIC_MEASUREMENTS_PUMP);
String TOPIC_MEASUREMENTS_AIR = String(MQTT_TOPIC_BASE) +
                                String(MQTT_TOPIC_MEASUREMENTS) +
                                String(MQTT_TOPIC_MEASUREMENTS_AIR);
String TOPIC_MEASUREMENTS_SOIL = String(MQTT_TOPIC_BASE) +
                                 String(MQTT_TOPIC_MEASUREMENTS) +
                                 String(MQTT_TOPIC_MEASUREMENTS_SOIL);
String TOPIC_MEASUREMENTS_WATER = String(MQTT_TOPIC_BASE) +
                                  String(MQTT_TOPIC_MEASUREMENTS) +
                                  String(MQTT_TOPIC_MEASUREMENTS_WATER);

void mqttSubscriptionCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadString = (char *)payload;
  payloadString.remove(length);

  services::MqttMessage *mqttMessage =
      new services::MqttMessage(topicStr, payloadString);

  if (topicStr == TOPIC_CONTROL_PUMP &&
      (payloadString == "0" || payloadString == "1")) {
    logging::logger->Debug("Received message from MQTT server. Topic: " +
                           topicStr + ". Payload: " + payloadString);
    xQueueSend(MqttSubscriptionEventQueue, &mqttMessage, pdMS_TO_TICKS(200));

  } else {
    logging::logger->Error(
        "Received invalid message from MQTT server. Topic: " + topicStr +
        ". Payload: " + payloadString);
  }
}

void networksHandling(void *parameter) {
  WiFiClient *espClient = new WiFiClient();
  PubSubClient *mqttClient = new PubSubClient(*espClient);

  mqttClient->setCallback(mqttSubscriptionCallback);

  services::WifiService *wifi = new services::WifiService(
      WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_RETRY_TIME_MILLISECONDS);

  services::MqttService *mqtt = new services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENT_ID,
      MQTT_MAX_RETRY_TIME_MILLISECONDS, mqttClient);

  services::MqttMessage *mqttMessage;

  while (true) {
    if (!wifi->IsConnected()) {
      wifi->Connect();

    } else if (!mqtt->IsConnected()) {
      mqtt->Connect();
      mqtt->Subscribe((TOPIC_CONTROL + "/#").c_str());

    } else {
      mqtt->Loop();

      while (xQueueReceive(MqttPublishingEventQueue, &mqttMessage,
                           pdMS_TO_TICKS(10)) == pdPASS) {
        mqtt->Publish(mqttMessage);
        delete mqttMessage;
      }
    }
  }
}

void mqttPublishMeasurementsCallback(control::Measures *measure) {
  float airTemperature = measure->GetAirProperties().GetTemperature();
  if (!isnan(airTemperature)) {
    services::MqttMessage *airTemperatureMessage = new services::MqttMessage(
        TOPIC_MEASUREMENTS_AIR + MQTT_TOPIC_MEASUREMENTS_AIR_TEMPERATURE,
        airTemperature);
    xQueueSend(MqttPublishingEventQueue, &airTemperatureMessage,
               pdMS_TO_TICKS(10));
  }

  float airRelativeHumidity = measure->GetAirProperties().GetRelativeHumidity();
  if (!isnan(airRelativeHumidity)) {
    services::MqttMessage *airRelativeHumidityMessage =
        new services::MqttMessage(
            TOPIC_MEASUREMENTS_AIR +
                MQTT_TOPIC_MEASUREMENTS_AIR_RELATIVE_HUMIDITY,
            airRelativeHumidity);
    xQueueSend(MqttPublishingEventQueue, &airRelativeHumidityMessage,
               pdMS_TO_TICKS(10));
  }

  float waterFlowRate = measure->GetWaterFlowRate().GetFlowRate();
  if (!isnan(waterFlowRate)) {
    services::MqttMessage *waterFlowRateMessage = new services::MqttMessage(
        TOPIC_MEASUREMENTS_WATER + MQTT_TOPIC_MEASUREMENTS_WATER_FLOW_RATE,
        waterFlowRate);
    xQueueSend(MqttPublishingEventQueue, &waterFlowRateMessage,
               pdMS_TO_TICKS(10));
  }

  String soilMoistureLevel = measure->GetSoilMoisture().GetLevelString();
  if (!soilMoistureLevel.isEmpty()) {
    services::MqttMessage *soilMoistureLevelMessage = new services::MqttMessage(
        TOPIC_MEASUREMENTS_SOIL + MQTT_TOPIC_MEASUREMENTS_SOIL_MOISTURE_LEVEL,
        soilMoistureLevel);
    xQueueSend(MqttPublishingEventQueue, &soilMoistureLevelMessage,
               pdMS_TO_TICKS(10));
  }

  float soilMoisture = measure->GetSoilMoisture().GetMoisture();
  if (!isnan(soilMoisture)) {
    services::MqttMessage *soilMoistureMessage = new services::MqttMessage(
        TOPIC_MEASUREMENTS_SOIL + MQTT_TOPIC_MEASUREMENTS_SOIL_MOISTURE,
        soilMoisture);
    xQueueSend(MqttPublishingEventQueue, &soilMoistureMessage,
               pdMS_TO_TICKS(10));
  }

  services::MqttMessage *pumpStateMessage = new services::MqttMessage(
      TOPIC_MEASUREMENTS_PUMP + MQTT_TOPIC_MEASUREMENTS_PUMP_STATE,
      measure->GetPumpState());
  xQueueSend(MqttPublishingEventQueue, &pumpStateMessage, pdMS_TO_TICKS(10));
}

void controlHandling(void *parameter) {
  peripherals::Dht11 *dht11 = new peripherals::Dht11(DHT11_PIN);
  peripherals::YfS401 *yfS401 =
      new peripherals::YfS401(YF_S401_PIN, YF_S401_VOLUME_PER_PULSE);
  peripherals::Sen0193 *sen0193 = new peripherals::Sen0193(
      SEN0193_PIN, SEN0193_WATER_VALUE, SEN0193_AIR_VALUE);

  control::Controller *controller = new control::Controller(
      MEASURING_INTERVAL_MILLISECONDS, dht11, yfS401, sen0193);

  services::MqttMessage *mqttMessage;
  controller->Begin();

  while (true) {
    controller->Loop();

    if (xQueueReceive(MqttSubscriptionEventQueue, &mqttMessage,
                      pdMS_TO_TICKS(10)) == pdPASS) {
      if (String(mqttMessage->GetTopic()) == TOPIC_CONTROL_PUMP) {
        bool pumpState = String(mqttMessage->GetPayload()) == "1";
        logging::logger->Debug("Processing pump control request. Operation: " +
                               String(pumpState ? "ON" : "OFF"));
        controller->StartPump(pumpState);
      }
      delete mqttMessage;
    }

    if (controller->IsMeasurementTimeReached()) {
      control::Measures measure = controller->Measure();
      mqttPublishMeasurementsCallback(&measure);
    }
  }
}

void setup() {
  xTaskCreatePinnedToCore(networksHandling, "networksHandling", 10000, NULL, 1,
                          &NetworksHandlingTask, 0);
  xTaskCreatePinnedToCore(controlHandling, "controlHandling", 10000, NULL, 1,
                          &ControlHandlingTask, 1);

  MqttPublishingEventQueue = xQueueCreate(4, sizeof(services::MqttMessage *));
  MqttSubscriptionEventQueue = xQueueCreate(1, sizeof(services::MqttMessage *));
}

void loop() {}
