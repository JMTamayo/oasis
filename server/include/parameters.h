// Configuration parameters to handle WiFi, MQTT and Geolocation:
#define WIFI_MAX_RETRY_TIME_MILLISECONDS 5000
#define MQTT_MAX_RETRY_TIME_MILLISECONDS 5000
#define GEOLOCATION_INTERVAL_MILLISECONDS 60000
#define GEOLOCATION_REQUEST_TIMEOUT_MILLISECONDS 1000

// Configuration parameters to handle the built-in LED:
#define BUILTIN_LED_PIN 8

// Configuration parameters to define the server commands:
#define MQTT_TOPIC_BASE_SEPARATOR "/"

#define COMMAND_RESET_SERVER "reset-server"
#define COMMAND_RESET_CONTROLLER "reset-controller"
#define COMMAND_PING "ping"
#define COMMAND_SET_SAMPLING_TIME "set-sampling-time"
#define COMMAND_GET_SAMPLING_TIME "get-sampling-time"
#define COMMAND_GET_PENDING_TIME_FOR_SAMPLING "get-pending-time-for-sampling"
#define COMMAND_GET_MEASUREMENTS "get-measurements"

// Configuration parameters to reset the controller:
#define RESET_CONTROLLER_PIN 6
#define RESET_CONTROLLER_DELAY_MICROSECONDS 10

// Configuration parameters to handle the communication with the controller via
// serial UART:
#define INTERCOM_COMMAND_SEPARATOR ":"
#define INTERCOM_TX_PIN 3
#define INTERCOM_RX_PIN 2
#define INTERCOM_BAUD_RATE 9600