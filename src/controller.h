#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define START_TEMPERATURE 21

// Set output GPIOs
const int releuDormitor = 32;
const int releuIrene = 21;
const int releuHol = 4;
const int releuAnda = 2;

// Topics defined on the MQTT broker
const char* mqttTopicTemperaturaDormitor = "house/etaj/dormitor/temp/value";
const char* mqttTopicActuatorDormitor = "house/etaj/dormitor/actuator/state";
const char* mqttTopicActuatorIrene = "house/etaj/irene/actuator/state";
const char* mqttTopicActuatorHol = "house/etaj/hol/actuator/state";
const char* mqttTopicActuatorAnda = "house/etaj/anda/actuator/state";

void publishMQTT(const char* mqttTopic, String value);

int checkMQTT();

// Sensori de temperatura
String temperaturaDormitor;
float readTemperature;

String getTemperature(float& readTemp);

#endif