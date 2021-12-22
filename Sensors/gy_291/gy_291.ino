#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>

#include <Wire.h>

#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#define ONBOARD_LED 2
#include "credentials.h"
#include <ESP8266WiFi.h>

WiFiClient wifiClient;
HTTPClient http;
unsigned long tick_from_start;
unsigned long sync_timestamp;
unsigned long time_gap = 0;
int sensor_id = 1;
int data_counter = 0;
String serwer_url = "http://192.168.0.201:3000/";
String sensor_data[1000][2];
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(sensor_id);

void setup()
{
  setup_wifi_and_timer();
  /* Initialise the sensor */
  if (!accel.begin())
  {
    String response = send_status("report_in", "0", "0", "0", "0", "ADXL345-connection error");
  }
  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_4_G);
  // DXL345_RANGE_8_G/ADXL345_RANGE_4_G/ADXL345_RANGE_2_G;
}

void setup_wifi_and_timer()
{
  String response = "";
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, HIGH);
  Serial.begin(115200);
  WiFi.begin(ssid, password); // Connect to the network
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(2000);
    blink_connecting();
  }
  blink_established();

  response = send_status("report_in/", "1", "0", "0", "0", "connected");
  sync_timestamp = response.toInt();
  tick_from_start = millis();
  time_gap = sync_timestamp - tick_from_start;
}

String send_status(String ap_url, String status, String ax, String ay, String az, String msg)
{
  String url_txt = "";
  String active_time = String(millis() + time_gap);
  int httpCode = -1;
  url_txt = serwer_url + ap_url +
            "?s_id=" + sensor_id +
            "&s=" + status +
            "&ts=" + active_time +
            "&ax=" + ax +
            "&ay=" + ay +
            "&az=" + az +
            "&msg=" + msg;
  while (httpCode <= 0)
  {
    http.begin(wifiClient, url_txt);
    httpCode = http.GET();
    String response = http.getString();
    http.end();
    blink_connecting();
  }
  return response;
}

void loop()
{
  if (data_counter == 1000)
  {
    data_counter = 0;
    send_data()
  }
  else
  {
    sensors_event_t event;
    accel.getEvent(&event);
    sensor_data[i][0] = String active_time = String(millis() + time_gap);
    sensor_data[i][0] = String active_time = String(event.acceleration.x);
    data_counter++;
  }
  /* Get a new sensor event */
  delay(10);
}
void send_data()
{
  http.begin(wifiClient, serwer_url + "feed/");
  http.addHeader("Content-Type", "application/json");
  int httpResponseCode = http.POST(array_to_JSON());
  http.end();
}
String array_to_JSON()
{
  String json = ' {';
  for (size_t i = 0; i < 1000; i++)
  {
    json += "[" + sensor_data[i][0] + "," + sensor_data[i][1] + "]";
    if (i != 999)
    {
      json += ",";
    }
  }
  return json
}
void blink_established()
{
  digitalWrite(ONBOARD_LED, LOW);
  delay(700);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(300);
  digitalWrite(ONBOARD_LED, LOW);
  delay(700);
  digitalWrite(ONBOARD_LED, HIGH);
  delay(1000);
}
void blink_connecting()
{
  for (int i = 0; i <= 4; i++)
  {
    digitalWrite(ONBOARD_LED, LOW);
    delay(50);
    digitalWrite(ONBOARD_LED, HIGH);
    delay(50);
  }
}
