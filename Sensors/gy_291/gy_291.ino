
#define ONBOARD_LED  2
#include "credentials.h"
#include <WiFi.h>

void setup() {
  pinMode(ONBOARD_LED,OUTPUT);
  //WiFi.mode(WIFI_STA); //Optional
  WiFi.begin(ssid, password);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    delay(100);
    digitalWrite(ONBOARD_LED,HIGH);
    delay(100);
    digitalWrite(ONBOARD_LED,LOW);
  } else {
    delay(100);
    digitalWrite(ONBOARD_LED,HIGH);
    delay(100);
    digitalWrite(ONBOARD_LED,LOW);
  }
}
