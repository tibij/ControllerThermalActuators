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
    getTemperature();
    Serial.println(readTemperature);
}

// Control centrale. Se opresc sau pornesc in functie de starile termostatelor si temperatura tur puffer
void controlIncalzireDormitor(){ 
    
    float tempDorita;   
    char convertedTemperature[8];
    int actuatorDeschis = 1;
    char actuatorDormitor[2];
    
    actuatorDormitor[0] = '1';
    actuatorDormitor[1] = '\0';
 

    // Verificam modul de lucru
    if (modPresetatDormitor)
        tempDorita = PRESET_TEMPERATURE_DORMITOR;
    else
        tempDorita = tempProgramataDormitor;
    
    // Verificam daca sensorul de temperatura este in parametrii
    if (readTemperature > tempDorita){
            // Temperatura a depasit valoarea presetata --> inchidem actuatoarele si oprim incalzirea
            actuatorDeschis = 0;
            actuatorDormitor[0] = '0';
            actuatorDormitor[1] = '\0';
    }

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
    
    // Convertim in string temperatura
    snprintf(convertedTemperature, strlen(convertedTemperature), "%.2f", readTemperature);

    // Publicam starea actuatoarelor din dormitor si temperatura medie
    publishMQTT(mqttTopicActuatorDormitor, actuatorDormitor);
    publishMQTT(mqttTopicTemperaturaDormitor, convertedTemperature);
}
// Functie de control incalzire camera Irene
void controlIncalzireIrene(){    
    int actuatorDeschis = 1;
    float tempDorita;
    char actuatorIrene[2];
    
    actuatorIrene[0] = '1';
    actuatorIrene[1] = '\0';    

    // Verificam modul de lucru
    if (modPresetatIrene)
        tempDorita = PRESET_TEMPERATURE_IRENE;
    else
        tempDorita = tempProgramataIrene;    

    if (tempInIrene > tempDorita) {
        // Temperatura a depasit valoarea presetata --> inchidem actuatoarele si oprim incalzirea
        actuatorDeschis = 0;
        actuatorIrene[0] = '0';
        actuatorIrene[1] = '\0';  
    }
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
    publishMQTT(mqttTopicActuatorIrene, actuatorIrene);
}

// Functie de control incalzire birou si hol
void controlIncalzireBirou(){
    int actuatorDeschis = 1;
    float tempDorita;
    char actuatorBirou[2];
    
    actuatorBirou[0] = '1';
    actuatorBirou[1] = '\0';    

    // Verificam modul de lucru
    if (modPresetatBirou)
        tempDorita = PRESET_TEMPERATURE_BIROU;
    else
        tempDorita = tempProgramataBirou;

    if (tempInBirou > tempDorita) {
            // Temperatura a depasit valoarea setata --> inchidem actuatoarele si oprim incalzirea
            actuatorDeschis = 0;
            actuatorBirou[0] = '0';
            actuatorBirou[1] = '\0';
    }
    if (actuatorDeschis) {
        // Actuator pe pozitie deschisa --> se deschide circuitul de incalzire       
        stareActuatoareBirou = 1;
        digitalWrite(releuBirou, LOW);
    }
    else {
        stareActuatoareBirou = 0;
        digitalWrite(releuBirou, HIGH);
    }
    publishMQTT(mqttTopicActuatorBirou, actuatorBirou);
}

// Setare mod de incalzire dormitor: programata sau presetata
void programareIncalzireDormitor(){
    // Daca temperatura este mai mare sau egala cu 100 inseamna ca modul de lucru este presetat
    if (tempProgramataDormitor < 100) {
        // Trecem pe mod programat
        modPresetatDormitor = 0;
        tempProgramataDormitor = tempProgramataDormitor;
        controlIncalzireDormitor();
    }
    else
    {
         // Trecem pe mod presetat
        modPresetatDormitor = 1;   
    }
}

// Setare mod de incalzire birou: programata sau presetata
void programareIncalzireBirou(){
    // Daca temperatura este mai mare sau egala cu 100 inseamna ca modul de lucru este presetat
    if (tempProgramataBirou < 100) {
        // Trecem pe mod programat
        modPresetatBirou = 0;
        controlIncalzireBirou();
    }
    else
    {
         // Trecem pe mod presetat
        modPresetatBirou = 1;   
    }    
}

// Setare mod de incalzire Irene: programata sau presetata
void programareIncalzireIrene(){
    // Daca temperatura este mai mare sau egala cu 100 inseamna ca modul de lucru este presetat
    if (tempProgramataIrene < 100) {
        // Trecem pe mod programat
        modPresetatIrene = 0;
        controlIncalzireIrene();
    }
    else
    {
         // Trecem pe mod presetat
        modPresetatIrene = 1;   
    }        
}