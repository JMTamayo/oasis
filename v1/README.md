# Oasis v1 🪴

> Part of the [Oasis monorepo](../README.md). This is the **v1** package: the first prototype.

This prototype allows monitoring of plant humidity, temperature and relative air humidity. It was developed as a hobby, applying knowledge of circuits, microcontrollers, sensors, actuators and software development.

The microcontroller publishes the measured values to an MQTT server, which can be accessed from a web interface, mobile app, desktop app or any other device that supports MQTT. The values are published as follows:

- **Schema**: `/oasis/<device-id>/v1/<topic>`
- **Topics**:
  - `air-temperature`: Air temperature [°C].
  - `air-humidity`: Air humidity [%].
  - `soil-moisture-1`: Soil moisture [%: 0% (wet) - 100% (dry)].
  - `soil-moisture-2`: Soil moisture [%: 0% (wet) - 100% (dry)].
  - `average-soil-moisture`: Average soil moisture [%: 0% (wet) - 100% (dry)], calculated as the average of the two sensors.
  - `geolocation`: Geolocation of the device, obtained from the network access point [JSON format].
  - `error`: Error message, published when the microcontroller fails to read the sensors [String].

The device can be controlled by sending the following commands to the MQTT server:

- **Schema**: `/oasis/<device-id>/v1/<command>`
- **Commands**:
  - `ping`: Ping the device, the response is a pong message.
  - `restart-device`: Restart the device.
  - `set-sampling-time`: Set the sampling time [ms].
  - `get-sampling-time`: Get the sampling time [ms].
  - `set-measurements-publication-time`: Set the measurements publication time [ms].
  - `get-measurements-publication-time`: Get the measurements publication time [ms].
  - `get-pending-time-for-measurements-publication`: Get the pending time for measurements publication [ms].

## Prototype

  
***Figure 1.** Oasis prototype, featuring sensors and a display for plant monitoring..*

### ⚙️ Hardware:

The prototype is composed by the following hardware elements:

- **[DEVKIT-ESP32-30PIN-CH340](https://www.didacticaselectronicas.com/shop/devkit-esp32s-30pin-ch340-tarjeta-de-desarrollo-devkit32-esp32s-ch340-39069?search=DEVKIT-ESP32-30PIN-CH340&order=name+asc#attr=)**: Development board for ESP32 with 30 pins and integrated WiFi module (1 UND).
- **[DHT22-ECO](https://www.didacticaselectronicas.com/shop/dht22-eco-sensor-de-humedad-y-temperatura-dht22-educativo-generico-39484?search=DHT22&order=name+asc#attr=)**: Temperature and humidity sensor (1 UND).
- **[SEN0193](https://www.didacticaselectronicas.com/shop/sen0193-sensor-de-humedad-de-suelo-capacitivo-5537?search=SEN0193&order=name+asc#attr=)**: A capacitive soil moisture sensor (2 UND).
- **[SR602](https://www.mercadolibre.com.co/sensor-de-movimiento-pir-mhsr602-sr602/up/MCOU2434760476#polycard_client=search-desktop&search_layout=grid&position=1&type=product&tracking_id=fcc67615-078f-4de4-941c-b6644d5ce27a&wid=MCO630276479&sid=search)**: Infrared motion sensor (1 UND).
- **[OLED-0.96-AM-AZ-SSD1306](https://www.didacticaselectronicas.com/shop/oled-0-96-am-az-ssd1306-display-oled-0-96-pulgadas-amarillo-azul-24062?search=OLED+&order=name+asc#attr=)**: Display for plant monitoring (1 UND).
- **[S-10-5](https://www.didacticaselectronicas.com/shop/s-10-5-fuente-suicheada-5v-2a-10w-3053?search=fuente+suicheada+5V+2A&order=name+asc#attr=2031,2032,2034,2035,2038,2036,2037,2033)**: With a 5V output and a 2A maximum current capacity (1 UND).
- **[RP-220R/1/4W](https://www.didacticaselectronicas.com/shop/rp-220r-1-4w-resistencia-de-precision-de-220r-1-4w-17012?search=resistencia+220&order=name+asc#attr=)**: 220Ω 1/4W Precision Resistor (1 UND).
- **[BOB-12009](https://www.didacticaselectronicas.com/shop/bob-12009-convertidor-de-nivel-logico-bidireccional-1451?search=convertidor+logico&order=name+asc#attr=)**: Bidirectional Logic Level Converter (1 UND).
- **[LED-5-B-D](https://www.didacticaselectronicas.com/shop/led-5-b-d-led-5mm-difuso-azul-432?search=led+azul&order=name+asc#attr=)**: Diffused LED for standard lighting or indication applications (1 UND).
- **[CA-PODER](https://www.didacticaselectronicas.com/shop/ca-poder-cable-de-poder-redondo-1-5m-20660?search=cable+alimentacion&order=name+asc#attr=)**: 1.5m Round Power Cable (1 UND).
- **[PCB-BOARD](assets/OASIS_PCB.png)**: PCB board for the prototype (1 UND).
- Cables and connectors for circuit assembly.
- Wooden board 20cm x 20cm and screws for fixing the elements.

### 💻 Software:

The software was developed using the following tools:

- **[Cursor IDE](https://cursor.com)**: A powerful AI-powered code editor.
- **[PlatformIO](https://platformio.org/)**: Cross-platform build system for embedded system projects.
- **[C++](https://es.wikipedia.org/wiki/C%2B%2B)**: Programming language for the firmware.

The firmware was developed using the microcontroller's two cores. The first core manages the connection to the Wi-Fi service and the MQTT server, and obtains geolocation information from the network access point. The second core reads sensor data and displays the values ​​on the integrated display. The two cores communicate using a task queuing system.

When a message is received from the MQTT server on any of the subscribed topics, the integrated LED flashes to indicate that the device is receiving messages correctly.

The integrated display shows the status of the Wi-Fi and MQTT server connections, as well as the values ​​measured by the sensors, whose sampling time can be adjusted. If the motion sensor does not detect movement, the display will turn off to save power. When it detects movement, it automatically turns on.

### ⚡ Electrical Design:

The prototype schematic is shown in the following figure. It was designed using [Autodesk Fusion 360](https://www.autodesk.com/latam/products/fusion-360/overview) software with a [student license](https://www.autodesk.com/education/edu-software/fusion).

  
***Figure 2.** Schematic diagram of the prototype's electrical circuit.*

## Contributing

We welcome contributions to this project. Here are some ways you can contribute:

- 🐛 **Bug Reports**: If you find a bug, please create an issue detailing the problem, the steps to reproduce it, and the expected behavior.
- 💡 **Feature Requests**: If you have an idea for a new feature or an enhancement to an existing one, please create an issue describing your idea.
- 🚀 **Pull Requests**: If you've fixed a bug or implemented a new feature, we'd love to see your work! Please submit a pull request. Make sure your code follows the existing style and all tests pass.
- 💬 **Recommendations**: If you have any recommendations for improvements, please share them in the issues section.

Thank you for your interest in improving Oasis! 🌱💛