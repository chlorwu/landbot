#include "../include/wifi_setup.h"
#include "../include/firebase_client.h"
#include "../include/line_sensors.h"

void setup() {
    Serial.begin(115200);

    setupWifi();
    setupFirebase();

    setupLineSensors();
}

void loop() {

    updateLineSensors();

    delay(50);
}