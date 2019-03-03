// MQTT broker connection paramters
const char* mqttServer = "172.27.0.20"; // local broker running on Oxygen Gateway
const char* mqttUserName = "";
const char* mqttPassword = "";

int setupWiFi();
void subscribeMQTT(const char* mqttTopic);
WiFiClient mqttWiFiClient;
PubSubClient mqttClient(mqttWiFiClient);

void programareIncalzireDormitor(char* tempValue);
void controlIncalzireBirou(char* tempValue);
void programareIncalzireBirou(char* tempValue);
void controlIncalzireIrene(char* tempValue);
void programareIncalzireIrene(char* tempValue);

// Topics defined on the MQTT broker
//
// Dormitorul mare
// Temperatura curenta indicata de senzor
const char* mqttTopicTemperaturaDormitor = "house/etaj/dormitor/temp/value";
// Stare actuatoare termice care controleaza radiatoarele din dormitor
const char* mqttTopicActuatorDormitor = "house/etaj/dormitor/actuator/state";
// Temperatura programata de la distanta
const char* mqttTopicTemperaturaDormitorCeruta = "house/etaj/dormitor/tempceruta/value";

// Birou - Hol
// Temperatura curenta indicata de senzor
const char* mqttTopicTemperaturaBirou = "house/etaj/birou/temp/value";
// Stare actuatoare termice care controleaza radiatoarele din birou si hol
const char* mqttTopicActuatorBirou = "house/etaj/birou/actuator/state";
// Temperatura programata de la distanta
const char* mqttTopicTemperaturaBirouCeruta = "house/etaj/birou/tempceruta/value";

// Irene
// Temperatura curenta indicata de senzor
const char* mqttTopicTemperaturaIrene = "house/etaj/irene/temp/value";
// Stare actuatoare termice care controleaza radiatoarele de la Irene
const char* mqttTopicActuatorIrene = "house/etaj/irene/actuator/state";
// Temperatura programata de la distanta
const char* mqttTopicTemperaturaIreneCeruta = "house/etaj/irene/tempceruta/value";


// Anda
// Temperatura curenta indicata de senzor
// const char* mqttTopicTemperaturaAnda = "house/etaj/anda/temp/value";
// Stare actuator termic care controleaza radiatoarul de la Anda 
// const char* mqttTopicActuatorAnda = "house/etaj/anda/actuator/state";
// Temperatura programata de la distanta
// const char* mqttTopicTemperaturaAndaCeruta = "house/etaj/anda/tempceruta/value";
