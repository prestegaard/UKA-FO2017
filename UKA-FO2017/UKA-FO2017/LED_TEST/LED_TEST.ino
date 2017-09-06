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
#define COLOR_ORDER GRB
#define NUM_LEDS    400 // 50 LEDS PER STRIP
CRGB leds[NUM_LEDS];

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

// ADDRESS SPACE PER SECTION PYRAMIDE
/*
Low
l0s, l0m, l1s, l1mm, ...
Mid
m0s, m0m, m1s, m1m, ...
High
h0s, h0m, h1s, h1m, ...
*/

uint8_t addresses[3][8] = {
	{L0s, L0m, L1s, L1m, L2m, L3s, L3m},
	[]
}

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
  
}
