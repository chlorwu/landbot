#include "../include/firebase_client.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#define DATABASE_URL "https://landbot-92fa9-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDuAeN0FrNvAHx0_rMP9F9JrJjiiFC_dO4"

FirebaseApp app;
RealtimeDatabase Database;
NoAuth no_auth;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient async_client(ssl_client);

bool parseNumericPoints(const String &jsonPayload, int &totalPoints) {
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, jsonPayload);
    if (error) {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return false;
    }

    int total = 0;

    if (!doc.is<JsonObject>()) {
        Serial.println("Expected /players to be a JSON object");
        return false;
    }

    JsonObject players = doc.as<JsonObject>();

    for (JsonPair player : players) {
        if (!player.value().is<JsonObject>()) {
            continue;
        }

        JsonObject playerData = player.value().as<JsonObject>();

        if (playerData["score"].is<int>()) {
            total += playerData["score"].as<int>();
        }
    }

    totalPoints = total;
    return true;
}

int parseNumericPoints(const String &jsonPayload) {
    int total = 0;
    if (!parseNumericPoints(jsonPayload, total)) {
        return -1;
    }

    return total;
}

void setupFirebase() {
    Serial.println("Initializing Firebase...");

    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("Firebase skipped: WiFi not connected");
        return;
    }

    ssl_client.setInsecure();

    initializeApp(async_client, app, getAuth(no_auth));
    app.getApp<RealtimeDatabase>(Database);
    Database.url(DATABASE_URL);

    Serial.println("Firebase initialized (waiting for ready in app.loop)...");
}

void firebaseLoop() {
    app.loop();
}

bool waitForFirebaseReady(unsigned long timeoutMs) {
    unsigned long start = millis();
    while (!app.ready() && millis() - start < timeoutMs) {
        firebaseLoop();
        delay(10);
    }
    if (!app.ready()) {
        Serial.println("Firebase app not ready (timeout)");
        return false;
    }
    return true;
}

int getTotalPoints() {
    int total = 0;
    if (!getTotalPoints(total)) {
        return -1;
    }

    return total;
}

bool getTotalPoints(int &totalPoints) {
    if (!app.ready()) {
        Serial.println("Firebase app not ready");
        return false;
    }

    String json = Database.get<String>(async_client, "/players");

    if (json.length() == 0 || json == "null") {
        Serial.println("No players data found");
        totalPoints = 0;
        return true;
    }

    Serial.println("Players data:");
    Serial.println(json);

    int total = 0;
    if (!parseNumericPoints(json, total)) {
        return false;
    }

    Serial.print("Total points: ");
    Serial.println(total);

    totalPoints = total;
    return true;
}
