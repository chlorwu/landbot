#include "../include/wifi_setup.h"
#include <WiFi.h>
#include <Arduino.h>

// this is chloe's personal hotspot
#define WIFI_SSID "YouCantBelieveItsAHotspot"
#define WIFI_PASSWORD "landbotaccess"

void setupWifi()
{
    Serial.print("Connecting to WiFi");

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    unsigned long start = millis();

    while (WiFi.status() != WL_CONNECTED && millis() - start < 15000)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected!");
        Serial.println(WiFi.localIP());
    } else {
        Serial.print("Failed. Status = ");
        Serial.println(WiFi.status());
        Serial.println("Is Chloe's hotspot on??");
    }
}