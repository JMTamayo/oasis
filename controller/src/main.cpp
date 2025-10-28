#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <DHT.h>

#include "parameters.h"
#include "secrets.h"

#include "led.h"
#include "sen0193.h"

peripherals::Led *led = new peripherals::Led(LED_PIN);

peripherals::Sen0193 *sen0193 = new peripherals::Sen0193(
    SEN0193_PIN, SEN0193_WATER_VALUE, SEN0193_AIR_VALUE);

DHT *dht = new DHT(DHT_PIN, DHT22);

class ServerCallbacks : public BLEServerCallbacks {
public:
  void onConnect(BLEServer *pServer) { led->High(); }
  void onDisconnect(BLEServer *pServer) {
    pServer->getAdvertising()->start();
    led->Low();
  }
};

class ReadAirTemperatureCallback : public BLECharacteristicCallbacks {
public:
  void onRead(BLECharacteristic *pCharacteristic) {
    float value = dht->readTemperature();
    pCharacteristic->setValue(String(value).c_str());
  }
};

class ReadAirRelativeHumidityCallback : public BLECharacteristicCallbacks {
public:
  void onRead(BLECharacteristic *pCharacteristic) {
    float value = dht->readHumidity();
    pCharacteristic->setValue(String(value).c_str());
  }
};

class ReadSoilMoistureCallback : public BLECharacteristicCallbacks {
public:
  void onRead(BLECharacteristic *pCharacteristic) {
    float value = sen0193->Read();
    pCharacteristic->setValue(String(value).c_str());
  }
};

void StartBLE() {
  BLEDevice::init(BLE_SERVER_NAME);

  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(BLE_SERVICE_UUID);

  BLECharacteristic *pAirTemperatureCharacteristic =
      pService->createCharacteristic(BLE_CHARACTERISTIC_AIR_TEMPERATURE_UUID,
                                     BLECharacteristic::PROPERTY_READ);
  pAirTemperatureCharacteristic->setCallbacks(new ReadAirTemperatureCallback());

  BLECharacteristic *pAirRelativeHumidityCharacteristic =
      pService->createCharacteristic(
          BLE_CHARACTERISTIC_AIR_RELATIVE_HUMIDITY_UUID,
          BLECharacteristic::PROPERTY_READ);
  pAirRelativeHumidityCharacteristic->setCallbacks(
      new ReadAirRelativeHumidityCallback());

  BLECharacteristic *pSoilMoistureCharacteristic =
      pService->createCharacteristic(BLE_CHARACTERISTIC_SOIL_MOISTURE_UUID,
                                     BLECharacteristic::PROPERTY_READ);
  pSoilMoistureCharacteristic->setCallbacks(new ReadSoilMoistureCallback());

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(BLE_SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
}

void setup() {
  Serial.begin(BAUD_RATE);
  while (!Serial)
    ;

  dht->begin();
  StartBLE();
}

void loop() {}
