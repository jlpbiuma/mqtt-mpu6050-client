#include "functions.h"

#define TIMEOUT 5000
#define AIO_TOPIC_ACCE_X "icedeal/feeds/AccelerationX"
#define AIO_TOPIC_ACCE_Y "icedeal/feeds/AccelerationY"
#define AIO_TOPIC_ACCE_Z "icedeal/feeds/AccelerationZ"
#define AIO_TOPIC_TEMP "icedeal/feeds/Temperature"

// LOW LEVEL FUNCTIONS

String generateGUID() {
  return String(random(0xFFFFFFFF), HEX) + String(random(0xFFFFFFFF), HEX) + String(random(0xFFFFFFFF), HEX) + String(random(0xFFFFFFFF), HEX);
}

void publish_sensor_value(PubSubClient& client, const char* topic, float value, int step_publish) {
  // Convert float value to string
  String valueStr = String(value);
  // Publish the message to the MQTT topic
  delay(step_publish);
  client.publish(topic, valueStr.c_str());
}

// HIGH LEVEL FUNCTIONS

void connect_wifi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(TIMEOUT);
    Serial.print(".");
  }
  Serial.println("Connected!");
}

void connect_mpu(Adafruit_MPU6050 &mpu) {
    while (!mpu.begin()) {
    Serial.println("MPU6050 not connected!");
    delay(TIMEOUT);
  }
  Serial.println("MPU6050 ready!");
}

void connect_mqtt_broker(PubSubClient& client, const char* username, const char* key, const char* server, int port) {
  client.setServer(server, port);
  if (client.connect(generateGUID().c_str(), username, key)) {
    Serial.println("Connected to MQTT broker");
  } else {
    Serial.println("Failed to connect to MQTT broker");
  }
}

void reconnect(PubSubClient &client, const char* username, const char* key) {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.println("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(generateGUID().c_str(), username, key)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(client.state());
      Serial.println(" Retrying in 5 seconds...");
      // Wait 5 seconds before retrying
      delay(TIMEOUT);
    }
  }
}

void read_mpu_values(Adafruit_MPU6050 &mpu, sensors_event_t &acceleration, sensors_event_t &gyro, sensors_event_t &temperature) {
  mpu.getEvent(&acceleration, &gyro, &temperature);
}

void publish_sensor_data(PubSubClient& client, const sensors_event_t& accel, const sensors_event_t& temp, int step_publish) {
  // Publish individual sensor values
  publish_sensor_value(client, AIO_TOPIC_ACCE_X, accel.acceleration.x, step_publish);
  publish_sensor_value(client, AIO_TOPIC_ACCE_Y, accel.acceleration.y, step_publish);
  publish_sensor_value(client, AIO_TOPIC_ACCE_Z, accel.acceleration.z, step_publish);
  publish_sensor_value(client, AIO_TOPIC_TEMP, temp.temperature, step_publish);
}

