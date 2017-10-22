/*
Copyright (c) 2014-2016 NicoHood
See the readme for credit to other people.

MSGEQ7 FastLED example
Output via Led strip and FastLED library

Reads MSGEQ7 IC with 7 different frequencies from range 0-255
63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25KHz, 16kHz
*/

// FastLED
#include "MSGEQ7.h"
#include <FastLED.h>

#define DATA_PIN 3
#define COLOR_ORDER BRG
#define CHIPSET     WS2811
#define NUM_LEDS    32

#define BRIGHTNESS  255  // reduce power consumption
#define LED_DITHER  255  // try 0 to disable flickering
#define CORRECTION  TypicalLEDStrip

CRGB leds[NUM_LEDS]; // Define the array of leds

					 // MSGEQ7
#include "MSGEQ7.h"
#define pinAnalogLeft A0
#define pinAnalogRight A0
#define pinReset 6
#define pinStrobe 4
#define MSGEQ7_INTERVAL ReadsPerSecond(50)
#define MSGEQ7_SMOOTH true

CMSGEQ7<MSGEQ7_SMOOTH, pinReset, pinStrobe, pinAnalogLeft, pinAnalogRight> MSGEQ7;

void setup() {
	// FastLED setup
	FastLED.addLeds<CHIPSET, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(CORRECTION);
	FastLED.setBrightness(BRIGHTNESS);
	FastLED.setDither(LED_DITHER);
	FastLED.show(); // needed to reset leds to zero

					// This will set the IC ready for reading
	MSGEQ7.begin();
}
uint8_t gHue = 0;
void loop() {
	// Analyze without delay
	bool newReading = MSGEQ7.read(MSGEQ7_INTERVAL);

	// Led strip output
	if (newReading) {
		// visualize the average bass of both channels
		uint8_t val = MSGEQ7.get(MSGEQ7_BASS);

		// Reduce noise
		val = mapNoise(val);

		// Visualize leds to the beat
		CRGB color = CHSV(gHue, 255, 255);
		color.nscale8_video(val);
		fill_solid(leds, NUM_LEDS, color);

		// Update Leds
		FastLED.show();
	}
	EVERY_N_MILLIS(20) { gHue++; }
}