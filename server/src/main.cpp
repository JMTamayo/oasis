#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>

#include "parameters.h"
#include "secrets.h"

#include "logger.h"

#include "controller.h"
#include "measurements.h"

#include "dht22.h"
#include "lcd1602_i2c.h"
#include "led.h"
#include "sen0193.h"

#include "geolocation.h"
#include "mqtt.h"
#include "wifi_.h"

TaskHandle_t ServerHandlingTask;
TaskHandle_t GeolocationHandlingTask;
TaskHandle_t ControlHandlingTask;

QueueHandle_t MqttPublishingEventQueue;
QueueHandle_t MqttSubscriptionsEventQueue;

String TOPIC_AIR_TEMPERATURE =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_AIR_TEMPERATURE);

String TOPIC_AIR_RELATIVE_HUMIDITY =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_AIR_RELATIVE_HUMIDITY);

String TOPIC_SOIL_MOISTURE =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_SOIL_MOISTURE);

String TOPIC_TANK_LEVEL =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_TANK_LEVEL);

String TOPIC_GEOLOCATION =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_GEOLOCATION);

String TOPIC_CONTROL_PUMP =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_CONTROL_PUMP);

String TOPIC_CONTROL_DISPLAY_BACKLIGHT =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_CONTROL_DISPLAY_BACKLIGHT);

String TOPIC_CONTROL_TAKE_MEASUREMENTS =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_CONTROL_TAKE_MEASUREMENTS);

String TOPIC_CONTROL_RESET_DEVICE =
    String(MQTT_TOPIC_BASE) + String(MQTT_TOPIC_CONTROL_RESET_DEVICE);

void mqttSubscriptionCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadString = (char *)payload;
  payloadString.remove(length);

  services::MqttMessage *mqttMessage =
      new services::MqttMessage(topicStr, payloadString);

  if (topicStr.equals(TOPIC_CONTROL_PUMP) &&
      (payloadString.equals("0") || payloadString.equals("1"))) {
    logging::logger->Debug("Received message from MQTT server. Topic: " +
                           topicStr + ". Payload: " + payloadString);

  } else if (topicStr.equals(TOPIC_CONTROL_DISPLAY_BACKLIGHT) &&
             (payloadString.equals("0") || payloadString.equals("1"))) {
    logging::logger->Debug("Received message from MQTT server. Topic: " +
                           topicStr + ". Payload: " + payloadString);

  } else if (topicStr.equals(TOPIC_CONTROL_TAKE_MEASUREMENTS) &&
             payloadString.equals("1")) {
    logging::logger->Debug("Received message from MQTT server. Topic: " +
                           topicStr + ". Payload: " + payloadString);

  } else if (topicStr.equals(TOPIC_CONTROL_RESET_DEVICE) &&
             payloadString.equals("1")) {
    logging::logger->Debug("Received message from MQTT server. Topic: " +
                           topicStr + ". Payload: " + payloadString);

  } else {
    logging::logger->Debug(
        "Received unimplemented message from MQTT server. Topic: " + topicStr +
        ". Payload: " + payloadString + ". Ignoring message.");

    return;
  }

  xQueueSend(MqttSubscriptionsEventQueue, &mqttMessage, pdMS_TO_TICKS(200));
}

