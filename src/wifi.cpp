#include <Arduino.h>
#include <WiFi.h>

// WiFi Credentials
const char *ssid = "cgr";
const char *password = "monchiX2";

void scanWifiNet()
{
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    delay(1000);

    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0)
    {
        Serial.println("no networks found");
    }
    else
    {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
            // Print SSID and RSSI for each network found
            Serial.print(i + 1);
            Serial.print(": ");
            Serial.print(WiFi.SSID(i));
            Serial.print(" (");
            Serial.print(WiFi.RSSI(i));
            Serial.print(")");
            Serial.println(WiFi.encryptionType(i));
            delay(10);
        }
    }
}

int setupWiFi()
{
    // Scan the WiFi network
    scanWifiNet();

    delay(1000);
    Serial.println("Diag info:");
    WiFi.printDiag(Serial);

    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    int wifiStatus = WiFi.begin(ssid, password);
    delay(5000);
    Serial.print(wifiStatus);
    // Loop until we connected but stop after 3 attempts
    int retryCount = 0;
    while (wifiStatus != WL_CONNECTED && retryCount < 30)
    {
        wifiStatus = WiFi.status();
        Serial.print(wifiStatus);
        delay(1000);
        //Serial.print(".");
        retryCount++;
    }

    switch (wifiStatus)
    {
    case WL_CONNECTED:
        Serial.println("");
        Serial.print("WiFi connected. ");
        Serial.print("IP address: ");
        Serial.println(WiFi.localIP());
        break;
    default:
        Serial.println("");
        Serial.println("WiFi connection failed.");
        ESP.restart();
        break;
    }

    return wifiStatus;
}