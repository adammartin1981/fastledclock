#include <Arduino.h>
#include <Chaser.h>
#include <Helper.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "config.h"

// Replace with your network credentials
// Create config.h file and define these next to the original file
const char *ssid = WIFI_SSID;
const char *password = WIFI_PASSWORD;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);

  delay(1000); // power-up safety delay

  Serial.print("Connecting WiFi:");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);  
    Serial.print(".");
  }
  Serial.println();

  // Initialize a NTPClient to get time
  timeClient.begin();
  
  timeClient.setTimeOffset(3600);
  timeClient.update();

  chaser_init();
}

void loop()
{
  EVERY_N_MINUTES(60) {
    timeClient.update();
  }

  static int lastSecond = -1;
  static int lastMinute = -1;
  static int lastHour = -1;

  int nowSecond = timeClient.getSeconds();
  int nowMinute = timeClient.getMinutes();
  int nowHour = timeClient.getHours();

  if(lastSecond != nowSecond)
  {
    lastSecond = nowSecond;
    chaser_updateSecond(nowSecond);
  }

  if (lastMinute != nowMinute)
  {
    lastMinute = nowMinute;
    chaser_updateMinute(nowMinute);
  }

  if (lastHour != nowHour)
  {
    lastHour = nowHour;
    chaser_updateHour(nowHour);
  }

  chaser_loop();
}