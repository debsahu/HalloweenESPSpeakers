#include <pgmspace.h>

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <FastLED.h>
#include <Ticker.h>

const int ledPin = 13;      //D7/GPIO13
const uint8_t numLeds = 16; //16 LEDs

uint8_t max_bright = 128;

CRGB leds[numLeds];

Ticker ledActionTimer;
bool ledActionInProgress = false;
int curColor;
uint32_t curDelay;

void ledDefault(uint32_t delay)
{

    ledActionTimer.detach();
    ledActionTimer.attach_ms(delay, []() {
        ledActionInProgress = false;
        static uint8_t hue = 0;
        fill_solid(leds, numLeds, CHSV(hue++, 255, 255));
    });
}

void ledRainbow(uint32_t delay)
{

    ledActionTimer.detach();
    ledActionTimer.attach_ms(delay, []() {
        ledActionInProgress = true;
        static uint8_t hue = 0;
        fill_solid(leds, numLeds, CHSV(hue, 255, 255));
        hue = ++hue % 255;
    });
}

void ledBlink(uint32_t delay, int color)
{

    curColor = color;

    Serial.println(curColor, HEX);

    ledActionTimer.detach();
    ledActionTimer.attach_ms(delay, []() {
        ledActionInProgress = true;
        static bool t = true;
        fill_solid(leds, numLeds, t ? curColor : CRGB::Black);
        t = !t;
    });
}

void ledSine(uint32_t delay, int color)
{

    curColor = color;

    ledActionTimer.detach();
    ledActionTimer.attach_ms(delay, []() {
        ledActionInProgress = true;

        static uint8_t i = 0;
        static CHSV hsvColor;
        if (curColor)
        {
            hsvColor = rgb2hsv_approximate(curColor);
            curColor = 0;
        }
        hsvColor.value = sin8(i);
        fill_solid(leds, numLeds, hsvColor);
        i = (i + 1) % 255;
    });
}

void ledPulse(uint32_t delay, int color)
{

    curColor = color;
    curDelay = delay;

    ledActionTimer.detach();
    ledActionTimer.attach_ms(delay, []() {
        ledActionInProgress = true;

        static uint16_t blackCountdown = 0;

        if (blackCountdown)
        {
            blackCountdown--;
            return;
        }

        static uint8_t i = 0;
        CRGB rgbColor = curColor;

        rgbColor.fadeToBlackBy(i);
        fill_solid(leds, numLeds, rgbColor);
        i = (i + 1) % 255;

        if (i == 0)
        {
            blackCountdown = 750 / curDelay; // Stay black during 750ms
        }
    });
}

void ledDisco(uint32_t delay)
{
    ledActionTimer.detach();
    ledActionTimer.attach_ms(delay, []() {
        ledActionInProgress = true;
        fill_solid(leds, numLeds, CHSV(random8(), 255, 255));
    });
}

void ledSolid(int color)
{
    ledActionTimer.detach();
    fill_solid(leds, numLeds, color);
}

void ledOff()
{
    ledSolid(0x000000);
}

void setupLEDs()
{
    FastLED.addLeds<WS2812B, ledPin, GRB>(leds, numLeds).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(max_bright);
    FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
    ledDefault(50);
}

void ledLoop()
{
    FastLED.show();
}