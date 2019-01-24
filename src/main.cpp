#include <Arduino.h>
#include <WiFi.h>
#include "main.h"

void setup() {
    // setup serial port
    Serial.begin(115200);

    // setup WiFi
    setupWiFi();

    // setup MQTT connection
    setupMQTT();

    // setup device pins
    setupGPIOs();

    // setup OneWire bus for reading temperature
    setupOneWireBus();
}

void loop() {

    Serial.println("Citeste temperatura dormitor ...");
    verificaTemperatura();
    
    // Porneste/opreste incalzirea in dormitor
    Serial.println("Setup dormitor ...");
    controlIncalzireDormitor();

    Serial.println("Publica stare curenta incalzire ...");
    publicaStareaActuala();
 
    delay(5000);
}