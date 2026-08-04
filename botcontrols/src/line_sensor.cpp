#include <Arduino.h>
#include "../include/line_sensor.h"

const int SENSOR_PINS[LINE_SENSOR_COUNT] = {5, 18, 19, 21, 22, 23};
const int LINE_DETECTED_STATE = HIGH;

bool sensorValues[LINE_SENSOR_COUNT];

void setupLineSensors() {
    for (int index = 0; index < LINE_SENSOR_COUNT; index++) {
        pinMode(SENSOR_PINS[index], INPUT);
    }

    Serial.println("Line sensors initialized");
}

void updateLineSensors() {
    for (int index = 0; index < LINE_SENSOR_COUNT; index++) {
        sensorValues[index] = digitalRead(SENSOR_PINS[index]) == LINE_DETECTED_STATE;
    }

    Serial.print(sensorValues[OUTER_LEFT] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[INNER_LEFT] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[CENTER_LEFT] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[CENTER_RIGHT] ? "BLACK " : "WHITE ");
    Serial.print(sensorValues[INNER_RIGHT] ? "BLACK " : "WHITE ");
    Serial.println(sensorValues[OUTER_RIGHT] ? "BLACK" : "WHITE");
}

bool lineIsCentered() {
    return !sensorValues[OUTER_LEFT]
        && !sensorValues[INNER_LEFT]
        && sensorValues[CENTER_LEFT]
        && sensorValues[CENTER_RIGHT]
        && !sensorValues[INNER_RIGHT]
        && !sensorValues[OUTER_RIGHT];
}
