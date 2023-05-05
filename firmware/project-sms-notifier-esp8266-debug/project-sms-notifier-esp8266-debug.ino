#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <ESP8266WebServer.h>
#include "twilio.hpp"

#define KEEP_ALIVE_PIN D5

static const char *ssid = "<SSID (WiFi Network)>";
static const char *password = "<WiFi Password>";

// Values from Twilio (find them on the dashboard)
static const char *account_sid = "<account_sid>";
static const char *auth_token = "<auth_token>";
static const char fingerprint[] = "<https fingerprint>";
static const char *from_number = "<twilio number, w/country code>"; // ex: +17045555478

static const char *to_number = "<number to send sms to>"; // ex: +15555552955
static const char *message = "<message>";

Twilio *twilio;

void setup() {

  // Handle the keep alive pin so the power supply stays on.
  pinMode(KEEP_ALIVE_PIN, OUTPUT);
  digitalWrite(KEEP_ALIVE_PIN, HIGH);

  // Initialize & turn the built in LED on
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  Serial.print("Connecting to WiFi network ;");
  Serial.print(ssid);
  Serial.println("'...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connecting...");
    delay(500);
  }
  Serial.println("Connected!");

  twilio = new Twilio(account_sid, auth_token, fingerprint);

  // If the board has trouble sending text messages, try uncommenting this to give more time to establish a WiFi connection
  // delay(1000);

  String response;
  bool success = twilio->send_message(to_number, from_number, message, response);
  if (success) {
    Serial.println("Sent message successfully!");
  } else {
    Serial.println(response);
  }

  // Turn the power supply off.
  digitalWrite(KEEP_ALIVE_PIN, LOW);
}

void loop() {}
