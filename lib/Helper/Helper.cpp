#include <Arduino.h>
#include <FastLED.h>
#include <Helper.h>

// Helper function that blends one uint8_t toward another by a given amount
void nblendU8TowardU8(uint8_t &cur, const uint8_t target, uint8_t amount)
{
    if (cur == target)
        return;

    if (cur < target)
    {
        uint8_t delta = target - cur;
        delta = scale8_video(delta, amount);
        cur += delta;
    }
    else
    {
        uint8_t delta = cur - target;
        delta = scale8_video(delta, amount);
        cur -= delta;
    }
}

// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor(CRGB &cur, const CRGB &target, uint8_t amount)
{
    nblendU8TowardU8(cur.red, target.red, amount);
    nblendU8TowardU8(cur.green, target.green, amount);
    nblendU8TowardU8(cur.blue, target.blue, amount);
    return cur;
}

int getNext60Value(int inValue)
{
    int nextValue = inValue + 1;

    if (nextValue == 60)
        return 0;

    return nextValue;
}

int getPrev60Value(int inValue)
{
    int nextValue = inValue - 1;

    if (nextValue == -1)
    {
        return 59;
    }

    return nextValue;
}

HourLEDStruct getHourLEDS(int currHour, int currMin)
{
    HourLEDStruct hours = HourLEDStruct();

    int realHourLED = (currHour > 12 ? currHour - 12 : currHour) * 5;

    int realMinLEDAddition = floor(currMin / 12);

    hours.currHour = realHourLED + realMinLEDAddition;
    hours.prevHour = getPrev60Value(hours.currHour);
    hours.nextHour = getNext60Value(hours.currHour);

    return hours;
}