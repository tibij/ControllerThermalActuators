#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "mqtt.h"

void callback(char* topic, byte* payload, unsigned int length) {
  char temp[8];
  float inTemp;

  // Allocate the correct amount of memory for the payload copy
  byte* p = (byte*)malloc(length);
  // Copy the payload to the new buffer to avoid conflict with another operation
  memcpy(p,payload,length);  

  Serial.print("Message with lenght ");
  Serial.print(length);
  Serial.print(" arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)p[i]);
    temp[i] = (char)p[i];
  }

  Serial.println();
  temp[length] = '\0';
  // Convert temperature to float
  inTemp = atof(temp);
  
  if (strcmp(topic, mqttTopicTemperaturaIrene) == 0){
    tempInIrene = inTemp;
    controlIncalzireIrene();
  }
  if (strcmp(topic, mqttTopicTemperaturaBirou) == 0){
    tempInBirou = inTemp;
    controlIncalzireBirou();
  }
  if (strcmp(topic, mqttTopicTemperaturaDormitorCeruta) == 0){
    tempProgramataDormitor = inTemp;
    programareIncalzireDormitor();
  }
  if (strcmp(topic, mqttTopicTemperaturaIreneCeruta) == 0){
    tempProgramataIrene = inTemp;
    programareIncalzireIrene();
  }
  if (strcmp(topic, mqttTopicTemperaturaBirouCeruta) == 0){
    tempProgramataBirou = inTemp;
    programareIncalzireBirou();
  }

  // Free the memory
  free(p);
}

int reconnect() {
  // Check if WiFi connection is still up and if not try to reconnect
  int wifiStatus = WiFi.status();
   
  if (wifiStatus != WL_CONNECTED) {
    Serial.print("Attempting to reconnect to WiFi...");
    wifiStatus = setupWiFi();
  }
 
  if (wifiStatus == WL_CONNECTED)
  {
    Serial.println("Attempting MQTT PUB connection...");
    // Attempt to connect
    if (mqttClient.connect("ATMQTTPUB")) {
      Serial.println("Connected to MQTT broker as ATMQTTPUB client");
      publishMQTT(mqttTopicAlive, "ATMQTTPUB");
      //subscribeMQTT(mqttTopicTemperaturaDormitorCeruta);
      subscribeMQTT(mqttTopicTemperaturaIrene);
      //subscribeMQTT(mqttTopicTemperaturaIreneCeruta);
      subscribeMQTT(mqttTopicTemperaturaBirou);
      //subscribeMQTT(mqttTopicTemperaturaBirouCeruta);
    }
    else
      ESP.restart();
  }
  return mqttClient.connected();
}

int checkMQTT() {  
  // Non-blocking reconnect. If the client loses
  // its connection, it attempts to reconnect every 5 seconds
  // without blocking the main loop.
  int isConnected = mqttClient.connected();
  if (!isConnected) {
    long now = millis();
    if (now - lastReconnectAttemptPub > 5000) {
      lastReconnectAttemptPub = now;
      isConnected = reconnect();
      // Attempt to reconnect
      if (isConnected) {
        lastReconnectAttemptPub = 0;
      }
    }      
  }
  else
    // Client connected
    mqttClient.loop();

  return isConnected;
}

void setupMQTT() {
  mqttClient.setServer(mqttServer, 1883);
  mqttClient.setCallback(callback);
  
  // Estabilish initial connection
  checkMQTT();  
}

void publishMQTT(const char* mqttTopic, const char* value) {
    
    int result;
    // Publish only if connection is fine to MQTT broker
    if ( mqttClient.connected() ) {
        mqttClient.loop();
        Serial.print("Topic: ");
        Serial.print(mqttTopic);
        Serial.print(" ");
        Serial.print(value);    
        result = mqttClient.publish(mqttTopic, value, 1);
        
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

int getMQTTClientStaus(){

  return mqttClient.state();

}

int getMQTTClientConnectedState() {
  return mqttClient.connected();
}

int keepMQTTAlive(){
  return mqttClient.loop();
}
