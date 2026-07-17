#include "../include/wifi_setup.h"
#include "../include/firebase_client.h"
#include "../include/line_sensor.h"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);

    setupWifi();
    setupFirebase();

    setupLineSensors();

    Serial.println("Getting initial points...");
    int totalPoints = getTotalPoints();

    Serial.print("Total points: ");
    Serial.println(totalPoints);
}

void loop() {
    updateLineSensors();

    delay(5000);

    int totalPoints = getTotalPoints();

    Serial.print("Current total points: ");
    Serial.println(totalPoints);
}