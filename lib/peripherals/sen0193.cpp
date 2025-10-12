#include "sen0193.h"

namespace peripherals {

SoilMoisture::SoilMoisture(float moisture, SoilMoistureLevel level)
    : moisture(moisture), level(level) {}

SoilMoisture::~SoilMoisture() {}

float SoilMoisture::GetMoisture() { return this->moisture; }

SoilMoistureLevel SoilMoisture::GetLevel() { return this->level; }

String SoilMoisture::GetLevelString() {
  switch (this->level) {
  case SoilMoistureLevel::EXTREMELY_DRY:
    return "EXTREMELY_DRY";
  case SoilMoistureLevel::DRY:
    return "DRY";
  case SoilMoistureLevel::MOIST:
    return "MOIST";
  case SoilMoistureLevel::WET:
    return "WET";
  case SoilMoistureLevel::SATURATED:
    return "SATURATED";
  default:
    return "UNKNOWN";
  }
}

const unsigned int Sen0193::getPin() const { return this->pin; }

unsigned long Sen0193::getAirValue() const { return this->airValue; }

unsigned long Sen0193::getWaterValue() const { return this->waterValue; }

Sen0193::Sen0193(const unsigned int pin, unsigned long waterValue,
                 unsigned long airValue)
    : pin(pin) {
  const unsigned long resolution = 4095;

  if (waterValue < airValue && airValue <= resolution) {
    this->airValue = airValue;
    this->waterValue = waterValue;

  } else {
    this->airValue = resolution;
    this->waterValue = 0;
  }

  pinMode(pin, INPUT);
}

Sen0193::~Sen0193() {}

SoilMoisture Sen0193::Read() {
  SoilMoistureLevel level;

  unsigned long value = analogRead(this->getPin());

  const float airValue = (float)this->getAirValue();
  const float waterValue = (float)this->getWaterValue();
  const float range = airValue - waterValue;

  float moisture =
      constrain(1.0f - ((float)value - waterValue) / range, 0.0f, 1.0f);

  if (moisture <= 0.2f) {
    level = SoilMoistureLevel::EXTREMELY_DRY;
  } else if (moisture <= 0.4f) {
    level = SoilMoistureLevel::DRY;
  } else if (moisture <= 0.6f) {
    level = SoilMoistureLevel::MOIST;
  } else if (moisture <= 0.8f) {
    level = SoilMoistureLevel::WET;
  } else {
    level = SoilMoistureLevel::SATURATED;
  }

  return SoilMoisture(moisture * 100.0f, level);
}
} // namespace peripherals
