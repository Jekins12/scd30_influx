#if defined(ESP32)
#include <WiFiMulti.h>
WiFiMulti wifiMulti;
#define DEVICE "ESP32"
#elif defined(ESP8266)
#include <ESP8266WiFiMulti.h>
ESP8266WiFiMulti wifiMulti;
#define DEVICE "ESP8266"
#endif

#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <InfluxDbClient.h>
#include <InfluxDbCloud.h>
#include "config.h"

#include <Adafruit_SCD30.h>
Adafruit_SCD30 scd30;

// Declare InfluxDB client instance with preconfigured InfluxCloud certificate
InfluxDBClient client(INFLUXDB_URL, INFLUXDB_ORG, INFLUXDB_BUCKET, INFLUXDB_TOKEN, InfluxDbCloud2CACert);

ESP8266WebServer server(80);

// Declare Data points
Point scd30Data("scd30_data");

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  wifiMulti.addAP(WIFI_SSID, WIFI_PASSWORD);

  Serial.print("Connecting to wifi");
  while (wifiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  timeSync(TZ_INFO, "pool.ntp.org", "time.nis.gov");

  // Check server connection
  if (client.validateConnection()) {
    Serial.print("Connected to InfluxDB: ");
    Serial.println(client.getServerUrl());
  } else {
    Serial.print("InfluxDB connection failed: ");
    Serial.println(client.getLastErrorMessage());
  }

  if (!scd30.begin()) {
    Serial.println("Failed to find SCD30 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("SCD30 Found!");
  scd30.setMeasurementInterval(MEASUREMENT_INTERVAL);
  Serial.print("Measurement Interval: ");
  Serial.print(scd30.getMeasurementInterval());
  Serial.println(" seconds");

  // Define HTTP endpoints
  server.on("/discover", HTTP_GET, handleDiscover);

  // Start server
  server.begin();
  Serial.println("HTTP server started. IP: "+ WiFi.localIP().toString());
}

void loop() {
  
  server.handleClient();

  if (scd30.dataReady()) {
    if (!scd30.read()) {
      Serial.println("Error reading sensor data");
      return;
    }

    // Clear fields for reusing the point. Tags will remain the same as set above.
    scd30Data.clearFields();

    int co2 = scd30.CO2;
    float temperature = (scd30.temperature);
    float humidity = (scd30.relative_humidity);
    int airQuality = calculateAirQuality(co2);

     // Format temperature and humidity with one digit after the decimal point
    String formattedTemperature = String(temperature, 1);
    String formattedHumidity = String(humidity, 1);

    scd30Data.addField("co2", co2);
    scd30Data.addField("temperature", formattedTemperature.toFloat());
    scd30Data.addField("humidity", formattedHumidity.toFloat());
    scd30Data.addField("air_quality", airQuality);

    /* Print data to the serial port
    Serial.print("CO2: ");
    Serial.print(co2);
    Serial.print(" | Temperature: ");
    Serial.print(temperature);
    Serial.print(" | Humidity: ");
    Serial.print(humidity);
    Serial.print(" | Air Quality: ");
    Serial.println(airQuality);
    */
    if (!client.writePoint(scd30Data)) {
      Serial.print("InfluxDB write failed: ");
      Serial.println(client.getLastErrorMessage());
    }
  }

  delay(100);
}

void handleDiscover() {
  StaticJsonDocument<200> doc;
  doc["name"] = "ESP8266 Air";
  doc["ip_address"] = WiFi.localIP().toString(); // Add the IP address
  doc["mac_address"] = WiFi.macAddress(); // Add the MAC address
  JsonArray endpoints = doc.createNestedArray("endpoints");
  endpoints.add("/discover");

  String response;
  serializeJson(doc, response);
  server.send(200, "application/json", response);
}

// Function to calculate air quality based on CO2 levels
int calculateAirQuality(int co2) {
  if (co2 < 700) {
    return 1; // Excellent
  } else if (co2 < 2000) {
    return 2; // Good
  } else if (co2 < 3000) {
    return 3; // Fair
  } else if (co2 < 4000) {
    return 4; // Poor
  } else {
    return 5; // Very Poor
  }
}
