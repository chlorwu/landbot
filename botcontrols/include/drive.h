#ifndef DRIVE_H
#define DRIVE_H

void setupMotor();
void motorForward(int speed);
void motorBackward(int speed);
void motorDrive(int leftSpeed, int rightSpeed);
void motorTurnLeft(int speed);
void motorTurnRight(int speed);
void motorBrake();
void motorStop();

#endif
