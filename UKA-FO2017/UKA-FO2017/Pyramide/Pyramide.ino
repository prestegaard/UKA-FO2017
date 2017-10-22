/*
Name:		LED_TEST.ino
Created:	9/4/2017 3:15:46 PM
Author:	Haagon
*/

#include "leds.h"
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
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120




void setup() {
	delay(3000); // 3 second delay for recovery

				 // tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	//FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
}

uint8_t gHue = 0;
// the loop function runs over and over again until power down or reset
void loop() {
	// Set each section with same color
	set_section(leds, LOW_START, LOW_END, CHSV(gHue, 255, 255));
	delay(1000);
	set_section(leds, MID_START, MID_END, CHSV(gHue, 255, 255));
	delay(1000);
	set_section(leds, TOP_START, TOP_END, CHSV(gHue, 255, 255));
	delay(1000);
	set_section(leds, BOX_START, BOX_END, CHSV(gHue, 255, 255));
	delay(5000);

	// Fill each section with different color upwards
	gHue += 50;
	fill_section(leds, LOW_START, LOW_END, 50, CHSV(gHue, 255, 255));
	delay(1000);

	gHue += 50;
	fill_section(leds, MID_START, MID_END, 50, CHSV(gHue, 255, 255));
	delay(1000);

	gHue += 50;
	fill_section(leds, TOP_START, TOP_END, 50, CHSV(gHue, 255, 255));
	delay(1000);

	// Run some sinelon loop
	sinelon(leds, CHSV(gHue, 255, 255));

	// Fade whole pyramide into new color
	fade_all(leds, 20, CHSV(gHue, 255, 255));
	delay(2000);
	gHue += 50;
	
	juggle(leds);

}
