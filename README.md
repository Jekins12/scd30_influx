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
