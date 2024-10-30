# SCD30 InfluxDB Data Logger

This project is an Arduino-based program for logging data from an SCD30 sensor (CO₂, temperature, and humidity) to an InfluxDB database. The program is designed to run on either an ESP8266 or ESP32 microcontroller, and data is sent to InfluxDB via Wi-Fi.

## Features

- Reads CO₂, temperature, and humidity from the SCD30 sensor.
- Calculates air quality based on CO₂ levels.
- Sends data to an InfluxDB instance.
- Provides an HTTP endpoint (`/discover`) to view device information.

## Hardware Requirements

- ESP8266 or ESP32 microcontroller
- SCD30 sensor (for CO₂, temperature, and humidity measurements)

## Software Requirements

- Arduino IDE (with ESP8266 or ESP32 board support)
- InfluxDB
- Wi-Fi network

## Installation

1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/scd30_influx.git
   cd scd30_influx
2. Install required Arduino libraries:

   - [ESP8266WiFi](https://github.com/esp8266/Arduino)
   - [ESP8266WebServer](https://github.com/esp8266/Arduino)
   - [ArduinoJson](https://arduinojson.org/)
   - [InfluxDbClient](https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino)
   - [Adafruit SCD30](https://github.com/adafruit/Adafruit_SCD30)
     
3. Configure your settings in `config.h`:

   - Update `WIFI_SSID` and `WIFI_PASSWORD` with your Wi-Fi credentials.
   - Set your InfluxDB URL, token, organization, and bucket.
   - Set the `MEASUREMENT_INTERVAL` for sensor readings (in seconds).

4. Upload the code to your ESP8266 or ESP32 using the Arduino IDE.

# Usage 

  - Once the program is running, it will connect to your Wi-Fi network and start collecting data from the SCD30 sensor.
  - Data is sent to your InfluxDB database at the specified measurement interval.
  - You can access the device information by navigating to http://<device-ip>/discover in your browser.

# Air Quality Calculation

   Air quality is rated based on CO₂ levels:
  - 1: Excellent (CO₂ < 700 ppm)
  - 2: Good (700 ≤ CO₂ < 2000 ppm)
  - 3: Fair (2000 ≤ CO₂ < 3000 ppm)
  - 4: Poor (3000 ≤ CO₂ < 4000 ppm)
  - 5: Very Poor (CO₂ ≥ 4000 ppm)
