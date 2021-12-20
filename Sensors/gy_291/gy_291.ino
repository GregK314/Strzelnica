
#include<Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define ONBOARD_LED  2
#include "credentials.h"
#include <ESP8266WiFi.h>


WiFiClient wifiClient;
HTTPClient http;
unsigned long tick_from_start;
unsigned long sync_timestamp;
unsigned long time_gap;

void setup() {
  setup_wifi_and_timer()
  Wire.begin(1, 2); //SDA, SCL
}

void loop() {
  
}

void setup_wifi_and_timer() {
  String response = "";
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, HIGH);
  Serial.begin(115200);         // Start the Serial communication to send messages to the computer
  WiFi.begin(ssid, password);             // Connect to the network
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(2000);
    blink_connecting();
  }
  blink_established();
  digitalWrite(ONBOARD_LED, HIGH);
  Serial.print("\nhhtp begin\n");
  
  http.begin(wifiClient,"http://192.168.0.108:3000/report_in/?s_id=1&s=1&ts=1&ax=1&ay=2&az=3");
  int httpCode = http.GET();
  Serial.print(String(httpCode)+"\n");
  response = http.getString();
  Serial.print(response+"\n");
  Serial.print("eof\n");
  http.end(); 
  sync_timestamp = response.toInt();
  tick_from_start = time(NULL);
  time_gap = sync_timestamp-tick_from_start;
  Serial.print("sync_timestamp: "+ String(sync_timestamp)+ " tick_from_start: " +String(tick_from_start)+ " time gap " + String(time_gap) +"\n");
  
}

void blink_established() {
  digitalWrite(ONBOARD_LED, LOW);
  delay(700);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(300);
  digitalWrite(ONBOARD_LED, LOW);
  delay(700);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(300);
}
void blink_connecting() {
    for (int i = 0; i <= 4; i++) {
      digitalWrite(ONBOARD_LED, LOW);
      delay(50);
      digitalWrite(ONBOARD_LED, HIGH);
      delay(50);
    }
}
