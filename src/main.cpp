#define FASTLED_INTERNAL

#include <Arduino.h>
#include <FastLED.h>
#include <Blinky.h>
#include <Clock.h>
#include <Time.h>
#include <Chaser.h>

static int nowTime;

void setup()
{
  Serial.begin(9600);

  // clock_init();
  
  delay(1000); // power-up safety delay

  setTime(10, 10, 55, 4,4,4);

  chaser_init();
}



void loop()
{
  // clock_loop();
  // clock_loop2();

  chaser_loop();

  static int lastSecond = second();  

  int nowSecond = second();

  if(lastSecond != nowSecond) {
    lastSecond = nowSecond;
    Serial.println();
    Serial.print("Second: ");
    Serial.println(nowSecond);
    chaser_updateTarget(nowSecond);
  }
}