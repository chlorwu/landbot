#pragma once

#include <WiFi.h>
#include <Arduino.h>
#include <ArduinoJson.h>
#include <FirebaseClient.h>

void setupFirebase();
int getTotalPoints();
int parseNumericPoints(const String &jsonPayload);

extern FirebaseApp app;
extern RealtimeDatabase Database;