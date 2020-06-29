#include <Arduino.h> /* we need arduino functions to implement this */
#include <Blinky.h>

void blinky_init()
{
    pinMode(LED_BUILTIN, OUTPUT);
}

void do_blink(int ms)
{
    digitalWrite(LED_BUILTIN, LOW);
    delay(ms);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(ms);
}