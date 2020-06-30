#include <FastLED.h>

#pragma once

void chaser_init();
void chaser_loop();
void chaser_updateSecond(int newTarget);
void chaser_updateMinute(int newTarget);
void chaser_updateHour(int newTarget);