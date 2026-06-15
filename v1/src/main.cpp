#include <Arduino.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <Wire.h>

#include "parameters.h"
#include "secrets.h"

#include "logger.h"

#include "geolocation.h"
#include "mqtt.h"
#include "wifi_.h"

#include "led.h"
#include "oled_ssd1306.h"
#include "sen0193.h"
#include "sr602.h"

logging::Logger
    *logger; // Logger to manage the logging information across the device.

WiFiClient *espClient;       // WiFi client to manage the WiFi connection.
services::WifiService *wifi; // Utilities to manage the WiFi service.

PubSubClient *mqttClient;    // MQTT client to manage the MQTT connection.
services::MqttService *mqtt; // Utilities to manage the MQTT service.

services::Geolocation
    *geolocation; // Geolocation service to get the device's geolocation.

peripherals::Led *indicatorLed; // LED to indicates when a MQTT message is
                                // received and processed.
peripherals::Sen0193
    *sen0193_1; // SEN0193 sensor 1 to measure the soil moisture.
peripherals::Sen0193
    *sen0193_2; // SEN0193 sensor 2 to measure the soil moisture.
peripherals::OledSsd1306 *oled; // OLED SSD1306 display to manage the display.
peripherals::Sr602 *sr602;      // SR602 infrared sensor to detect motion.

DHT *dht; // DHT sensor to measure the environment temperature and
          // relativehumidity.

bool displayEnabled;                  // Display enabled status.
unsigned long lastDisplayEnabledTime; // Last time the display was enabled.
unsigned long displayEnabledTime;     // Display enabled time in milliseconds.

unsigned long lastMeasurementsPublicationTime; // Last time the measurements
                                               // were published.
unsigned long measurementsPublicationTime; // Measurements publication time in
                                           // milliseconds.

unsigned long lastSamplingTime; // Last time the sampling was performed.
unsigned long samplingTime;     // Sampling time in milliseconds.

TaskHandle_t ServerHandlingTask; // Handle for the server communication task
TaskHandle_t GeolocationHandlingTask; // Handle for the geolocation task
TaskHandle_t ControlHandlingTask; // Handle for the control and measurement task

QueueHandle_t
    MqttPublishingEventQueue; // Queue for messages to be published via MQTT
QueueHandle_t MqttSubscriptionsEventQueue; // Queue for incoming MQTT
                                           // subscription messages

/**
  MQTT subscription message callback.

  This function is triggered automatically whenever an MQTT message is received
  on a topic to which the client is subscribed. It parses the topic and
  payload, constructs an MqttMessage object, and places it on the
  MqttSubscriptionsEventQueue for further handling elsewhere in the
  application.

  Parameters:
    topic   - The topic on which the message was received.
    payload - The payload of the MQTT message.
    length  - The length of the payload.
 */
void mqttSubscriptionCallback(char *topic, byte *payload, unsigned int length) {
  String topicStr = topic;
  String payloadStr = (char *)payload;
  payloadStr.remove(length);
  String subject = mqtt->GetSubject(topicStr);

  logger->Info("MQTT", "Message received. Topic: " + topicStr + ". Payload: " +
                           (payloadStr.length() > 0 ? payloadStr : "(empty)"));

  services::MqttMessage *msg = new services::MqttMessage(subject, payloadStr);
  if (xQueueSend(MqttSubscriptionsEventQueue, &msg, pdMS_TO_TICKS(10)) !=
      pdPASS)
    delete msg;

  indicatorLed->Blink();
}

/**
  Server handling task.

  This is a FreeRTOS task that manages network communications. This task
  runs continuously, handling WiFi and MQTT connections, and processing
  messages from the publishing queue that are sent by the control and
  geolocation tasks.

  Parameters:
    parameter - Task parameter (unused in this implementation).
 */
