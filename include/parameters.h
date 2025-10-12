// Configuration parameters to handle WiFi, MQTT and geolocation services:
#define WIFI_MAX_RETRY_TIME_MILLISECONDS 5000
#define MQTT_MAX_RETRY_TIME_MILLISECONDS 5000
#define GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS 500

// Configuration parameters to handle MQTT topics:
#define MQTT_TOPIC_BASE "/oasis"
#define MQTT_TOPIC_AIR_TEMPERATURE "/air-temperature"
#define MQTT_TOPIC_AIR_RELATIVE_HUMIDITY "/air-relative-humidity"
#define MQTT_TOPIC_SOIL_MOISTURE "/soil-moisture"
#define MQTT_TOPIC_GEOLOCATION "/location"
#define MQTT_TOPIC_CONTROL_PUMP "/control-pump"

// Configuration parameters to handle measuring:
#define MEASURING_INTERVAL_MILLISECONDS 2000
#define GEOLOCATION_INTERVAL_MILLISECONDS 60000

// Configuration parameters to handle DHT22:
#define DHT22_PIN 32

// Configuration parameters to handle LCD1602I2C:
#define LCD1602I2C_ADDRESS 0x27
#define LCD1602I2C_COLS 16
#define LCD1602I2C_ROWS 2

// Configuration parameters to handle SEN0193:
#define SEN0193_PIN 35
#define SEN0193_WATER_VALUE 1250
#define SEN0193_AIR_VALUE 3300
#define LOW_MOISTURE_LED_INDICATOR_PIN 18
