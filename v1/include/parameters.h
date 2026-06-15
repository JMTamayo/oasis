#pragma once

// Configuration parameters to define the project:
#define PROJECT_NAME "oasis"
#define PROJECT_VERSION "v1"

// Configuration parameters for the device:
#define DEVICE_ANALOG_READ_RESOLUTION 4095
#define DEVICE_ANALOG_READ_VOLTAGE 3.3

// Configuration parameters to handle the tasks:
#define SERVER_HANDLING_TASK_NAME "serverHandling"
#define SERVER_HANDLING_TASK_PRIORITY 1
#define SERVER_HANDLING_TASK_STACK_SIZE 6144
#define SERVER_HANDLING_TASK_CORE 0

#define GEOLOCATION_HANDLING_TASK_NAME "geolocationHandling"
#define GEOLOCATION_HANDLING_TASK_PRIORITY 0
#define GEOLOCATION_HANDLING_TASK_STACK_SIZE 8192
#define GEOLOCATION_HANDLING_TASK_CORE 0

#define CONTROL_HANDLING_TASK_NAME "controlHandling"
#define CONTROL_HANDLING_TASK_PRIORITY 1
#define CONTROL_HANDLING_TASK_STACK_SIZE 4096
#define CONTROL_HANDLING_TASK_CORE 1

// Configuration parameters to handle the queues:
#define MQTT_PUBLISHING_EVENT_QUEUE_SIZE 12
#define MQTT_SUBSCRIPTIONS_EVENT_QUEUE_SIZE 1

// Configuration parameters to handle WiFi, MQTT and Geolocation:
#define WIFI_MAX_RETRY_TIME_MILLISECONDS 5000
#define MQTT_MAX_RETRY_TIME_MILLISECONDS 5000
#define MQTT_BUFFER_SIZE 1024
#define GEOLOCATION_PUBLISHING_TIME_MILLISECONDS 300000
#define GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS 1000

// Configuration parameters to define the server commands:
#define MQTT_TOPIC_BASE_SEPARATOR "/"

#define MQTT_COMMAND_PING "ping"
#define MQTT_COMMAND_RESTART_DEVICE "restart-device"
#define MQTT_COMMAND_SET_SAMPLING_TIME "set-sampling-time"
#define MQTT_COMMAND_GET_SAMPLING_TIME "get-sampling-time"
#define MQTT_COMMAND_SET_MEASUREMENTS_PUBLICATION_TIME                         \
  "set-measurements-publication-time"
#define MQTT_COMMAND_GET_MEASUREMENTS_PUBLICATION_TIME                         \
  "get-measurements-publication-time"
#define MQTT_COMMAND_GET_PENDING_TIME_FOR_MEASUREMENTS_PUBLICATION             \
  "get-pending-time-for-measurements-publication"

#define MQTT_SUBJECT_PONG "pong"
#define MQTT_SUBJECT_SAMPLING_TIME "sampling-time"
#define MQTT_SUBJECT_MEASUREMENTS_PUBLICATION_TIME                             \
  "measurements-publication-time"
#define MQTT_SUBJECT_PENDING_TIME_FOR_MEASUREMENTS_PUBLICATION                 \
  "pending-time-for-measurements-publication"
#define MQTT_SUBJECT_ERROR "error"
#define MQTT_SUBJECT_GEOLOCATION "geolocation"
#define MQTT_SUBJECT_AIR_TEMPERATURE "air-temperature"
#define MQTT_SUBJECT_AIR_HUMIDITY "air-humidity"
#define MQTT_SUBJECT_SOIL_MOISTURE_1 "soil-moisture-1"
#define MQTT_SUBJECT_SOIL_MOISTURE_2 "soil-moisture-2"
#define MQTT_SUBJECT_AVERAGE_SOIL_MOISTURE "average-soil-moisture"

// Configuration parameters to define the sampling time:
#define DEFAULT_SAMPLING_TIME_MILLISECONDS 2000
#define MIN_SAMPLING_TIME_MILLISECONDS 2000
#define MAX_SAMPLING_TIME_MILLISECONDS 60000

// Configuration parameters to publish measurements:
#define DEFAULT_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS 60000
#define MIN_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS 2000
#define MAX_MEASUREMENTS_PUBLICATION_TIME_MILLISECONDS 3600000

// Configuration parameters to handle the indicator LED:
#define INDICATOR_LED_PIN 4

// Configuration parameters to define the I2C bus:
#define I2C_SDA_PIN 21
#define I2C_SCL_PIN 22

// Configuration parameters to define the OLED SSD1306 display:
#define OLED_SSD1306_WIDTH 128
#define OLED_SSD1306_HEIGHT 64
#define OLED_SSD1306_ADDRESS 0x3C

#define DEFAULT_DISPLAY_ENABLED_TIME_MILLISECONDS 30000
#define MIN_DISPLAY_ENABLED_TIME_MILLISECONDS 10000
#define MAX_DISPLAY_ENABLED_TIME_MILLISECONDS 120000

// Configuration parameters to define the SR602 infrared sensor:
#define SR602_PIN 33

// Configuration parameters to define the DHT sensor:
#define DHT_PIN 32
#define DHT_TYPE DHT22

// Configuration parameters to define the SEN0193 sensors:
#define SEN0193_1_PIN 34
#define SEN0193_1_WATER_VALUE 1330
#define SEN0193_1_AIR_VALUE 3375

#define SEN0193_2_PIN 35
#define SEN0193_2_WATER_VALUE 1340
#define SEN0193_2_AIR_VALUE 3386