void serverHandling(void *parameter) {
  while (true) {
    if (!wifi->IsConnected()) {
      wifi->Connect();

    } else if (!mqtt->IsConnected()) {
      if (mqtt->Connect()) {
        mqtt->Subscribe(MQTT_COMMAND_RESTART_DEVICE);
        mqtt->Subscribe(MQTT_COMMAND_PING);
        mqtt->Subscribe(MQTT_COMMAND_SET_SAMPLING_TIME);
        mqtt->Subscribe(MQTT_COMMAND_GET_SAMPLING_TIME);
        mqtt->Subscribe(MQTT_COMMAND_SET_MEASUREMENTS_PUBLICATION_TIME);
        mqtt->Subscribe(MQTT_COMMAND_GET_MEASUREMENTS_PUBLICATION_TIME);
        mqtt->Subscribe(
            MQTT_COMMAND_GET_PENDING_TIME_FOR_MEASUREMENTS_PUBLICATION);
      }

    } else {
      mqtt->Loop();

      services::MqttMessage *msg;
      while (xQueueReceive(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) ==
             pdPASS) {
        mqtt->Publish(msg);
        delete msg;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(60));
  }
}

/**
  Geolocation handling task.

  This is a FreeRTOS task that manages the geolocation service. This task
  runs continuously, getting the device's geolocation and sending it to the
  MQTT service via queue.

  Parameters:
    parameter - Task parameter (unused in this implementation).
 */
void geolocationHandling(void *parameter) {
  while (true) {
    if (wifi->IsConnected()) {
      services::MqttMessage *msg = geolocation->Loop();
      if (msg != nullptr) {
        if (xQueueSend(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) !=
            pdPASS)
          delete msg;
      }
    }

    vTaskDelay(pdMS_TO_TICKS(60));
  }
}

/**
  Restart device function.

  This function restarts the device instantly before turning off the display.
 */
void restartDevice() {
  oled->Lighting(false);
  ESP.restart();
}

/**
  Ping function.

  This function sends a pong message to the MQTT server after a ping command is
  received.
 */
void ping() {
  services::MqttMessage *msg = new services::MqttMessage(MQTT_SUBJECT_PONG, "");
  if (xQueueSend(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) != pdPASS)
    delete msg;
}

/**
  Set sampling time function.

  This function sets the sampling time. If the requested time is less than the
  minimum time, the minimum time is used. If the requested time is greater than
  the maximum time, the maximum time is used.
 */
void setSamplingTime(unsigned long time) {
  samplingTime = constrain(time, MIN_SAMPLING_TIME_MILLISECONDS,
                           MAX_SAMPLING_TIME_MILLISECONDS);
}

/**
  Get sampling time function.

  This function sends a message to the MQTT server with the current sampling
  time.
 */
void getSamplingTime() {
  services::MqttMessage *msg =
      new services::MqttMessage(MQTT_SUBJECT_SAMPLING_TIME, samplingTime);
  if (xQueueSend(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) != pdPASS)
    delete msg;
}

/**
  Set measurements publication time function.

  This function sets the measurements publication time. If the requested time is
  less than the minimum time, the minimum time is used. If the requested time is
  greater than the maximum time, the maximum time is used.
 */
void setMeasurementsPublicationTime(unsigned long time) {
  measurementsPublicationTime =
      constrain(time, MIN_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS,
                MAX_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS);
}

/**
  Get measurements publication time function.

  This function sends a message to the MQTT server with the current measurements
  publication time.
 */
void getMeasurementsPublicationTime() {
  services::MqttMessage *msg = new services::MqttMessage(
      MQTT_SUBJECT_MEASUREMENTS_PUBLICATION_TIME, measurementsPublicationTime);
  if (xQueueSend(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) != pdPASS)
    delete msg;
}

/**
  Get pending time for measurements publication function.

  This function calculates the pending time for the next measurements
  publication and sends a message to the MQTT server with the pending time.
 */
void getPendingTimeForMeasurementsPublication() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - lastMeasurementsPublicationTime;
  unsigned long pendingTime = elapsedTime >= measurementsPublicationTime
                                  ? 0
                                  : measurementsPublicationTime - elapsedTime;

  services::MqttMessage *msg = new services::MqttMessage(
      MQTT_SUBJECT_PENDING_TIME_FOR_MEASUREMENTS_PUBLICATION, pendingTime);
  if (xQueueSend(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) != pdPASS)
    delete msg;
}

/**
  Command not supported function.

  This function sends an error message to the MQTT server indicating that the
  received command is not supported.

  Parameters:
    command - The command that is not supported.
 */
