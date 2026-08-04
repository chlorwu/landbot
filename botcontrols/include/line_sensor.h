#pragma once

enum LineSensorIndex {
    OUTER_LEFT = 0,
    INNER_LEFT,
    CENTER_LEFT,
    CENTER_RIGHT,
    INNER_RIGHT,
    OUTER_RIGHT,
    LINE_SENSOR_COUNT   
};

extern bool sensorValues[LINE_SENSOR_COUNT];

void setupLineSensors();
void updateLineSensors();
bool lineIsCentered();
