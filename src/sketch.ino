#include <Wire.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
// Custom libraries
#include "functions.h"

// SETUP
// Variables declaration
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVER_PORT 1883
#define AIO_USERNAME "icedeal"
#define AIO_KEY "aio_zhCW75P50m76FqF1zCj91G5Sf7CI"
#define STEP_PUBLISH 1000 // Time for each mqtt request
#define CLOCK 2000 // Time to continue the cycle
// Create an MQTT client instance
WiFiClient espClient;
PubSubClient client(espClient);
// Initialize MPU6050
Adafruit_MPU6050 mpu;
// SETUP FUNCTION
void setup(void) {
  Serial.begin(115200);
  // Connect to Wi-Fi
  connect_wifi();
  // Connect to MPU6050
  connect_mpu(mpu);
  // Connect to MQTT Broker
  connect_mqtt_broker(client, AIO_USERNAME, AIO_KEY, AIO_SERVER, AIO_SERVER_PORT);
}

//MAIN LOOP FUNCTION
void loop() {
  // Reconnect to MQTT if necessary
  if (!client.connected()) {
    reconnect(client, AIO_USERNAME, AIO_KEY);
  }
  // Create the events to read the sensor values
  sensors_event_t a, g, t;
  // Read sensor values
  read_mpu_values(mpu, a, g, t);
  // Publish acceleration and temperature
  publish_sensor_data(client, a, t, STEP_PUBLISH);
  delay(CLOCK);
}


