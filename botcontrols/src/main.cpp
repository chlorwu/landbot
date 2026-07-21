#include "../include/wifi_setup.h"
#include "../include/firebase_client.h"
#include "../include/line_sensor.h"
#include "../include/drive.h"
#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting robot...");

    // Initialize motor driver
    setupMotor();

    // Initialize WiFi + Firebase
    setupWifi();
    setupFirebase();

    // Initialize line sensors
    setupLineSensors();

    Serial.println("Getting initial points...");
    waitForFirebaseReady();
    int totalPoints = getTotalPoints();

    Serial.print("Total points: ");
    Serial.println(totalPoints);
}


void loop() {
    firebaseLoop();

    // Read line sensors
    updateLineSensors();

    // Motor test
    Serial.println("Motor forward");
    motorForward();
    delay(3000);

    Serial.println("Motor stop");
    motorStop();
    delay(1000);

    Serial.println("Motor backward");
    motorBackward();
    delay(3000);

    Serial.println("Motor stop");
    motorStop();
    delay(1000);

    firebaseLoop();

    // Firebase check
    int totalPoints = getTotalPoints();

    Serial.print("Current total points: ");
    Serial.println(totalPoints);
}