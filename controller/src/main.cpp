#include <Arduino.h>
#include <ArduinoJson.h>
#include <DHT.h>

#include "parameters.h"

#include "hcsr04.h"
#include "led.h"
#include "pump.h"
#include "sen0193.h"

DHT dht(DHT_PIN, DHT_TYPE);

peripherals::Hcsr04 *hcsr04;
peripherals::Led *builtinLed;
peripherals::Sen0193 *sen0193_1;
peripherals::Sen0193 *sen0193_2;
peripherals::Pump *pump;

unsigned long lastMeasurementTime;
unsigned long samplingTime;

unsigned long timeBetweenPumping;
unsigned long lastTimeBetweenPumping;
unsigned long pumpingTimeLimit;
unsigned long lastPumpingTime;
float lastPumpingSpeed;

void ping() { Serial.println(RESPONSE_PONG); }

bool pumping() { return lastPumpingSpeed != 0.0f; }

void getPendingTimeForPumping() {
  unsigned long elapsedTime = millis() - lastTimeBetweenPumping;
  unsigned long pendingTime = elapsedTime >= timeBetweenPumping
                                  ? 0
                                  : (timeBetweenPumping - elapsedTime);
  Serial.println(RESPONSE_PENDING_TIME_FOR_PUMPING +
                 String(INTERCOMM_SEPARATOR) + pendingTime);
}

void pumpWater(float speed) {
  if (pumping() && speed == 0.0f) {
    pump->Run(speed);
    lastPumpingSpeed = speed;
    lastTimeBetweenPumping = millis();
    return;
  }

  if (!pumping() && speed > 0.0f) {
    unsigned long elapsedTime = millis() - lastTimeBetweenPumping;
    if (elapsedTime < timeBetweenPumping) {
      Serial.println(RESPONSE_ERROR + String(INTERCOMM_SEPARATOR) +
                     "Minimum time between pumping not reached");
      return;
    }
    pump->Run(speed);
    lastPumpingSpeed = speed;
    lastPumpingTime = millis();
    return;
  }

  if (pumping() && lastPumpingSpeed != speed && speed > 0.0f) {
    pump->Run(speed);
    lastPumpingSpeed = speed;
  }
}

void setPumpingTimeLimit(unsigned long time) {
  pumpingTimeLimit = constrain(time, MIN_PUMPING_TIME_MS, MAX_PUMPING_TIME_MS);
}

void getPumpingTimeLimit() {
  Serial.println(RESPONSE_PUMPING_TIME_LIMIT + String(INTERCOMM_SEPARATOR) +
                 pumpingTimeLimit);
}

void setSamplingTime(unsigned long time) {
  samplingTime = constrain(time, MIN_SAMPLING_TIME_MS, MAX_SAMPLING_TIME_MS);
}

void getSamplingTime() {
  Serial.println(RESPONSE_SAMPLING_TIME + String(INTERCOMM_SEPARATOR) +
                 samplingTime);
}

void getPendingTimeForSampling() {
  unsigned long elapsedTime = millis() - lastMeasurementTime;
  unsigned long pendingTime =
      elapsedTime >= samplingTime ? 0 : (samplingTime - elapsedTime);
  Serial.println(RESPONSE_PENDING_TIME_FOR_SAMPLING +
                 String(INTERCOMM_SEPARATOR) + pendingTime);
}

float getAverageSoilMoisture() {
  float soilMoisture1 = sen0193_1->Read();
  float soilMoisture2 = sen0193_2->Read();
  return isnan(soilMoisture1) || isnan(soilMoisture2)
             ? NAN
             : (soilMoisture1 + soilMoisture2) / 2;
}

void measure() {
  JsonDocument json;

  json["air_temperature"] = dht.readTemperature();
  json["air_humidity"] = dht.readHumidity();

  json["soil_moisture"]["sensor_1"] = sen0193_1->Read();
  json["soil_moisture"]["sensor_2"] = sen0193_2->Read();
  json["soil_moisture"]["average"] = getAverageSoilMoisture();

  json["tank_level"] = hcsr04->Read();

  String jsonString;
  serializeJson(json, jsonString);

  Serial.println(RESPONSE_MEASUREMENTS + String(INTERCOMM_SEPARATOR) +
                 jsonString);
}

