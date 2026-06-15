#ifndef OLED_SSD1306_H
#define OLED_SSD1306_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

namespace peripherals {

/**
  OledSsd1306 class.

  This class is used to represent a OLED SSD1306 display that can be used to
  display the data received from the sensors and the connections status.
 */
class OledSsd1306 {
private:
  Adafruit_SSD1306 *display; // The display object.

  unsigned int width;  // The width of the display.
  unsigned int height; // The height of the display.

  const char *projectName; // The project name.
  const char *deviceId;    // The device ID.

  /**
    Get the display object.

    Returns:
      The display object.
   */
  Adafruit_SSD1306 *getDisplay();

  /**
    Get the width of the display.

    Returns:
      The width of the display.
   */
  unsigned int getWidth();

  /**
    Get the height of the display.

    Returns:
      The height of the display.
   */
  unsigned int getHeight();

  /**
    Get the header.

    Returns:
      The header, which is the project name and the device ID.
   */
  String getHeader();

  /**
    Convert a float to a string with 6 decimal places.

    Parameters:
      num - The float to convert.

    Returns:
      The string representation of the float. If the float has more than 6
      decimal places, it will be truncated. If the float has less than 6
      decimal places, it will be padded with zeros.
   */
  String floatToString_6(float num);

public:
  /**
    Constructor.

    Parameters:
      width - The width of the display.
      height - The height of the display.
      address - The address of the display.
      projectName - The project name.
      deviceId - The device ID.
   */
  OledSsd1306(unsigned int width, unsigned int height, unsigned int address,
              const char *projectName, const char *deviceId);

  /**
    Destructor.
   */
  ~OledSsd1306();

  /**
    Begin the display.

    Parameters:
      address - The address of the display.

    Returns:
      True if the display is successfully initialized, false otherwise.
   */
  bool Begin(unsigned int address);

  /**
    Display the data on the display.

    Parameters:
      wifiConnected - The WiFi connection status.
      mqttConnected - The MQTT connection status.
      airTemperature - The air temperature in degrees Celsius.
      airHumidity - The air humidity in percentage.
      averageSoilMoisture - The average soil moisture in percentage.
   */
  void Display(bool wifiConnected, bool mqttConnected, float airTemperature,
               float airHumidity, float averageSoilMoisture);

  /**
    Light up the display.

    Parameters:
      on - True to light up the display, false to turn it off.
   */
  void Lighting(bool on);
};

} // namespace peripherals

#endif