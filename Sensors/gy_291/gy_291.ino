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
long long tick_from_start;
long long sync_timestamp;
long long time_gap = 0;


int sensor_id = 1;



int data_counter = 0;
int test_active = 0;
String serwer_url = "http://192.168.0.201:3000/";
String sensor_data[100][2];
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(sensor_id);

void setup()
{
  String response = "";
  setup_wifi_and_timer();

  /* Initialise the sensor */
  if (!accel.begin())
  {
    response = send_status("report_in/", "0", "0", "0", "0", "ADXL345_connection_error");
  }
  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_4_G);
  // DXL345_RANGE_8_G/ADXL345_RANGE_4_G/ADXL345_RANGE_2_G;
  sensors_event_t event;
  await_test_activate();
}
void await_test_activate(){
    while (test_active == 0)
  {
    sensors_event_t event;
    accel.getEvent(&event);
    String response = send_status("report_in/", "1", String(event.acceleration.x), String(event.acceleration.y), String(event.acceleration.z), "Waiting_for_test_to_activate");
    test_active = getValue(response, ',', 1).toInt();
    delay(2000);
  }
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

  response = send_status("report_in/", "1", "0", "0", "0", "Connected");
  update_time_gap(response);

  // Serial.print("\n resp:" + String(response)+"\n");
  // Serial.print("Buf: ");Serial.print(Buf);Serial.print("\n");
  // Serial.print("sync_timestamp: "); Serial.print(sync_timestamp);Serial.print(" tick_from_start:");
  // Serial.print(tick_from_start);Serial.print(" time gap: ");  Serial.print(buf_tg); Serial.print("\n");
}
void update_time_gap(String response) {
  sync_timestamp = getValue(response, ',', 0).toInt();
  tick_from_start = millis();
  time_gap = sync_timestamp - tick_from_start;
  test_active = getValue(response, ',', 1).toInt();
}
String send_status(String ap_url, String stat, String ax, String ay, String az, String msg)
{
  String url_txt = "";
  long active_time = millis() + time_gap;

  int httpCode = -1;
  String response = "";
  url_txt = serwer_url + ap_url +
            "?s_id=" + sensor_id +
            "&s=" + stat +
            "&ts=" + String(active_time) +
            "&ax=" + ax +
            "&ay=" + ay +
            "&az=" + az +
            "&msg=" + msg;
  while (httpCode <= 0)
  {
    http.begin(wifiClient, url_txt);
    httpCode = http.GET();
    response = http.getString();
    http.end();
    blink_connecting();
  }
  return response;
}

void loop()
{
  if (test_active==0) {
    await_test_activate();
    data_counter = 0;
  }
  if (data_counter == 100)
  {
    data_counter = 0;
    send_data();
  }
  else
  {
    sensors_event_t event;
    accel.getEvent(&event);
    sensor_data[data_counter][0] = String(millis() + time_gap);
    sensor_data[data_counter][1] = String(event.acceleration.x);
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
  String response = http.getString();
  update_time_gap(response);
  http.end();
}
String array_to_JSON()
{
  String json = " {\"s_data\":[";
  for (size_t i = 0; i < 100; i++)
  {
    json += "[" + sensor_data[i][0] + "," + sensor_data[i][1] + "]";
    if (i != 99)
    {
      json += ",";
    }
  }
  json += "],\"test_id\":"+String(test_active)+",\"sensor_id\":"+String(sensor_id)+"}";
  return json;
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
String getValue(String data, char separator, int index)
{
  //https://stackoverflow.com/questions/29671455/how-to-split-a-string-using-a-specific-delimiter-in-arduino
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
