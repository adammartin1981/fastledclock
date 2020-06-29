#include <Arduino.h> /* we need arduino functions to implement this */
#include <Clock.h>
#include <FastLED.h>
#include <Time.h>

#define NUM_LEDS 60
#define LED_PIN 4
#define BRIGHTNESS 64

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB chaseLEDS[NUM_LEDS];

// Need to set some variables
static int currentLED = 0;
static int targetLED = 0;

static bool initialSweep = false;

void chaser_init() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(chaseLEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

// Fade to Black later
void chaser_fadeToBlack(int ledToKeep, CRGB ledSet[60]) {
    for (int i = 0; i<NUM_LEDS; i++) {
        if (i != currentLED) {
            ledSet[i].fadeToBlackBy(35);
        }
    }
}

void chaser_updateTarget(int newTarget) {
    targetLED = newTarget;
    initialSweep = true;
}

void increment60th(int &inValue) {
    inValue++;
    if (inValue == 60 ) inValue = 0;
}

bool isLEDAheadOfOther(int led1, int led2) {
    if (led1 == 0 && led2 == 59) {
        return true;
    }

    if (led1 > led2) {
        return true;
    }

    return false;
}

void updateSecondLEDS() {
    EVERY_N_MILLIS(10)
    {
        if (currentLED != targetLED || initialSweep)
        {
            increment60th(currentLED);
            chaseLEDS[currentLED] = CRGB::Purple;
        }
        // Need to tidy
        if (isLEDAheadOfOther(currentLED, targetLED) && initialSweep)
        {
            Serial.println("Reset initial sweep to false");
            // Gone past the 'target'
            initialSweep = false;
        }

        chaser_fadeToBlack(currentLED, chaseLEDS);
    }
}

void chaser_loop() {
    updateSecondLEDS();
    
    FastLED.show();
}