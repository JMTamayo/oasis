# Oasis 🪴

Because your plants deserve better than *'oops, I forgot to water you'*

**Oasis** is a hobby IoT project for monitoring plant health: soil moisture, air temperature and relative air humidity. Measurements are published over MQTT so they can be consumed from a web interface, a mobile or desktop app, or any other MQTT-capable client.

This repository is organized as a **monorepo**. Each top-level version directory is a self-contained package with its own source code, build configuration and documentation. This lets the project evolve across hardware and firmware iterations while keeping the full history in one place.

## Packages

| Package | Description |
|---------|-------------|
| [`v1/`](v1) | First prototype. ESP32-based firmware (PlatformIO + Arduino) with DHT22, dual SEN0193 capacitive soil sensors, SR602 motion sensor and an SSD1306 OLED display. Publishes sensor data and accepts remote commands over MQTT. |

Each package contains its own `README.md` with the full hardware, software and setup details.

## Repository layout

```
oasis/
├── LICENSE        # License for the whole repository
├── README.md      # This file (monorepo overview)
└── v1/            # Version 1 package (firmware + hardware design)
```

## Contributing

We welcome contributions to this project. Here are some ways you can contribute:

- 🐛 **Bug Reports**: If you find a bug, please create an issue detailing the problem, the steps to reproduce it, and the expected behavior.
- 💡 **Feature Requests**: If you have an idea for a new feature or an enhancement to an existing one, please create an issue describing your idea.
- 🚀 **Pull Requests**: If you've fixed a bug or implemented a new feature, we'd love to see your work! Please submit a pull request. Make sure your code follows the existing style and all tests pass.
- 💬 **Recommendations**: If you have any recommendations for improvements, please share them in the issues section.

## License

This project is licensed under the terms of the [LICENSE](LICENSE) file.

Thank you for your interest in improving Oasis! 🌱💛