void serverHandling(void *parameter) {
  services::WifiService *wifi = new services::WifiService(
      WIFI_SSID, WIFI_PASSWORD, WIFI_MAX_RETRY_TIME_MILLISECONDS);

  WiFiClient *espClient = new WiFiClient();
  PubSubClient *mqttClient = new PubSubClient(*espClient);
  mqttClient->setCallback(mqttSubscriptionCallback);

  services::MqttService *mqtt = new services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, MQTT_CLIENT_ID,
      MQTT_MAX_RETRY_TIME_MILLISECONDS, mqttClient);

  services::MqttMessage *mqttMessage;

  while (true) {
    if (!wifi->IsConnected()) {
      wifi->Connect();

    } else if (!mqtt->IsConnected()) {
      if (mqtt->Connect()) {
        mqtt->Subscribe(TOPIC_CONTROL_PUMP);
        mqtt->Subscribe(TOPIC_CONTROL_DISPLAY_BACKLIGHT);
        mqtt->Subscribe(TOPIC_CONTROL_TAKE_MEASUREMENTS);
        mqtt->Subscribe(TOPIC_CONTROL_RESET_DEVICE);
      }

    } else {
      mqtt->Loop();

      while (xQueueReceive(MqttPublishingEventQueue, &mqttMessage,
                           pdMS_TO_TICKS(10)) == pdPASS) {
        mqtt->Publish(mqttMessage);
        delete mqttMessage;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void geolocationHandling(void *parameter) {
  services::Geolocation *geolocation = new services::Geolocation(
      GEOLOCATION_REQUEST_URL, GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS,
      GEOLOCATION_INTERVAL_MILLISECONDS);

  while (true) {
    if (geolocation->IsLocalizationTimeReached()) {
      String localization = geolocation->Localize();
      if (!localization.isEmpty()) {
        services::MqttMessage *localizationMessage =
            new services::MqttMessage(TOPIC_GEOLOCATION, localization);
        xQueueSend(MqttPublishingEventQueue, &localizationMessage,
                   pdMS_TO_TICKS(10));
      }
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void controlHandling(void *parameter) {
  peripherals::Dht22 *dht22 = new peripherals::Dht22(DHT22_PIN);

  peripherals::Hcsr04 *hcsr04 = new peripherals::Hcsr04(
      HCSR04_PIN_ECHO, HCSR04_PIN_TRIGGER, HCSR04_SENSOR_HEIGHT,
      HCSR04_MAX_LEVEL_HEIGHT, HCSR04_SIGNAL_DELAY_MICROSECONDS,
      HCSR04_SOUND_SPEED_CM_PER_MICROSECOND);

  peripherals::Lcd1602I2c *lcd1602I2c = new peripherals::Lcd1602I2c(
      LCD1602I2C_ADDRESS, LCD1602I2C_COLS, LCD1602I2C_ROWS);

  peripherals::Sen0193 *sen0193 = new peripherals::Sen0193(
      SEN0193_PIN, SEN0193_WATER_VALUE, SEN0193_AIR_VALUE);

  peripherals::Led *warningLedIndicator =
      new peripherals::Led(WARNING_LED_INDICATOR_PIN);

  control::Controller *controller =
      new control::Controller(MEASURING_INTERVAL_MILLISECONDS, dht22, hcsr04,
                              sen0193, warningLedIndicator, lcd1602I2c);

  services::MqttMessage *mqttMessage;

  control::Measures *measures = nullptr;
  bool takeMeasurements = false;

  while (true) {
    if (xQueueReceive(MqttSubscriptionsEventQueue, &mqttMessage,
                      pdMS_TO_TICKS(10)) == pdPASS) {
      if (mqttMessage->GetTopic().equals(TOPIC_CONTROL_PUMP)) {
        controller->StartPump(mqttMessage->GetPayload().equals("1"));

      } else if (mqttMessage->GetTopic().equals(
                     TOPIC_CONTROL_DISPLAY_BACKLIGHT)) {
        controller->DisplayBacklightOn(mqttMessage->GetPayload().equals("1"));

      } else if (mqttMessage->GetTopic().equals(
                     TOPIC_CONTROL_TAKE_MEASUREMENTS)) {
        takeMeasurements = mqttMessage->GetPayload().equals("1");

      } else if (mqttMessage->GetTopic().equals(TOPIC_CONTROL_RESET_DEVICE)) {
        if (mqttMessage->GetPayload().equals("1"))
          ESP.restart();
      }

      delete mqttMessage;
    }

    controller->Loop();
    measures = controller->Measure(takeMeasurements);

    if (measures != nullptr) {
      float airTemperature = measures->GetAirTemperature();
      if (!isnan(airTemperature)) {
        services::MqttMessage *airTemperatureMessage =
            new services::MqttMessage(TOPIC_AIR_TEMPERATURE, airTemperature);
        xQueueSend(MqttPublishingEventQueue, &airTemperatureMessage,
                   pdMS_TO_TICKS(10));
      }

      float airRelativeHumidity = measures->GetAirRelativeHumidity();
      if (!isnan(airRelativeHumidity)) {
        services::MqttMessage *airRelativeHumidityMessage =
            new services::MqttMessage(TOPIC_AIR_RELATIVE_HUMIDITY,
                                      airRelativeHumidity);
        xQueueSend(MqttPublishingEventQueue, &airRelativeHumidityMessage,
                   pdMS_TO_TICKS(10));
      }

      float soilMoisture = measures->GetSoilMoisture();
      if (!isnan(soilMoisture)) {
        services::MqttMessage *soilMoistureMessage =
            new services::MqttMessage(TOPIC_SOIL_MOISTURE, soilMoisture);
        xQueueSend(MqttPublishingEventQueue, &soilMoistureMessage,
                   pdMS_TO_TICKS(10));
      }

      float tankLevel = measures->GetTankLevel();
      if (!isnan(tankLevel)) {
        services::MqttMessage *tankLevelMessage =
            new services::MqttMessage(TOPIC_TANK_LEVEL, tankLevel);
        xQueueSend(MqttPublishingEventQueue, &tankLevelMessage,
                   pdMS_TO_TICKS(10));
      }

      takeMeasurements = false;
      delete measures;
    }

    vTaskDelay(pdMS_TO_TICKS(100));
  }
}

void setup() {
  xTaskCreatePinnedToCore(serverHandling, "serverHandling", 15000, NULL, 1,
                          &ServerHandlingTask, 0);
  xTaskCreatePinnedToCore(geolocationHandling, "geolocationHandling", 15000,
                          NULL, 0, &GeolocationHandlingTask, 0);
  xTaskCreatePinnedToCore(controlHandling, "controlHandling", 15000, NULL, 1,
                          &ControlHandlingTask, 1);

  MqttPublishingEventQueue = xQueueCreate(7, sizeof(services::MqttMessage *));
  MqttSubscriptionsEventQueue =
      xQueueCreate(1, sizeof(services::MqttMessage *));
}

void loop() {}
