#include <wifi_setup.h>
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

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(300);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}