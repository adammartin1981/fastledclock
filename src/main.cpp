#include <Arduino.h>
#include <Chaser.h>
#include <Helper.h>
#include <Time.h>
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Replace with your network credentials
const char *ssid = "xxx";
const char *password = "xxx";

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

void setup()
{
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  
  timeClient.setTimeOffset(3600);

  timeClient.update();

  delay(1000); // power-up safety delay


  int currentHour = timeClient.getHours();
  
  int currentMinute = timeClient.getMinutes();
  
  int currentSecond = timeClient.getSeconds();

  setTime(currentHour, currentMinute, currentSecond, 4, 4, 4);

  chaser_init();

  // chaser_updateMinute(45);
}

void loop()
{

  timeClient.update();

  static int lastSecond = -1;
  static int lastMinute = -1;
  static int lastHour = -1;

  int nowSecond = second();
  int nowMinute = minute();
  int nowHour = hour();

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