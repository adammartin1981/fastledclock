#include <Arduino.h> /* we need arduino functions to implement this */

#define FASTLED_INTERNAL

#include <FastLED.h>
#include <Time.h>
#include <Helper.h>

#define NUM_LEDS 60
#define LED_PIN 4 // Use 2 on ESP8266
#define BRIGHTNESS 64

#define LED_TYPE WS2812B
#define COLOR_ORDER GRB

CRGB secondLEDS[NUM_LEDS];
CRGB clockLEDS[NUM_LEDS];
CRGB minuteLEDS[NUM_LEDS];
CRGB hourLEDS[NUM_LEDS];
CRGB handLEDS[NUM_LEDS];

// Need to set some variables
static int currentSecondLED = 0;
static int currentMinuteLED = 0;
static int currentHourLED = 0;
static int targetSecondLED = 0;
static int targetMinuteLED = 0;
static int targetHourLED = 0;

static bool initialSweep = false;

void chaser_init() {
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(clockLEDS, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
}

void chaser_fadeToBlack(int ledToKeep, CRGB ledSet[60], int speed) {
    for (int i = 0; i<NUM_LEDS; i++) {
        if (i != currentSecondLED) {
            ledSet[i].fadeToBlackBy(speed);
        }
    }
}

void chaser_updateSecond(int newSecond) {
    targetSecondLED = newSecond;
    initialSweep = true;
}

void chaser_updateMinute(int newMinute)
{
    targetMinuteLED = newMinute;
}

void chaser_updateHour(int newHour)
{
    targetHourLED = newHour;
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

void updateMinutesLEDS() {
    if (targetMinuteLED != currentMinuteLED) {
        currentMinuteLED = targetMinuteLED;

        int nextMinute = getNext60Value(currentMinuteLED);

        // Hidden start colour
        minuteLEDS[nextMinute] = CHSV(255, 255, 0);
    }

    EVERY_N_SECONDS(1) {
        int nextMinute = getNext60Value(currentMinuteLED);

        minuteLEDS[currentMinuteLED] = CRGB::Red;
        fadeTowardColor(minuteLEDS[nextMinute], CRGB::Red, 255/60);

        chaser_fadeToBlack(currentMinuteLED, minuteLEDS, 15);
    }
}

void updateHourLEDS() {
    // Ensure we have the latest hour set
    if (targetHourLED != currentHourLED)
    {
        currentHourLED = targetHourLED;        
    }

    HourLEDStruct hoursNow = getHourLEDS(currentHourLED, currentMinuteLED);

    EVERY_N_SECONDS(1)
    {
        hourLEDS[hoursNow.prevHour].fadeToBlackBy(20);
        hourLEDS[hoursNow.currHour] = CRGB::Green;
    }
}

void updateSecondLEDS() {
    EVERY_N_MILLIS(10)
    {
        if (currentSecondLED != targetSecondLED || initialSweep)
        {
            increment60th(currentSecondLED);
            secondLEDS[currentSecondLED] = CRGB::Purple;
        }

        if (isLEDAheadOfOther(currentSecondLED, targetSecondLED) && initialSweep)
        {
            // Gone past the target
            initialSweep = false;
        }

        chaser_fadeToBlack(currentSecondLED, secondLEDS, 35);
    }
}

void initialize(CRGB *arr, std::initializer_list<std::size_t> list, CRGB value)
{
    for (auto i : list)
    {
        arr[i] = value;
    }
}

void updateHands() {
    initialize(handLEDS, {0}, CRGB(15, 15, 0));
    initialize(handLEDS, {15,30,45}, CRGB(0, 10, 10));
    initialize(handLEDS, {5, 10, 20, 25, 35, 40, 50, 55}, CRGB(4, 4, 4));
}

void combineTime() {
    // Get all the diff LEDS and combine them
    for(int i = 0; i < NUM_LEDS; i++) {
        CRGB combinedColour = minuteLEDS[i] + secondLEDS[i] + hourLEDS[i] + handLEDS[i]; 

        clockLEDS[i] = combinedColour;
    }
}

void chaser_loop() {
    updateSecondLEDS();
    updateMinutesLEDS();
    updateHourLEDS();
    updateHands();

    combineTime();
    
    FastLED.show();
}