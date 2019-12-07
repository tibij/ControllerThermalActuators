#include <Arduino.h>
#include <WiFi.h>
#include "main.h"

void setup()
{
    // setup serial port
    Serial.begin(115200);

    // Enable debug messages
    Serial.setDebugOutput(true);
    esp_log_level_set("*", ESP_LOG_VERBOSE);

    // setup WiFi
    setupWiFi();

    // setup MQTT connection
    setupMQTT();

    // setup device pins
    setupGPIOs();

    // setup OneWire bus for reading temperature
    setupOneWireBus();
}

void loop()
{

    currentMillis = millis(); //get the current time

    checkMQTT();

    if (currentMillis - startMillis >= period) //test whether the period has elapsed
    {
        // Porneste/opreste incalzirea in dormitor
        // Calculam media temperaturilor citite
        if (contorDormitor < 10)
        {
            Serial.print("Citeste temperatura dormitor ... :");
            verificaTemperatura();
            sumSerieDormitor += readTemperature;
            contorDormitor++;
            Serial.print("Contor dormitor: ");
            Serial.println(contorDormitor);
        }
        else
        {
            readTemperature = sumSerieDormitor / 10;
            contorDormitor = 0;
            sumSerieDormitor = 0;
            Serial.print("Temperatura medie dormitor:");
            Serial.println(readTemperature);

            Serial.println("Setup dormitor ...");
            controlIncalzireDormitor();
        }
        startMillis = currentMillis;
    }
}