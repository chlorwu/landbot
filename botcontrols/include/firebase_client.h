#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FirebaseClient.h>

void setupFirebase();
void firebaseLoop();
bool waitForFirebaseReady(unsigned long timeoutMs = 15000);
int getTotalPoints();
int parseNumericPoints(const String &jsonPayload);

extern FirebaseApp app;
extern RealtimeDatabase Database;