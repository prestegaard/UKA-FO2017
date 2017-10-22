// leds.h

#ifndef _LEDS_h
#define _LEDS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FastLED.h>

#define DATA_PIN    3
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG // Single ws2811 lights are rgb, strips are brg
#define NUM_LEDS    150 // 50 LEDS PER STRIP 
#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  600

#define LOW_START 0
#define LOW_END 49
#define MID_START 50
#define MID_END 74
#define TOP_START 75
#define TOP_END 99
#define BOX_START 100
#define BOX_END 149


// Set color functions
void set_section(CRGB* leds, uint16_t section_start, uint16_t section_end, CHSV color);
void set_all(CRGB* leds, CHSV color);

// Fade color functions
void fade_section(CRGB* leds, uint16_t section_start, uint16_t section_end, uint16_t delay_time, CHSV color);
void fade_all(CRGB* leds, uint16_t delay_time, CHSV color);

// Fill color functions
void fill_section(CRGB* leds, uint16_t section_start, uint16_t section_end, uint16_t delay_time, CHSV color);
// Show functions
void blink_section(CRGB* leds, uint16_t section_start, uint16_t section_end, uint16_t delay_time, CHSV color);
void fade_color_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fade_color_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void sinelon(CRGB* leds, CHSV color);
void juggle(CRGB* leds);
#endif

