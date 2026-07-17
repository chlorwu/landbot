#include <Arduino.h>
#include "../include/line_sensor.h"

const int LEFT2_PIN  = 13;
const int LEFT1_PIN  = 12;
const int CENTER_PIN = 14;
const int RIGHT1_PIN = 27;
const int RIGHT2_PIN = 26;

// Exported so other files can access them
bool sensorValues[5];

void setupLineSensors() {
    pinMode(LEFT2_PIN, INPUT);
    pinMode(LEFT1_PIN, INPUT);
    pinMode(CENTER_PIN, INPUT);
    pinMode(RIGHT1_PIN, INPUT);
    pinMode(RIGHT2_PIN, INPUT);

    Serial.println("Line sensors initialized");
}

void updateLineSensors() {

    sensorValues[0] = digitalRead(LEFT2_PIN)  == HIGH;
    sensorValues[1] = digitalRead(LEFT1_PIN)  == HIGH;
    sensorValues[2] = digitalRead(CENTER_PIN) == HIGH;
    sensorValues[3] = digitalRead(RIGHT1_PIN) == HIGH;
    sensorValues[4] = digitalRead(RIGHT2_PIN) == HIGH;

    Serial.print(sensorValues[0] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[1] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[2] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[3] ? "BLACK " : "WHITE ");
    Serial.println(sensorValues[4] ? "BLACK" : "WHITE");
}