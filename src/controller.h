#include <Arduino.h>

#ifndef CONTROLLER_H
#define CONTROLLER_H

#define PRESET_TEMPERATURE_DORMITOR 22
#define PRESET_TEMPERATURE_BIROU 21
#define PRESET_TEMPERATURE_IRENE 22

void programareIncalzireDormitor();
void controlIncalzireBirou();
void programareIncalzireBirou();
void controlIncalzireIrene();
void programareIncalzireIrene();

// Set output GPIOs. Checked and 4, 16, 17, 21, 22, 25, 26, 32, 33 are ok
const int releuDormitor = 32; //
const int releuIrene = 25;
const int releuBirou = 27;
// const int releuAnda = 21;

// Parametri de stare actuatoare
int stareActuatoareDormitor = 1;
int stareActuatoareIrene = 1;
int stareActuatoareBirou = 1;
int stareActuatoareAnda = 1;

// Parameri mod de lucru: presetat sau cu temperatura programata
// Implicit este presetat adica temperatura de control este presetata in aplicatie
int modPresetatDormitor = 1;
int modPresetatIrene = 1;
int modPresetatBirou = 1;
int modPresetatAnda = 1;

// Variabile pentru memorarea temperaturi programate de la distanta
float tempProgramataDormitor;
float tempProgramataBirou;
float tempProgramataIrene;
// float tempProgramataAnda;
float tempInBirou;
float tempInIrene;
// float tempInAnda;

extern const char *mqttTopicTemperaturaDormitor;
extern const char *mqttTopicActuatorDormitor;
extern const char *mqttTopicTemperaturaDormitorCeruta;
extern const char *mqttTopicTemperaturaBirou;
extern const char *mqttTopicActuatorBirou;
extern const char *mqttTopicTemperaturaBirouCeruta;
extern const char *mqttTopicTemperaturaIrene;
extern const char *mqttTopicActuatorIrene;
extern const char *mqttTopicTemperaturaIreneCeruta;

// extern const char* mqttTopicTemperaturaAnda;
// extern const char* mqttTopicActuatorAnda;
// extern const char* mqttTopicTemperaturaAndaCeruta;

void publishMQTT(const char *mqttTopic, const char *value);

float readTemperature = 24.0;

void getTemperature();

#endif