void commandNotSupported(String command) {
  services::MqttMessage *msg = new services::MqttMessage(
      MQTT_SUBJECT_ERROR, String("Command not supported: ") + command);
  if (xQueueSend(MqttPublishingEventQueue, &msg, pdMS_TO_TICKS(10)) != pdPASS)
    delete msg;
}

/**
  Publish measurements function.

  This function reads the implemented sensors data and sends them to the MQTT
  server via queue.
 */
void publishMeasurements() {
  float airTemperature = dht->readTemperature();
  if (isnan(airTemperature)) {
    services::MqttMessage *errorMsg = new services::MqttMessage(
        MQTT_SUBJECT_ERROR,
        String("Failed to read air temperature. NaN value obtained."));
    if (xQueueSend(MqttPublishingEventQueue, &errorMsg, pdMS_TO_TICKS(10)) !=
        pdPASS)
      delete errorMsg;
  } else {
    services::MqttMessage *airTemperatureMsg =
        new services::MqttMessage(MQTT_SUBJECT_AIR_TEMPERATURE, airTemperature);
    if (xQueueSend(MqttPublishingEventQueue, &airTemperatureMsg,
                   pdMS_TO_TICKS(10)) != pdPASS)
      delete airTemperatureMsg;
  }

  float airHumidity = dht->readHumidity();
  if (isnan(airHumidity)) {
    services::MqttMessage *errorMsg = new services::MqttMessage(
        MQTT_SUBJECT_ERROR,
        String("Failed to read air humidity. NaN value obtained."));
    if (xQueueSend(MqttPublishingEventQueue, &errorMsg, pdMS_TO_TICKS(10)) !=
        pdPASS)
      delete errorMsg;
  } else {
    services::MqttMessage *airHumidityMsg =
        new services::MqttMessage(MQTT_SUBJECT_AIR_HUMIDITY, airHumidity);
    if (xQueueSend(MqttPublishingEventQueue, &airHumidityMsg,
                   pdMS_TO_TICKS(10)) != pdPASS)
      delete airHumidityMsg;
  }

  float soilMoisture1 = sen0193_1->Read();
  if (isnan(soilMoisture1)) {
    services::MqttMessage *errorMsg = new services::MqttMessage(
        MQTT_SUBJECT_ERROR,
        String("Failed to read soil moisture 1. NaN value obtained."));
    if (xQueueSend(MqttPublishingEventQueue, &errorMsg, pdMS_TO_TICKS(10)) !=
        pdPASS)
      delete errorMsg;
  } else {
    services::MqttMessage *soilMoisture1Msg =
        new services::MqttMessage(MQTT_SUBJECT_SOIL_MOISTURE_1, soilMoisture1);
    if (xQueueSend(MqttPublishingEventQueue, &soilMoisture1Msg,
                   pdMS_TO_TICKS(10)) != pdPASS)
      delete soilMoisture1Msg;
  }

  float soilMoisture2 = sen0193_2->Read();
  if (isnan(soilMoisture2)) {
    services::MqttMessage *errorMsg = new services::MqttMessage(
        MQTT_SUBJECT_ERROR,
        String("Failed to read soil moisture 2. NaN value obtained."));
    if (xQueueSend(MqttPublishingEventQueue, &errorMsg, pdMS_TO_TICKS(10)) !=
        pdPASS)
      delete errorMsg;
  } else {
    services::MqttMessage *soilMoisture2Msg =
        new services::MqttMessage(MQTT_SUBJECT_SOIL_MOISTURE_2, soilMoisture2);
    if (xQueueSend(MqttPublishingEventQueue, &soilMoisture2Msg,
                   pdMS_TO_TICKS(10)) != pdPASS)
      delete soilMoisture2Msg;
  }

  float averageSoilMoisture = (soilMoisture1 + soilMoisture2) / 2;
  if (isnan(averageSoilMoisture)) {
    services::MqttMessage *errorMsg = new services::MqttMessage(
        MQTT_SUBJECT_ERROR,
        String("Failed to read average soil moisture. NaN value obtained."));
    if (xQueueSend(MqttPublishingEventQueue, &errorMsg, pdMS_TO_TICKS(10)) !=
        pdPASS)
      delete errorMsg;
  } else {
    services::MqttMessage *averageSoilMoistureMsg = new services::MqttMessage(
        MQTT_SUBJECT_AVERAGE_SOIL_MOISTURE, averageSoilMoisture);
    if (xQueueSend(MqttPublishingEventQueue, &averageSoilMoistureMsg,
                   pdMS_TO_TICKS(10)) != pdPASS)
      delete averageSoilMoistureMsg;
  }
}

