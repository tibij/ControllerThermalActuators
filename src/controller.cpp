#include <Arduino.h>
#include "controller.h"

void setupGPIOs(){
    pinMode(releuDormitor, OUTPUT);
    pinMode(releuIrene, OUTPUT);
    pinMode(releuBirou, OUTPUT);
    // pinMode(releuAnda, OUTPUT);
    
    delay(5);
    // Setam starea initiala a actuatoarelor termice ca deschise (LOW = DESCHIS, actuatorul este normal inchis)
    digitalWrite(releuDormitor, LOW);
    digitalWrite(releuIrene, LOW);
    digitalWrite(releuBirou, LOW);
    // digitalWrite(releuAnda, LOW);
}

void verificaTemperatura() {
    temperaturaDormitor = getTemperature(readTemperature);
    Serial.println(temperaturaDormitor);
}

// Control centrale. Se opresc sau pornesc in functie de starile termostatelor si temperatura tur puffer
void controlIncalzireDormitor(){ 
    
    int actuatorDeschis = 1;
    float tempDorita;

    // Verificam modul de lucru
    if (modPresetatDormitor)
        tempDorita = PRESET_TEMPERATURE_DORMITOR;
    else
        tempDorita = tempProgramataDormitor;
    
    // Verificam daca sensorul de temperatura este in parametrii
    if ((readTemperature != -127) && (readTemperature != 85))
        if (readTemperature > tempDorita)
            // Temperatura a depasit valoarea presetata --> inchidem actuatoarele si oprim incalzirea
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
// Functie de control incalzire camera Irene
void controlIncalzireIrene(char* tempValue){
    int actuatorDeschis = 1;
    float temp = atof(tempValue);
    float tempDorita;

    // Verificam modul de lucru
    if (modPresetatIrene)
        tempDorita = PRESET_TEMPERATURE_IRENE;
    else
        tempDorita = tempProgramataIrene;    

    if (temp > tempDorita)
        // Temperatura a depasit valoarea presetata --> inchidem actuatoarele si oprim incalzirea
        actuatorDeschis = 0;
    
    if (actuatorDeschis){
        // Actuator pe pozitie deschisa --> se deschide circuitul de incalzire       
        stareActuatoareIrene = 1;
        digitalWrite(releuIrene, LOW);
    }
    else
    {
        stareActuatoareIrene = 0;
        digitalWrite(releuIrene, HIGH);
    }
    Serial.print("Temperatura Irene: ");
    Serial.println(tempValue);
    Serial.print("Actuator Irene: ");
    Serial.println(stareActuatoareIrene);
}

// Functie de control incalzire birou si hol
void controlIncalzireBirou(char* tempValue){

    int actuatorDeschis = 1;
    
    float temp = atof(tempValue);
    float tempDorita;

    // Verificam modul de lucru
    if (modPresetatBirou)
        tempDorita = PRESET_TEMPERATURE_BIROU;
    else
        tempDorita = tempProgramataBirou;

    if (temp > tempDorita)
            // Temperatura a depasit valoarea setata --> inchidem actuatoarele si oprim incalzirea
            actuatorDeschis = 0;
    
    if (actuatorDeschis){
        // Actuator pe pozitie deschisa --> se deschide circuitul de incalzire       
        stareActuatoareBirou = 1;
        digitalWrite(releuBirou, LOW);
    }
    else
    {
        stareActuatoareBirou = 0;
        digitalWrite(releuBirou, HIGH);
    }
    Serial.print("Temperatura birou: ");
    Serial.println(tempValue);
    Serial.print("Actuator birou: ");
    Serial.println(stareActuatoareBirou);
}

// Setare mod de incalzire dormitor: programata sau presetata
void programareIncalzireDormitor(char* tempValue){
    float temp = atof(tempValue);

    // Daca temperatura este mai mare sau egala cu 100 inseamna ca modul de lucru este presetat
    if (temp < 100) {
        // Trecem pe mod programat
        modPresetatDormitor = 0;
        tempProgramataDormitor = temp;
        controlIncalzireDormitor();
    }
    else
    {
         // Trecem pe mod presetat
        modPresetatDormitor = 1;   
    }
}

// Setare mod de incalzire birou: programata sau presetata
void programareIncalzireBirou(char* tempValue){
    float temp = atof(tempValue);
    Serial.print("TEMP BIROU PROGRAMATA: ");
    Serial.println(temp);
    // Daca temperatura este mai mare sau egala cu 100 inseamna ca modul de lucru este presetat
    if (temp < 100) {
        // Trecem pe mod programat
        Serial.println("BIROU MOD PROGRAMAT");
        modPresetatBirou = 0;
        tempProgramataBirou = temp;
        controlIncalzireBirou(tempValue);
    }
    else
    {
         // Trecem pe mod presetat
        modPresetatBirou = 1;   
    }    
}

// Setare mod de incalzire Irene: programata sau presetata
void programareIncalzireIrene(char* tempValue){
    float temp = atof(tempValue);
    
    // Daca temperatura este mai mare sau egala cu 100 inseamna ca modul de lucru este presetat
    if (temp < 100) {
        // Trecem pe mod programat
        modPresetatIrene = 0;
        tempProgramataIrene = temp;
        controlIncalzireIrene(tempValue);
    }
    else
    {
         // Trecem pe mod presetat
        modPresetatIrene = 1;   
    }        
}

// Functie de control incalzire camera Anda
void controlIncalzireAnda(){
       
}

void publicaStareaActuala() {
 
    // Convert to string
    String actuatoareDormitor(stareActuatoareDormitor);
    String actuatoareIrene(stareActuatoareIrene);
    String actuatoareBirou(stareActuatoareBirou);
    // String actuatoareAnda(stareActuatoareAnda);
     
    // Publish only if we have connection to MQTT broker
    int isConnected = checkMQTT();
    if (isConnected) {
        publishMQTT(mqttTopicActuatorDormitor, actuatoareDormitor);
        publishMQTT(mqttTopicTemperaturaDormitor, temperaturaDormitor);
        publishMQTT(mqttTopicActuatorIrene, actuatoareIrene);
        publishMQTT(mqttTopicActuatorBirou, actuatoareBirou);
        // publishMQTT(mqttTopicActuatorAnda, actuatoareAnda);
    }
}