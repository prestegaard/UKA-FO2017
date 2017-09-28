/*
 Name:		LED_TEST.ino
 Created:	9/4/2017 3:15:46 PM
 Author:	Haagon
*/

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
	#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    3
	//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG // Single ws2811 lights are rgb, strips are brg
#define NUM_LEDS    50 // 50 LEDS PER STRIP
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          255
#define FRAMES_PER_SECOND  600

uint8_t gHue = 0; // rotating "base color" used by many of the patterns


void setup() {
	delay(3000); // 3 second delay for recovery

				 // tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	//FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
}

// the loop function runs over and over again until power down or reset
void loop() {
	juggle();
	FastLED.show();
	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / FRAMES_PER_SECOND);

	// do some periodic updates
	EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
}

void rainbow()
{
	// FastLED's built-in rainbow generator
	fill_rainbow(leds, NUM_LEDS, gHue, 5);
}

void confetti()
{
	// random colored speckles that blink in and fade smoothly
	fadeToBlackBy(leds, NUM_LEDS, 10);
	int pos = random16(NUM_LEDS);
	leds[pos] += CHSV(gHue + random8(64), 200, 255);
}
void sinelon()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS - 1);
	leds[pos] += CHSV(gHue, 255, 192);
}

void juggle() {
	// eight colored dots, weaving in and out of sync with each other
	fadeToBlackBy(leds, NUM_LEDS, 20);
	byte dothue = 0;
	for (int i = 0; i < 16; i++) {
		leds[beatsin16(i + 7, 0, NUM_LEDS - 1)] |= CHSV(dothue, 200, 255);
		dothue += 32;
	}
}