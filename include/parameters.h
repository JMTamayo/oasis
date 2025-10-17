// Configuration parameters to handle WiFi, MQTT and geolocation services:
#define WIFI_MAX_RETRY_TIME_MILLISECONDS 5000
#define MQTT_MAX_RETRY_TIME_MILLISECONDS 5000
#define GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS 500

// Configuration parameters to handle MQTT topics to publish data:
#define MQTT_TOPIC_AIR_TEMPERATURE "/air-temperature"
#define MQTT_TOPIC_AIR_RELATIVE_HUMIDITY "/air-relative-humidity"
#define MQTT_TOPIC_SOIL_MOISTURE "/soil-moisture"
#define MQTT_TOPIC_TANK_LEVEL "/tank-level"
#define MQTT_TOPIC_GEOLOCATION "/location"

// Configuration parameters to handle MQTT topics to control the device:
#define MQTT_TOPIC_CONTROL_PUMP "/control-start-pump"
#define MQTT_TOPIC_CONTROL_DISPLAY_BACKLIGHT "/control-display-backlight"
#define MQTT_TOPIC_CONTROL_TAKE_MEASUREMENTS "/control-take-measurements"
#define MQTT_TOPIC_CONTROL_RESET_DEVICE "/control-reset-device"

// Configuration parameters to handle measuring:
#define MEASURING_INTERVAL_MILLISECONDS 60000
#define GEOLOCATION_INTERVAL_MILLISECONDS 3600000

// Configuration parameters to handle DHT22:
#define DHT22_PIN 32

// Configuration parameters to handle HCSR04:
#define HCSR04_PIN_ECHO 26
#define HCSR04_PIN_TRIGGER 27
#define HCSR04_SENSOR_HEIGHT 12
#define HCSR04_MAX_LEVEL_HEIGHT 10
#define HCSR04_SIGNAL_DELAY_MICROSECONDS 20
#define HCSR04_SOUND_SPEED_CM_PER_MICROSECOND 0.034

// Configuration parameters to handle LCD1602I2C:
#define LCD1602I2C_ADDRESS 0x27
#define LCD1602I2C_COLS 16
#define LCD1602I2C_ROWS 2

// Configuration parameters to handle SEN0193:
#define SEN0193_PIN 35
#define SEN0193_WATER_VALUE 1250
#define SEN0193_AIR_VALUE 3300

// Configuration parameters to handle warning led indicator:
#define WARNING_LED_INDICATOR_PIN 33
