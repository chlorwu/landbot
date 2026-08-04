#include <Arduino.h>
#include "../include/drive.h"

const int MOTOR_EN_A = 14;
const int MOTOR_IN_1 = 27;
const int MOTOR_IN_2 = 26;

const int MOTOR_EN_B = 32;
const int MOTOR_IN_3 = 25;
const int MOTOR_IN_4 = 33;

const int PWM_FREQUENCY = 1000;
const int PWM_CHANNEL_A = 0;
const int PWM_CHANNEL_B = 1;
const int PWM_RESOLUTION = 8;

int clampSpeed(int speed) {
    return constrain(speed, 0, 255);
}

void writeMotorSpeed(int leftSpeed, int rightSpeed) {
#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcWrite(MOTOR_EN_A, clampSpeed(leftSpeed));
    ledcWrite(MOTOR_EN_B, clampSpeed(rightSpeed));
#else
    ledcWrite(PWM_CHANNEL_A, clampSpeed(leftSpeed));
    ledcWrite(PWM_CHANNEL_B, clampSpeed(rightSpeed));
#endif
}

void setupMotor() {
    pinMode(MOTOR_IN_1, OUTPUT);
    pinMode(MOTOR_IN_2, OUTPUT);
    pinMode(MOTOR_IN_3, OUTPUT);
    pinMode(MOTOR_IN_4, OUTPUT);

#if defined(ESP_ARDUINO_VERSION_MAJOR) && ESP_ARDUINO_VERSION_MAJOR >= 3
    ledcAttachChannel(MOTOR_EN_A, PWM_FREQUENCY, PWM_RESOLUTION, PWM_CHANNEL_A);
    ledcAttachChannel(MOTOR_EN_B, PWM_FREQUENCY, PWM_RESOLUTION, PWM_CHANNEL_B);
#else
    ledcSetup(PWM_CHANNEL_A, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcSetup(PWM_CHANNEL_B, PWM_FREQUENCY, PWM_RESOLUTION);
    ledcAttachPin(MOTOR_EN_A, PWM_CHANNEL_A);
    ledcAttachPin(MOTOR_EN_B, PWM_CHANNEL_B);
#endif

    motorStop();
}

void motorForward(int speed) {
    motorDrive(speed, speed);
}

void motorBackward(int speed) {
    writeMotorSpeed(speed, speed);
    digitalWrite(MOTOR_IN_1, LOW);
    digitalWrite(MOTOR_IN_2, HIGH);
    digitalWrite(MOTOR_IN_3, LOW);
    digitalWrite(MOTOR_IN_4, HIGH);
}

void motorDrive(int leftSpeed, int rightSpeed) {
    writeMotorSpeed(leftSpeed, rightSpeed);
    digitalWrite(MOTOR_IN_1, HIGH);
    digitalWrite(MOTOR_IN_2, LOW);
    digitalWrite(MOTOR_IN_3, HIGH);
    digitalWrite(MOTOR_IN_4, LOW);
}

void motorTurnLeft(int speed) {
    writeMotorSpeed(speed, speed);
    digitalWrite(MOTOR_IN_1, LOW);
    digitalWrite(MOTOR_IN_2, HIGH);
    digitalWrite(MOTOR_IN_3, HIGH);
    digitalWrite(MOTOR_IN_4, LOW);
}

void motorTurnRight(int speed) {
    writeMotorSpeed(speed, speed);
    digitalWrite(MOTOR_IN_1, HIGH);
    digitalWrite(MOTOR_IN_2, LOW);
    digitalWrite(MOTOR_IN_3, LOW);
    digitalWrite(MOTOR_IN_4, HIGH);
}

void motorBrake() {
    writeMotorSpeed(255, 255);
    digitalWrite(MOTOR_IN_1, HIGH);
    digitalWrite(MOTOR_IN_2, HIGH);
    digitalWrite(MOTOR_IN_3, HIGH);
    digitalWrite(MOTOR_IN_4, HIGH);
}

void motorStop() {
    writeMotorSpeed(0, 0);
    digitalWrite(MOTOR_IN_1, LOW);
    digitalWrite(MOTOR_IN_2, LOW);
    digitalWrite(MOTOR_IN_3, LOW);
    digitalWrite(MOTOR_IN_4, LOW);
}