/**
  Display measurements function.

  This function displays the measurements on the OLED SSD1306 display.
 */
void displayMeasurements() {
  float averageSoilMoisture = (sen0193_1->Read() + sen0193_2->Read()) / 2.0f;
  oled->Display(wifi->IsConnected(), mqtt->IsConnected(),
                dht->readTemperature(), dht->readHumidity(),
                averageSoilMoisture);
}

/**
  Control handling task.

  This is a FreeRTOS task that manages sensor measurements and control. This
  task runs continuously, taking sensor measurements and sending them to the
  server task via queues, while also receiving control commands from MQTT.

  Parameters:
    parameter - Task parameter (unused in this implementation).
 */
void controlHandling(void *parameter) {
  bool motion = false;

  while (true) {
    services::MqttMessage *msg;
    if (xQueueReceive(MqttSubscriptionsEventQueue, &msg, pdMS_TO_TICKS(10)) ==
        pdPASS) {

      String subject = String(msg->GetSubject());
      String payload = String(msg->GetPayload());

      if (subject.equals(MQTT_COMMAND_RESTART_DEVICE)) {
        restartDevice();

      } else if (subject.equals(MQTT_COMMAND_PING)) {
        ping();

      } else if (subject.equals(MQTT_COMMAND_SET_SAMPLING_TIME)) {
        setSamplingTime(payload.toInt());
        getSamplingTime();

      } else if (subject.equals(MQTT_COMMAND_GET_SAMPLING_TIME)) {
        getSamplingTime();

      } else if (subject.equals(
                     MQTT_COMMAND_SET_MEASUREMENTS_PUBLICATION_TIME)) {
        setMeasurementsPublicationTime(payload.toInt());
        getMeasurementsPublicationTime();

      } else if (subject.equals(
                     MQTT_COMMAND_GET_MEASUREMENTS_PUBLICATION_TIME)) {
        getMeasurementsPublicationTime();

      } else if (
          subject.equals(
              MQTT_COMMAND_GET_PENDING_TIME_FOR_MEASUREMENTS_PUBLICATION)) {
        getPendingTimeForMeasurementsPublication();

      } else {
        commandNotSupported(subject);
      }
      delete msg;
    }

    unsigned long currentTime = millis();

    if (currentTime - lastMeasurementsPublicationTime >=
        measurementsPublicationTime) {
      publishMeasurements();
      lastMeasurementsPublicationTime = currentTime;
    }

    if (currentTime - lastSamplingTime >= samplingTime) {
      displayMeasurements();
      lastSamplingTime = currentTime;
    }

    motion = sr602->Read();
    if (displayEnabled && motion) {
      lastDisplayEnabledTime = currentTime;

    } else if (displayEnabled &&
               currentTime - lastDisplayEnabledTime > displayEnabledTime) {
      oled->Lighting(false);
      displayEnabled = false;

    } else if (!displayEnabled && motion) {
      oled->Lighting(true);
      displayEnabled = true;
      lastDisplayEnabledTime = currentTime;
    }

    vTaskDelay(pdMS_TO_TICKS(10));
  }
}

/**
  Setup function.

  Initializes the ESP32 system with peripherals, logger, services, tasks and
  queues.
 */
