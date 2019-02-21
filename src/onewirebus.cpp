#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4   // DS18B20 pin 27

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

void setupOneWireBus() {
  // setup OneWire bus
  DS18B20.begin();
}

String getTemperature(float& readTemp) {  

  DS18B20.setResolution(12);
  DS18B20.requestTemperatures();

  readTemp = DS18B20.getTempCByIndex(0);
  Serial.println(readTemp);

  if ((readTemp == -127) || (readTemp == 85))
    // Senzorul de temperatura nu e in parametri.
    // Setam o temperatura astfel incat sa ne asiguram ca incalzirea e pornita  
    readTemp = 15.00;
  
  // Convert temperature to a string with two digits before the comma and 2 digits for precision
  char convertedTemperature[8];
  snprintf(convertedTemperature, sizeof(convertedTemperature), "%.2f", readTemp);
  
  //dtostrf(readTemp, 5, 2, convertedTemperature);
  return (String)convertedTemperature;
}