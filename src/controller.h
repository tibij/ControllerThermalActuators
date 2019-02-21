#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define START_TEMPERATURE 23

// Set output GPIOs. Checked and 4, 16, 17, 21, 22, 25, 26, 32, 33 are ok
const int releuDormitor = 32; // 
// const int releuIrene = 17;
const int releuBirou = 21;
// const int releuAnda = 27;

// Topics defined on the MQTT broker
const char* mqttTopicTemperaturaDormitor = "house/etaj/dormitor/temp/value";
const char* mqttTopicActuatorDormitor = "house/etaj/dormitor/actuator/state";
// const char* mqttTopicActuatorIrene = "house/etaj/irene/actuator/state";

const char* mqttTopicActuatorBirou = "house/etaj/birou/actuator/state";

// const char* mqttTopicActuatorAnda = "house/etaj/anda/actuator/state";

void publishMQTT(const char* mqttTopic, String value);

int checkMQTT();

// Sensori de temperatura
String temperaturaDormitor;

float readTemperature = 24.0;

String getTemperature(float& readTemp);

#endif