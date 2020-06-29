#include <Arduino.h> /* we need arduino functions to implement this */
#include <Clock.h>
#include <FastLED.h>
#include <Time.h>

#define NUM_LEDS 60
#define LED_PIN 4
#define BRIGHTNESS 128

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB chaseLEDS[NUM_LEDS];
// CHSV hsvLeds[NUM_LEDS];
// Fader ledsToFade[NUM_LEDS];

// Need to set some variables
static int currentLED = 0;
static int targetLED = 0;

static bool targetLogged = false;

static bool initialSweep = true;

void chaser_init() {
    Serial.println("Chase Init");
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(chaseLEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

// Fade to Black later
void chaser_fadeToGreen() {
    for (int i = 0; i<NUM_LEDS; i++) {
        if (i != currentLED) {
            chaseLEDS[i].fadeToBlackBy(35);
            // chaseLEDS[i] = CRGB(0, 64, 0);
            // chaseLEDS[i] = CRGB::Black;
        }
    }
}

void chaser_updateTarget(int newTarget) {
    Serial.print("UPDATE TARGET AND RESET SWEEP: ");
    Serial.print(newTarget);
    Serial.print(" current led: ");
    Serial.println(currentLED);
    targetLED = newTarget;
    targetLogged = false;
    initialSweep = true;
}

void increment60th(int &inValue) {
    inValue++;
    if (inValue == 60 ) inValue = 0;
}

CRGB getAlternateColor(int ledNum)
{


    CRGB colour = CRGB::Red + CRGB::Purple;

    // if (chaseLEDS[ledNum].r == 255)
    // {
    //     colour = CRGB::Green;
    // }

    return colour;
}



void chaser_loop() {
    // BLOCKING
    EVERY_N_MILLIS(10) {
        if (currentLED == targetLED && !initialSweep)
        {
            if (!targetLogged) {
                Serial.println();
                Serial.println("Reached Target");
                targetLogged = true;
            }
        } else if (currentLED != targetLED || initialSweep) {
            increment60th(currentLED);
            Serial.print(currentLED);
            Serial.print(",");
            chaseLEDS[currentLED] = getAlternateColor(currentLED);
        } 
        // Need to tidy
        if ((currentLED > targetLED || currentLED == 0 && targetLED == 59) && initialSweep)
        {
            Serial.println("Reset initial sweep to false");
            // Gone past the 'target'
            initialSweep = false;
        }

        chaser_fadeToGreen();
    }

    FastLED.show();
}