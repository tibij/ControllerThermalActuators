// Main.h

void setupGPIOs();
int setupWiFi();
void setupOneWireBus();

void setupMQTT();
int checkMQTT();
int getMQTTClientStaus();
int keepMQTTAlive();
int getMQTTClientConnectedState();

void verificaTemperatura();
void controlIncalzireDormitor();

unsigned long startMillis;
unsigned long currentMillis;
const unsigned long period = 12000;  //the value is a number of milliseconds between changes of brightness

unsigned long rebootMillis;
const unsigned long rebootPeriod = 600000;

extern float readTemperature;
unsigned int contorDormitor = 0;
float sumSerieDormitor = 0;