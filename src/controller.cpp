#include <Arduino.h>
#include "controller.h"

// Parametri de stare actuatoare
int stareActuatoareDormitor = 1;
int stareActuatoareIrene = 1;
int stareActuatoareHol = 1;
int stareActuatoareAnda = 1;

void setupGPIOs(){
    // pinMode(releuCentralaElectrica, OUTPUT);   
    pinMode(releuDormitor, OUTPUT);
    pinMode(releuIrene, OUTPUT);
    pinMode(releuHol, OUTPUT);
    pinMode(releuAnda, OUTPUT);
    
    delay(5);
    // Setam starea initiala a actuatoarelor termice ca deschise (LOW = DESCHIS, actuatorul este normal inchis)
    digitalWrite(releuDormitor, LOW);
    digitalWrite(releuIrene, LOW);
    digitalWrite(releuHol, LOW);
    digitalWrite(releuAnda, LOW);
}

void verificaTemperatura() {
    temperaturaDormitor = getTemperature(readTemperature);
    Serial.println(temperaturaDormitor);
}

// Control centrale. Se opresc sau pornesc in functie de starile termostatelor si temperatura tur puffer
void controlIncalzireDormitor(){ 
    
    int actuatorDeschis = 1;
    
    // Verificam daca sensorul de temperatura este in parametrii
    if ((readTemperature != -127) && (readTemperature != 85))
        if (readTemperature > START_TEMPERATURE)
            // Temperatura a depasit valoarea setata --> inchidem actuatoarele si oprim incalzirea
            actuatorDeschis = 0;     

    if (actuatorDeschis){
        // Actuator pe pozitie deschisa --> se deschide circuitul de incalzire       
        stareActuatoareDormitor = 1;
        digitalWrite(releuDormitor, LOW);
    }
    else
    {
        stareActuatoareDormitor = 0;
        digitalWrite(releuDormitor, HIGH);
    }
}
// Functie de control incalzire parter
void controlIncalzireIrene(){   
}

// Functie de control incalzire parter
void controlIncalzireHol(){   
}

// Functie de control incalzire parter
void controlIncalzireAnda(){   
}

void publicaStareaActuala() {
 
    // Convert to string
    String actuatoareDormitor(stareActuatoareDormitor);
    String actuatoareIrene(stareActuatoareIrene);
    String actuatoareHol(stareActuatoareHol);
    String actuatoareAnda(stareActuatoareAnda);
     
    // Publish only if we have connection to MQTT broker
    int isConnected = checkMQTT();
    if (isConnected) {
        publishMQTT(mqttTopicActuatorDormitor, actuatoareDormitor);
        publishMQTT(mqttTopicActuatorIrene, actuatoareIrene);
        publishMQTT(mqttTopicActuatorHol, actuatoareHol);
        publishMQTT(mqttTopicActuatorAnda, actuatoareAnda);
        publishMQTT(mqttTopicTemperaturaDormitor, temperaturaDormitor);
    }
}