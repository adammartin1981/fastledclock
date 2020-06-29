#include <Arduino.h>
#include <Chaser.h>
#include <Time.h>

void setup()
{
  Serial.begin(9600);
  
  delay(1000); // power-up safety delay

  chaser_init();
}



void loop()
{
  chaser_loop();

  static int lastSecond = second();  

  int nowSecond = second();

  if(lastSecond != nowSecond) {
    lastSecond = nowSecond;
    chaser_updateTarget(nowSecond);
  }
}