#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4   // DS18B20 pin 27

extern float readTemperature;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


void setupOneWireBus() {
  // setup OneWire bus
  DS18B20.begin();
}

void getTemperature() {  
  DS18B20.setResolution(12);
  DS18B20.requestTemperatures();

  readTemperature = DS18B20.getTempCByIndex(0);

  if ((readTemperature == -127) || (readTemperature == 85))
    // Senzorul de temperatura nu e in parametri.
    // Setam o temperatura astfel incat sa ne asiguram ca incalzirea e pornita  
    readTemperature = 15.00;
}