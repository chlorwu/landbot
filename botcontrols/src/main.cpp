#include "../include/wifi_setup.h"
#include "../include/firebase_client.h"
#include "../include/line_sensor.h"
#include "../include/drive.h"
#include <Arduino.h>

const int DEFAULT_POINTS = 1000;
const int MAX_TOTAL_POINTS = 2000;
const int BASE_SPEED = 75;
const int STEER_SLOW_PERCENT = 60;
const int TURN_SPEED_PERCENT = 85;

const unsigned long FIREBASE_REFRESH_MS = 2000;
const unsigned long TURN_TIMEOUT_MS = 3500;
const unsigned long BRAKE_TIME_MS = 120;
const unsigned long LOOP_DELAY_MS = 20;

// Tune this value for how far forward to drive before turning
const unsigned long TURN_APPROACH_TIME_MS = 230;

int firebasePoints = 0;
int currentSpeed = BASE_SPEED;
unsigned long lastFirebaseRefresh = 0;
int lastLoggedTotalPoints = -9999;


int totalPointsFromFirebase(int points) {
    return constrain(DEFAULT_POINTS + points, 0, MAX_TOTAL_POINTS);
}


int speedFromPoints(int points) {
    int totalPoints = totalPointsFromFirebase(points);

    if (totalPoints <= 0) {
        return 0;
    }

    return constrain((BASE_SPEED * totalPoints) / DEFAULT_POINTS, 0, 255);
}


int turnSpeed() {
    if (currentSpeed <= 0) {
        return 0;
    }

    return constrain((currentSpeed * TURN_SPEED_PERCENT) / 100, 80, 255);
}


void refreshFirebasePoints(bool force = false) {
    firebaseLoop();

    if (!force && millis() - lastFirebaseRefresh < FIREBASE_REFRESH_MS) {
        return;
    }

    lastFirebaseRefresh = millis();

    int fetchedPoints = 0;

    if (!getTotalPoints(fetchedPoints)) {
        Serial.println("Using last known points");
        return;
    }

    firebasePoints = fetchedPoints;
    currentSpeed = speedFromPoints(firebasePoints);

    int totalPoints = totalPointsFromFirebase(firebasePoints);

    if (totalPoints != lastLoggedTotalPoints) {
        Serial.print("Firebase points: ");
        Serial.print(firebasePoints);
        Serial.print(" | speed points: ");
        Serial.print(totalPoints);
        Serial.print(" | PWM speed: ");
        Serial.println(currentSpeed);

        lastLoggedTotalPoints = totalPoints;
    }
}


void brakeBriefly() {
    motorBrake();
    delay(BRAKE_TIME_MS);
}


// Drive forward before turning so sensors clear the intersection
void driveForwardBeforeTurn() {
    Serial.println("Moving forward before turn");

    unsigned long start = millis();

    while (millis() - start < TURN_APPROACH_TIME_MS) {
        firebaseLoop();
        motorForward(currentSpeed);
        delay(LOOP_DELAY_MS);
    }

    motorStop();
    delay(100);
}


void turnUntilCentered(bool turnLeft) {

    brakeBriefly();

    // Move past the intersection first
    driveForwardBeforeTurn();

    unsigned long turnStart = millis();

    while (millis() - turnStart < TURN_TIMEOUT_MS) {

        firebaseLoop();
        updateLineSensors();

        if (lineIsCentered()) {
            motorStop();
            Serial.println("Turn complete");
            return;
        }

        int speed = turnSpeed();

        if (speed <= 0) {
            motorStop();
            return;
        }

        if (turnLeft) {
            motorTurnLeft(speed);
        } 
        else {
            motorTurnRight(speed);
        }

        delay(LOOP_DELAY_MS);
    }

    Serial.println("Turn timeout before centered line was found");
    motorStop();
}


void followLine() {

    refreshFirebasePoints();

    if (currentSpeed <= 0) {
        motorStop();
        return;
    }

    updateLineSensors();


    if (sensorValues[OUTER_LEFT]) {
        Serial.println("Left turn marker detected");
        turnUntilCentered(true);
        return;
    }


    if (sensorValues[OUTER_RIGHT]) {
        Serial.println("Right turn marker detected");
        turnUntilCentered(false);
        return;
    }


    int slowSpeed = (currentSpeed * STEER_SLOW_PERCENT) / 100;


    if (sensorValues[INNER_LEFT] && !sensorValues[INNER_RIGHT]) {
        motorDrive(slowSpeed, currentSpeed);
        return;
    }


    if (sensorValues[INNER_RIGHT] && !sensorValues[INNER_LEFT]) {
        motorDrive(currentSpeed, slowSpeed);
        return;
    }


    if (sensorValues[CENTER_LEFT] || sensorValues[CENTER_RIGHT]) {
        motorForward(currentSpeed);
        return;
    }


    Serial.println("Line lost, stopping");
    motorStop();
}



void setup() {

    Serial.begin(115200);
    delay(1000);

    Serial.println("Starting robot...");


    setupMotor();


    setupWifi();
    setupFirebase();


    setupLineSensors();


    Serial.println("Getting initial points...");

    waitForFirebaseReady();

    refreshFirebasePoints(true);


    Serial.print("Total points: ");
    Serial.println(totalPointsFromFirebase(firebasePoints));
}



void loop() {
    followLine();
    delay(LOOP_DELAY_MS);
}