#include <FastLED.h>

#pragma once

/* clock initialization function */
void clock_init();

void clock_loop();
void clock_loop2();

/* blinks the LED once for an length of <ms> milliseconds. */
// void do_blink(int ms);

struct Fader
{
    unsigned long timeAdded;
    unsigned long timeToFadeOver;
    CHSV ledHSV;
};