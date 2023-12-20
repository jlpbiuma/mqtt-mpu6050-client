// sensor_functions.h

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Adafruit_Sensor.h>
#include <Adafruit_MPU6050.h>
#include <PubSubClient.h>
#include <WiFi.h>

// Function declarations
void connect_wifi();
void connect_mpu(Adafruit_MPU6050 &mpu);
void connect_mqtt_broker(PubSubClient& client, const char* username, const char* key, const char* server, int port);
void reconnect(PubSubClient &client, const char* username, const char* key);
void read_mpu_values(Adafruit_MPU6050 &mpu, sensors_event_t &acceleration, sensors_event_t &gyro, sensors_event_t &temperature);
void publish_sensor_data(PubSubClient& client, const sensors_event_t& accel, const sensors_event_t& temp, int step_publish);

#endif FUNCTIONS_H
