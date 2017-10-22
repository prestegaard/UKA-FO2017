/*
 Name:		Drage.ino
 Created:	10/3/2017 1:18:55 AM
 Author:	Haagon
*/

// the setup function runs once when you press reset or power the board
#include "ws2811.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
	#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

CRGB leds[NUM_LEDS];

uint8_t gHue = 0;

void setup() {
	delay(3000); // 3 second delay for recovery

				 // tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
	//FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);
	set_body(leds, CHSV(HUE_GREEN, 255, 255));
	delay(3000);
	set_wing(leds, WING_RIGHT, CHSV(HUE_BLUE, 255, 255));
	delay(3000);
	set_wing(leds, WING_LEFT, CHSV(HUE_BLUE, 255, 255));
	delay(3000);
	set_head(leds, CHSV(HUE_RED, 255, 255));
	delay(3000);
}
// the loop function runs over and over again until power down or reset
void loop() {
		fill_body_upwards(leds, 100, CHSV(gHue, 255, 255));
		delay(1000);
		gHue += 100;
		fill_body_downwards(leds, 100, CHSV(gHue, 255, 255));
		gHue += 100;
	
		fade_head_in(leds, 2, CHSV(gHue, 255, 255));
		fade_head_out(leds, 2, CHSV(gHue, 255, 255));
		fade_head_in(leds, 2, CHSV(gHue, 255, 255));
		gHue += 100;
	
		set_wing(leds, WING_RIGHT, CHSV(gHue, 255, 255));
		set_wing(leds, WING_RIGHT, CHSV(gHue, 255, 255));
		gHue += 100;
}