void getMeasurements() {
  if (millis() - lastMeasurementTime < MIN_SAMPLING_TIME_MS) {
    Serial.println(RESPONSE_ERROR + String(INTERCOMM_SEPARATOR) +
                   "Minimum sampling time not reached");
    return;
  }
  measure();
  lastMeasurementTime = lastMeasurementTime + MIN_SAMPLING_TIME_MS;
}

void commandNotSupported(String command) {
  Serial.println(RESPONSE_ERROR + String(INTERCOMM_SEPARATOR) + "Command '" +
                 command + "' is not supported");
}

void handleRequest(String request) {
  builtinLed->Light(true);

  request.trim();
  int index = request.indexOf(INTERCOMM_SEPARATOR);

  String command = request.substring(0, index);
  command.trim();

  String value = index == -1 ? "" : request.substring(index + 1);
  value.trim();

  if (command.equals(COMMAND_PING)) {
    ping();

  } else if (command.equals(COMMAND_SET_PUMPING_TIME_LIMIT)) {
    setPumpingTimeLimit(value.toInt());
    getPumpingTimeLimit();

  } else if (command.equals(COMMAND_GET_PUMPING_TIME_LIMIT)) {
    getPumpingTimeLimit();

  } else if (command.equals(COMMAND_GET_PENDING_TIME_FOR_PUMPING)) {
    getPendingTimeForPumping();

  } else if (command.equals(COMMAND_PUMP_WATER)) {
    pumpWater(value.toFloat());

  } else if (command.equals(COMMAND_SET_SAMPLING_TIME)) {
    setSamplingTime(value.toInt());
    getSamplingTime();

  } else if (command.equals(COMMAND_GET_SAMPLING_TIME)) {
    getSamplingTime();

  } else if (command.equals(COMMAND_GET_PENDING_TIME_FOR_SAMPLING)) {
    getPendingTimeForSampling();

  } else if (command.equals(COMMAND_GET_MEASUREMENTS)) {
    getMeasurements();

  } else {
    commandNotSupported(command);
  }

  builtinLed->Light(false);
}

void intercommLoop() {
  if (Serial.available() > 0) {
    String request = Serial.readStringUntil('\n');
    handleRequest(request);
  }
}

void pumpingLoop() {
  unsigned long elapsedTime = millis() - lastPumpingTime;
  if (pumping() && (elapsedTime > pumpingTimeLimit))
    pumpWater(0.0f);
}

void measurementsLoop() {
  if (millis() - lastMeasurementTime >= samplingTime) {
    measure();
    lastMeasurementTime = millis();
  }
}

void setup() {
  Serial.begin(INTERCOMM_SERIAL_BAUD_RATE);
  while (!Serial)
    continue;

  builtinLed = new peripherals::Led(LED_BUILTIN);
  builtinLed->Light(false);

  timeBetweenPumping = MIN_TIME_BETWEEN_PUMPING_MS;
  lastTimeBetweenPumping = millis();
  pumpingTimeLimit = MIN_PUMPING_TIME_MS;
  lastPumpingTime = 0;
  lastPumpingSpeed = 0.0f;
  pump = new peripherals::Pump(PUMP_PIN, DEVICE_ANALOG_WRITE_RESOLUTION);
  pump->Run(lastPumpingSpeed);

  dht.begin();

  hcsr04 = new peripherals::Hcsr04(
      HCSR04_PIN_ECHO, HCSR04_PIN_TRIGGER, HCSR04_SENSOR_HEIGHT,
      HCSR04_MAX_LEVEL_HEIGHT, HCSR04_SIGNAL_DELAY_MICROSECONDS,
      HCSR04_SOUND_SPEED_CM_PER_MICROSECOND);

  sen0193_1 =
      new peripherals::Sen0193(SEN0193_1_PIN, DEVICE_ANALOG_READ_RESOLUTION,
                               SEN0193_1_WATER_VALUE, SEN0193_1_AIR_VALUE);
  sen0193_2 =
      new peripherals::Sen0193(SEN0193_2_PIN, DEVICE_ANALOG_READ_RESOLUTION,
                               SEN0193_2_WATER_VALUE, SEN0193_2_AIR_VALUE);

  samplingTime = DEFAULT_SAMPLING_TIME_MS;
  lastMeasurementTime = millis();
}

void loop() {
  intercommLoop();
  pumpingLoop();
  measurementsLoop();
}