void setup() {
  // Initialize the logger
  logger = new logging::Logger();
  logger->Begin();

  // Initialize the I2C bus
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);

  // Initialize the builtin LED
  indicatorLed = new peripherals::Led(INDICATOR_LED_PIN);

  // Initialize the SEN0193 sensors
  sen0193_1 =
      new peripherals::Sen0193(SEN0193_1_PIN, DEVICE_ANALOG_READ_RESOLUTION,
                               SEN0193_1_WATER_VALUE, SEN0193_1_AIR_VALUE);
  sen0193_2 =
      new peripherals::Sen0193(SEN0193_2_PIN, DEVICE_ANALOG_READ_RESOLUTION,
                               SEN0193_2_WATER_VALUE, SEN0193_2_AIR_VALUE);

  // Initialize the OLED SSD1306 display
  oled = new peripherals::OledSsd1306(OLED_SSD1306_WIDTH, OLED_SSD1306_HEIGHT,
                                      OLED_SSD1306_ADDRESS, PROJECT_NAME,
                                      DEVICE_ID);
  bool ok = oled->Begin(OLED_SSD1306_ADDRESS);
  if (!ok)
    logger->Error("SETUP", "Failed to initialize the OLED SSD1306 display");

  // Initialize the SR602 infrared sensor
  sr602 = new peripherals::Sr602(SR602_PIN);

  // Initialize the DHT sensor
  dht = new DHT(DHT_PIN, DHT_TYPE);
  dht->begin();

  // Initialize the WiFi client and service
  espClient = new WiFiClient();
  wifi = new services::WifiService(WIFI_SSID, WIFI_PASSWORD,
                                   WIFI_MAX_RETRY_TIME_MILLISECONDS, logger);

  // Initialize the MQTT client and service
  mqttClient = new PubSubClient(*espClient);
  mqttClient->setBufferSize(MQTT_BUFFER_SIZE);
  mqttClient->setCallback(mqttSubscriptionCallback);

  mqtt = new services::MqttService(
      MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD, PROJECT_NAME, DEVICE_ID,
      PROJECT_VERSION, MQTT_TOPIC_BASE_SEPARATOR,
      MQTT_MAX_RETRY_TIME_MILLISECONDS, mqttClient, logger);

  // Initialize the geolocation service
  geolocation = new services::Geolocation(
      GEOLOCATION_REQUEST_URL, GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS,
      GEOLOCATION_PUBLISHING_TIME_MILLISECONDS, MQTT_SUBJECT_GEOLOCATION,
      logger);

  // Initialize the publication times and sampling times
  unsigned long currentTime = millis();

  measurementsPublicationTime =
      DEFAULT_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS;
  lastMeasurementsPublicationTime =
      currentTime - measurementsPublicationTime +
      MIN_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS;

  samplingTime = DEFAULT_SAMPLING_TIME_MILLISECONDS;
  lastSamplingTime = currentTime;

  // Initialize the display status and times
  displayEnabled = true;
  lastDisplayEnabledTime = currentTime;
  displayEnabledTime = DEFAULT_DISPLAY_ENABLED_TIME_MILLISECONDS;

  // Set up the tasks
  xTaskCreatePinnedToCore(serverHandling, SERVER_HANDLING_TASK_NAME,
                          SERVER_HANDLING_TASK_STACK_SIZE, NULL,
                          SERVER_HANDLING_TASK_PRIORITY, &ServerHandlingTask,
                          SERVER_HANDLING_TASK_CORE);
  xTaskCreatePinnedToCore(geolocationHandling, GEOLOCATION_HANDLING_TASK_NAME,
                          GEOLOCATION_HANDLING_TASK_STACK_SIZE, NULL,
                          GEOLOCATION_HANDLING_TASK_PRIORITY,
                          &GeolocationHandlingTask,
                          GEOLOCATION_HANDLING_TASK_CORE);

  xTaskCreatePinnedToCore(controlHandling, CONTROL_HANDLING_TASK_NAME,
                          CONTROL_HANDLING_TASK_STACK_SIZE, NULL,
                          CONTROL_HANDLING_TASK_PRIORITY, &ControlHandlingTask,
                          CONTROL_HANDLING_TASK_CORE);

  // Set up the queues
  MqttPublishingEventQueue = xQueueCreate(MQTT_PUBLISHING_EVENT_QUEUE_SIZE,
                                          sizeof(services::MqttMessage *));
  MqttSubscriptionsEventQueue = xQueueCreate(
      MQTT_SUBSCRIPTIONS_EVENT_QUEUE_SIZE, sizeof(services::MqttMessage *));

  logger->Info("SETUP", "Device setup completed");
}

void loop() {}