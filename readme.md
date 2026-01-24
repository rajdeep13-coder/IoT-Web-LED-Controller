# IoT Web LED Controller

[![ESP32 Web Control](https://github.com/IoT-Core-Engineering/ESP-IoT-WebControl/workflows/ESP32%20Build/badge.svg)](https://github.com/IoT-Core-Engineering/ESP-IoT-WebControl/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp32)

**Control ESP32 GPIO Pin 2 (LED) via web browser** with instant toggle, authentication, and WiFi timeout protection.


## 🚀 Features
- ✅ **Async Web Server** (non-blocking, fast)
- ✅ **HTTP Basic Auth** (admin/admin)
- ✅ **Live state update** (no page reload)
- ✅ **ESP32 + ESP8266** compatible
- ✅ **WiFi timeout protection**
- ✅ **Single .ino file** (upload ready)

## 📋 Hardware Required
| Component | Pin | Notes |
|-----------|-----|-------|
| ESP32 DevKit | GPIO 2 | Built-in LED |
| LED + 220Ω | GPIO 2 | External (optional) |
| Jumper wires | - | - |

**Schematic**: GPIO 2 → LED → 220Ω → GND

