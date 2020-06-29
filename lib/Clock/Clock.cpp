#include <Arduino.h> /* we need arduino functions to implement this */
#include <Clock.h>
#include <FastLED.h>
#include <Time.h>

#define NUM_LEDS 60
#define LED_PIN 4
#define BRIGHTNESS 128

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB leds[NUM_LEDS];
CHSV hsvLeds[NUM_LEDS];
Fader ledsToFade[NUM_LEDS];

static int prevSecond = -1;

void clock_init()
{
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void setSecond(int nowSecond) {
    prevSecond = nowSecond;
}

void loopSecond(int nowSecond) {
    // leds[nowSecond] = CRGB::White;
    ledsToFade[nowSecond].timeAdded = millis();
    ledsToFade[nowSecond].timeToFadeOver = 8000;
    ledsToFade[nowSecond].ledHSV = CHSV(0, 0, 255);
    setSecond(nowSecond);
}

void updateLEDS() {
    unsigned long now = millis();
    for (int f = 0; f < NUM_LEDS; f++ ){
        Fader fader = ledsToFade[f];
        if (fader.timeAdded) {
            // Utils?
            unsigned long startTime = fader.timeAdded;
            int timeEllapsed = now - startTime;
            int timeToFadeOver = fader.timeToFadeOver;
            
            float percThrough = (float)timeEllapsed / timeToFadeOver;
        
            int newFadeVal = map(percThrough * 100, 0, 100, 255, 10);

            ledsToFade[f].ledHSV.v = newFadeVal;

            if (newFadeVal <= 11) {
                ledsToFade[f].timeAdded = 0;
                ledsToFade[f].ledHSV.v = 0;
            }
        }
    }
}

void writeLEDS() {
    for (int led = 0; led < NUM_LEDS; led++)
    {
        // Need to check for valid LED
        CHSV ledToUse = ledsToFade[led].timeAdded ? ledsToFade[led].ledHSV : CHSV(0,0,0);
        hsv2rgb_rainbow(ledsToFade[led].ledHSV, leds[led]);
    }
    FastLED.show();
}

void chase(CRGB col, int del)
{
    for (int lead_led = 0; lead_led < NUM_LEDS; lead_led++)
    {
        for (int dot = 0; dot < NUM_LEDS; dot++)
        {
            leds[dot].fadeToBlackBy(30);
        }
        leds[lead_led] = CHSV(255, 0, 250);
        FastLED.show();
        delay(del);
    }
}

void clock_loop() {
    int nowSecond = second();

    // if (nowSecond != prevSecond)
    // {
        loopSecond(nowSecond);
    // }

    updateLEDS();
    writeLEDS();
}

void clock_loop2()
{
    static int currentLed = 0;

    // Start at one - spin all the way around - stop at next one - do nothing else.


    EVERY_N_MILLISECONDS(10) {
        leds[currentLed] = CRGB::Red;
        currentLed++;
        if (currentLed == 60) currentLed = 0;

        // Try and only do this for the ones that are set?
        for (int n = 0; n < NUM_LEDS; n++)
        {
            if (leds[n])
            {
                leds[n].fadeToBlackBy(30);
            }
        }
    }

    FastLED.show();
}