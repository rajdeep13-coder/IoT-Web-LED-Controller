# IoT Web LED Controller

[![Arduino CI](https://github.com/rajdeep13-coder/IoT-Web-LED-Controller/workflows/Arduino%20CI/badge.svg?branch=main)](https://github.com/rajdeep13-coder/IoT-Web-LED-Controller/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![ESP8266](https://img.shields.io/badge/ESP8266-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp8266)
[![GitHub stars](https://img.shields.io/github/stars/rajdeep13-coder/IoT-Web-LED-Controller?style=social)](https://github.com/rajdeep13-coder/IoT-Web-LED-Controller/stargazers)
[![GitHub forks](https://img.shields.io/github/forks/rajdeep13-coder/IoT-Web-LED-Controller?style=social)](https://github.com/rajdeep13-coder/IoT-Web-LED-Controller/network/members)

Control an ESP32 or ESP8266 GPIO pin (LED) directly from a web browser using an asynchronous web server.  
The project provides real-time LED control with authentication and WiFi fail-safe protection.

---

## Overview

This project demonstrates how to build a lightweight IoT web interface on ESP32/ESP8266 using an asynchronous HTTP server.  
It allows authenticated users to toggle a GPIO pin (LED) from any browser connected to the same network.

Designed to be:
- Simple to deploy
- Fast and non-blocking
- Easy to extend for real IoT applications

---

## Security Notice

This project uses HTTP Basic Authentication over plain HTTP. Basic Auth credentials are only base64-encoded, not encrypted, so they can be sniffed by anyone on the same network. Do not expose this device directly to untrusted networks. If you need stronger protection, place it behind TLS terminated by a reverse proxy or another secure gateway.

---

## Features

- Asynchronous web server (non-blocking)
- HTTP Basic Authentication
- Instant LED state update without page reload
- ESP32 and ESP8266 compatible
- WiFi connection timeout protection
- Single `.ino` file for easy upload
- Minimal hardware requirements

---

## Hardware Requirements

| Component | Pin | Description |
|---------|-----|------------|
| ESP32 Dev Board | GPIO 2 | Built-in LED |
| External LED | GPIO 2 | Optional |
| Resistor | 220Ω | Current limiting |
| Jumper Wires | — | Connections |

**Connection:**  
GPIO 2 → LED → 220Ω resistor → GND

---

## Software Requirements

- Arduino IDE (latest recommended)
- ESP32 or ESP8266 board support installed
- Required libraries:
  - ESPAsyncWebServer
  - AsyncTCP (ESP32) or ESPAsyncTCP (ESP8266)

---


