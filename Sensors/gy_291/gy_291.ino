

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
unsigned long time_gap;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(00001);

void setup()
{
  Serial.begin(115200);
  setup_wifi_and_timer();
  Serial.print("\n");
  Serial.print("\n");
  Serial.print("Wire.begin(1, 2);\n");
  //Wire.begin(4, 5); //SDA, SCL

  Serial.print("Accelerometer Test\n");
  Serial.print("\n");

  /* Initialise the sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1)
      ;
  }
  ///
  /* Set the range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  //displaySetRange(ADXL345_RANGE_2_G);

  /* Display some basic information on this sensor */
  displaySensorDetails();

  /* Display additional settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.print("\n");
}

void loop()
{
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: ");
  Serial.print(event.acceleration.x);
  Serial.print(" ");
  String active_time = String(millis() + time_gap);

  http.begin(wifiClient, "http://192.168.0.108:3000/feed/?sensor_id=1&sensor_reading=" + String(event.acceleration.x) + "&sensor_tick=" + active_time);
  int httpCode = http.GET();
  http.end();
  // Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print(" ");
  //Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print(" ");Serial.println("m/s^2 ");
  delay(100);
}

void setup_wifi_and_timer()
{
  String response = "";
  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, HIGH);
  Serial.begin(115200);       // Start the Serial communication to send messages to the computer
  WiFi.begin(ssid, password); // Connect to the network
  while (WiFi.status() != WL_CONNECTED)
  { // Wait for the Wi-Fi to connect
    delay(2000);
    blink_connecting();
  }
  blink_established();

  Serial.print("\nhhtp begin\n");
  http.begin(wifiClient, "http://192.168.0.108:3000/report_in/?s_id=1&s=1&ts=1&ax=1&ay=2&az=3");
  int httpCode = http.GET();
  Serial.print(String(httpCode) + "\n");
  response = http.getString();
  Serial.print(response + "\n");
  Serial.print("eof\n");
  http.end();
  sync_timestamp = response.toInt();
  tick_from_start = millis();
  time_gap = sync_timestamp - tick_from_start;
  Serial.print("sync_timestamp: " + String(sync_timestamp) + " tick_from_start: " + String(tick_from_start) + " time gap " + String(time_gap) + "\n");
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
  delay(300);
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

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor: ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver: ");
  Serial.println(sensor.version);
  Serial.print("Unique ID: ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value: ");
  Serial.print(sensor.max_value);
  Serial.println(" m/s^2");
  Serial.print("Min Value: ");
  Serial.print(sensor.min_value);
  Serial.println(" m/s^2");
  Serial.print("Resolution: ");
  Serial.print(sensor.resolution);
  Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
void displayDataRate(void)
{
  Serial.print("Data Rate: ");

  switch (accel.getDataRate())
  {
  case ADXL345_DATARATE_3200_HZ:
    Serial.print("3200 ");
    break;
  case ADXL345_DATARATE_1600_HZ:
    Serial.print("1600 ");
    break;
  case ADXL345_DATARATE_800_HZ:
    Serial.print("800 ");
    break;
  case ADXL345_DATARATE_400_HZ:
    Serial.print("400 ");
    break;
  case ADXL345_DATARATE_200_HZ:
    Serial.print("200 ");
    break;
  case ADXL345_DATARATE_100_HZ:
    Serial.print("100 ");
    break;
  case ADXL345_DATARATE_50_HZ:
    Serial.print("50 ");
    break;
  case ADXL345_DATARATE_25_HZ:
    Serial.print("25 ");
    break;
  case ADXL345_DATARATE_12_5_HZ:
    Serial.print("12.5 ");
    break;
  case ADXL345_DATARATE_6_25HZ:
    Serial.print("6.25 ");
    break;
  case ADXL345_DATARATE_3_13_HZ:
    Serial.print("3.13 ");
    break;
  case ADXL345_DATARATE_1_56_HZ:
    Serial.print("1.56 ");
    break;
  case ADXL345_DATARATE_0_78_HZ:
    Serial.print("0.78 ");
    break;
  case ADXL345_DATARATE_0_39_HZ:
    Serial.print("0.39 ");
    break;
  case ADXL345_DATARATE_0_20_HZ:
    Serial.print("0.20 ");
    break;
  case ADXL345_DATARATE_0_10_HZ:
    Serial.print("0.10 ");
    break;
  default:
    Serial.print("???? ");
    break;
  }
  Serial.println(" Hz");
}
void displayRange(void)
{
  Serial.print("Range: +/- ");

  switch (accel.getRange())
  {
  case ADXL345_RANGE_16_G:
    Serial.print("16 ");
    break;
  case ADXL345_RANGE_8_G:
    Serial.print("8 ");
    break;
  case ADXL345_RANGE_4_G:
    Serial.print("4 ");
    break;
  case ADXL345_RANGE_2_G:
    Serial.print("2 ");
    break;
  default:
    Serial.print("?? ");
    break;
  }
  Serial.println(" g");
}
