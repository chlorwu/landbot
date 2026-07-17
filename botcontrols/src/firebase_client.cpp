#include "firebase_client.h"
#include <Arduino.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>

#define DATABASE_URL "https://landbot-92fa9-default-rtdb.firebaseio.com/"
#define API_KEY "AIzaSyDuAeN0FrNvAHx0_rMP9F9JrJjiiFC_dO4"


FirebaseApp app;
RealtimeDatabase Database;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient async_client(ssl_client);

int parseNumericPoints(const String &jsonPayload)
{
    int total = 0;

    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error)
    {
        Serial.print("JSON parsing failed: ");
        Serial.println(error.c_str());
        return -1;
    }

    JsonObject players = doc.as<JsonObject>();

    for (JsonPair player : players)
    {
        total += player.value().as<int>();
    }

    return total;
}

void setupFirebase()
{
    Serial.println("Initializing Firebase...");

    ssl_client.setInsecure();
    
    app.getApp<RealtimeDatabase>(Database);
    Database.url(DATABASE_URL);

    Serial.println("Firebase initialized.");
}

int getTotalPoints()
{
    if (!app.ready())
    {
        Serial.println("Firebase app not ready");
        return -1;
    }

    String json = Database.get<String>(async_client, "/players");

    if (json.length() == 0 || json == "null")
    {
        Serial.println("No players data found");
        return 0;
    }

    Serial.println("Players data:");
    Serial.println(json);

    int total = parseNumericPoints(json);

    Serial.print("Total points: ");
    Serial.println(total);

    return total;
}