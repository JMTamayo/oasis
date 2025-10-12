#include "lcd1602_i2c.h"

namespace peripherals {

LiquidCrystal_I2C *Lcd1602I2c::getLcd() const { return this->lcd; }

Lcd1602I2c::Lcd1602I2c(unsigned int address, unsigned int cols,
                       unsigned int rows) {
  this->lcd = new LiquidCrystal_I2C(address, cols, rows);

  this->getLcd()->init();
  this->getLcd()->clear();
  this->getLcd()->backlight();
}

Lcd1602I2c::~Lcd1602I2c() { delete lcd; }

void Lcd1602I2c::DisplayProcessStatus(float airTemperature,
                                      float airRelativeHumidity,
                                      float soilMoisture, float tankLevel) {
  this->getLcd()->clear();

  this->getLcd()->setCursor(0, 0);
  this->getLcd()->print("AT:");
  this->getLcd()->print(String(airTemperature, 1));

  this->getLcd()->setCursor(8, 0);
  this->getLcd()->print("AH:");
  this->getLcd()->print(String(airRelativeHumidity, 1));

  this->getLcd()->setCursor(0, 1);
  this->getLcd()->print("SM:");
  this->getLcd()->print(String(soilMoisture, 1));

  this->getLcd()->setCursor(8, 1);
  this->getLcd()->print("TL:");
  this->getLcd()->print(String(tankLevel, 1));
}

} // namespace peripherals