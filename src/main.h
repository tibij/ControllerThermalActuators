// Main.h

void setupGPIOs();
int setupWiFi();
void setupMQTT();
void setupOneWireBus();

void verificaTemperatura();
void controlIncalzireDormitor();
void publicaStareaActuala();

String getTemperature();
