#define DEVICE_ANALOG_READ_RESOLUTION 1023

#define INTERCOMM_SERIAL_BAUD_RATE 9600
#define INTERCOMM_SEPARATOR ":"

#define COMMAND_PING "ping"
#define COMMAND_SET_SAMPLING_TIME "set-sampling-time"
#define COMMAND_GET_SAMPLING_TIME "get-sampling-time"
#define COMMAND_GET_PENDING_TIME_FOR_SAMPLING "get-pending-time-for-sampling"
#define COMMAND_GET_MEASUREMENTS "get-measurements"

#define RESPONSE_ERROR "error"
#define RESPONSE_PONG "pong"
#define RESPONSE_SAMPLING_TIME "sampling-time"
#define RESPONSE_PENDING_TIME_FOR_SAMPLING "pending-time-for-sampling"
#define RESPONSE_AIR_TEMPERATURE "air-temperature"
#define RESPONSE_AIR_HUMIDITY "air-humidity"
#define RESPONSE_SOIL_MOISTURE_1 "soil-moisture-1"
#define RESPONSE_SOIL_MOISTURE_2 "soil-moisture-2"
#define RESPONSE_AVERAGE_SOIL_MOISTURE "average-soil-moisture"

#define DEFAULT_SAMPLING_TIME_MS 15000
#define MIN_SAMPLING_TIME_MS 2000
#define MAX_SAMPLING_TIME_MS 3600000

#define DHT_PIN 2
#define DHT_TYPE DHT22

#define SEN0193_1_PIN A5
#define SEN0193_1_WATER_VALUE 250
#define SEN0193_1_AIR_VALUE 585

#define SEN0193_2_PIN A4
#define SEN0193_2_WATER_VALUE 250
#define SEN0193_2_AIR_VALUE 585