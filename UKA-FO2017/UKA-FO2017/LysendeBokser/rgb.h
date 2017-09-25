// rgb.h

#ifndef _RGB_h
#define _RGB_h

// Define pins for rgb strip
#define GREEN 3
#define RED 5
#define BLUE 6

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FastLED.h>

FASTLED_USING_NAMESPACE

void set_color(CHSV hsv);
void fade_up(CHSV hsv, uint8_t  speed);
void fade_down(CHSV hsv, uint8_t speed);
void color_off();

#endif

