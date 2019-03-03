#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt.h"

void callback(char* topic, byte* payload, unsigned int length) {
  
  char temp[length];

  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    temp[i] = (char)payload[i];
  }
  temp[length] = '\0';
  Serial.println();

  if (strcmp(topic, mqttTopicTemperaturaDormitorCeruta) == 0)
    programareIncalzireDormitor(temp); 
  if (strcmp(topic, mqttTopicTemperaturaBirou) == 0)
    controlIncalzireBirou(temp);
  if (strcmp(topic, mqttTopicTemperaturaBirouCeruta) == 0)
    programareIncalzireBirou(temp);
  if (strcmp(topic, mqttTopicTemperaturaIrene) == 0)
    controlIncalzireIrene(temp);
  if (strcmp(topic, mqttTopicTemperaturaIreneCeruta) == 0)
    programareIncalzireIrene(temp);

  // Subscrierea e unica asa ca putem sa trimitem comanda direct
  // In caz ca vor fi mai multe subscierei va trebui refacut
  controlIncalzireBirou(temp);
}

int reconnect() {

  // Check if WiFi connection is still up and if not try to reconnect
  int wifiStatus = WiFi.status();
  if (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to reconnect to WiFi...");
    wifiStatus = setupWiFi();
  }

  // Loop until we're reconnected but stop after 3 attempts or no Internet connection 
  int retryCount = 0;
  while (!mqttClient.connected() && (retryCount < 3) && (wifiStatus == WL_CONNECTED) )  {
    Serial.println("Attempting MQTT connection...");
    
    // Attempt to connect
    if (mqttClient.connect("ESP32ACTUATORE")) {
      Serial.println("Connected to MQTT broker");
      subscribeMQTT(mqttTopicTemperaturaDormitorCeruta);
      subscribeMQTT(mqttTopicTemperaturaIrene);
      subscribeMQTT(mqttTopicTemperaturaIreneCeruta);
      subscribeMQTT(mqttTopicTemperaturaBirou);
      subscribeMQTT(mqttTopicTemperaturaBirouCeruta);

    } else {
      Serial.print("Failed to connect to MQTT broker, return code = ");
      Serial.print(mqttClient.state());
      Serial.println(" will try again in 3 seconds");

      // Wait 3 seconds before retrying
      delay(3000);
    }
    retryCount++;
  }
  return mqttClient.connected();
}

int checkMQTT() {  
  int isConnected = mqttClient.connected();
  if (!isConnected)
      isConnected = reconnect();
  else
      mqttClient.loop();
  
  return isConnected;
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback);
}

void publishMQTT(const char* mqttTopic, String value) {
    
    int result;
    char valueString[value.length()];

    // Publish only if connection is fine to MQTT broker
    if ( mqttClient.connected() ) {
        
        mqttClient.loop();

        value.toCharArray(valueString,value.length());
        
        Serial.print("Topic: ");
        Serial.print(mqttTopic);
        Serial.print(" ");
        Serial.print(value);
        
        result = mqttClient.publish(mqttTopic, valueString, 1);
        
        if (result)
          Serial.println(" Publish: SUCCESS");
        else
          Serial.println(" Publish: FAILED ");
    }
}

void subscribeMQTT(const char* mqttTopic) {
    
    int result;
    
    Serial.println("Subscribing ...");

    // Subscribe only if connection is fine to MQTT broker
    if ( mqttClient.connected() ) {
        
        mqttClient.loop();
        
        Serial.print("Subscribe to topic: ");
        Serial.print(mqttTopic);
        
        result = mqttClient.subscribe(mqttTopic);
        
        if (result)
          Serial.println(" Subscribe: SUCCESS");
        else
          Serial.println(" Subscribe: FAILED ");
    }
    else
        Serial.println("Could not subscribe. Not connected to MQTT broker.");
}
