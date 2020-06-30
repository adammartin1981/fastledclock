#include <Arduino.h>
#include <Chaser.h>
#include <Helper.h>
#include <Time.h>

void setup()
{
  Serial.begin(9600);
  
  delay(1000); // power-up safety delay

  chaser_init();

  setTime(10, 45, 50, 4, 4, 4);

  chaser_updateMinute(45);
}

void loop()
{
  chaser_loop();

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
}