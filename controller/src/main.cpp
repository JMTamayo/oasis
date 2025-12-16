#include <Arduino.h>
#include <DHT.h>

#include "parameters.h"

#include "led.h"
#include "sen0193.h"

DHT *dht;

peripherals::Led *builtinLed;
peripherals::Sen0193 *sen0193_1;
peripherals::Sen0193 *sen0193_2;

unsigned long lastMeasurementTime;
unsigned long samplingTime;

void ping() { Serial.println(RESPONSE_PONG); }

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

void measure() {
  float airTemperature = dht->readTemperature();
  Serial.println(RESPONSE_AIR_TEMPERATURE + String(INTERCOMM_SEPARATOR) +
                 airTemperature);

  float airHumidity = dht->readHumidity();
  Serial.println(RESPONSE_AIR_HUMIDITY + String(INTERCOMM_SEPARATOR) +
                 airHumidity);

  float soilMoisture_1 = sen0193_1->Read();
  Serial.println(RESPONSE_SOIL_MOISTURE_1 + String(INTERCOMM_SEPARATOR) +
                 soilMoisture_1);

  float soilMoisture_2 = sen0193_2->Read();
  Serial.println(RESPONSE_SOIL_MOISTURE_2 + String(INTERCOMM_SEPARATOR) +
                 soilMoisture_2);

  float averageSoilMoisture = (soilMoisture_1 + soilMoisture_2) / 2;
  Serial.println(RESPONSE_AVERAGE_SOIL_MOISTURE + String(INTERCOMM_SEPARATOR) +
                 averageSoilMoisture);
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

void intercomm() {
  if (Serial.available() > 0) {
    String request = Serial.readStringUntil('\n');
    handleRequest(request);
  }
}

void measurements() {
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

  dht = new DHT(DHT_PIN, DHT_TYPE);
  dht->begin();

  sen0193_1 =
      new peripherals::Sen0193(SEN0193_1_PIN, DEVICE_ANALOG_READ_RESOLUTION,
                               SEN0193_1_WATER_VALUE, SEN0193_1_AIR_VALUE);
  sen0193_2 =
      new peripherals::Sen0193(SEN0193_2_PIN, DEVICE_ANALOG_READ_RESOLUTION,
                               SEN0193_2_WATER_VALUE, SEN0193_2_AIR_VALUE);

  samplingTime = DEFAULT_SAMPLING_TIME_MS;
  lastMeasurementTime = millis() - samplingTime + MIN_SAMPLING_TIME_MS;
}

void loop() {
  intercomm();
  measurements();
}
