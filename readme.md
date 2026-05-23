# IoT Web LED Controller

[![Arduino CI](https://github.com/rajdeep13-coder/IoT-Web-LED-Controller/workflows/Arduino%20CI/badge.svg?branch=main)](https://github.com/rajdeep13-coder/IoT-Web-LED-Controller/actions)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![ESP32](https://img.shields.io/badge/ESP32-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp32)
[![ESP8266](https://img.shields.io/badge/ESP8266-Compatible-blue.svg)](https://www.espressif.com/en/products/socs/esp8266)

A lightweight web-based LED controller for ESP32 and ESP8266 boards.

It serves a clean browser UI from LittleFS, supports authentication, and exposes a small JSON API for state polling and control.

## Highlights

- Async web server for responsive control
- LittleFS-hosted UI with separate HTML and CSS
- `GET /status` JSON endpoint for current pin state
- `POST /update` for LED toggling
- mDNS discovery at `esp-led.local`
- ArduinoOTA support for wireless updates
- Standalone browser demo for quick presentation

## Preview

If you just want to show the UI without hardware, run the local demo:

```bash
python preview_server.py
```

Then open:

```text
http://127.0.0.1:8000/
```

## How It Works

The firmware:

- connects to Wi-Fi
- mounts LittleFS
- serves `index.html` and `style.css`
- returns LED status as JSON
- lets you update the pin from the browser
- keeps OTA and mDNS available on the same network

## API

### `GET /status`

Returns the current LED state:

```json
{"pin":2,"state":1}
```

### `POST /update`

Sends a state change request to the device.

## Hardware

| Component | Connection |
|---|---|
| ESP board | Wi-Fi connected |
| LED | `GPIO 2` |
| Resistor | `220Ω` |
| Jumper wires | As needed |

Typical wiring:

```text
GPIO 2 -> LED -> 220Ω resistor -> GND
```

## Requirements

- Arduino IDE or Arduino CLI
- ESP32 or ESP8266 board package
- Libraries:
  - `ESPAsyncWebServer`
  - `AsyncTCP` for ESP32 or `ESPAsyncTCP` for ESP8266

## Configuration

Put your Wi-Fi and auth values in `config.h`.

## Security

This project uses HTTP Basic Authentication over plain HTTP. That is fine for a trusted local network, but it is not encrypted. Do not expose it directly to the public internet.

## Project Layout

- `main.ino` — firmware and routes
- `config.h` — local secrets and settings
- `data/index.html` — web UI
- `data/style.css` — UI styles
- `preview_server.py` — local browser preview
- `showcase.html` — standalone demo page

## License

MIT


