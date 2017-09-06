// leds.h

#ifndef _LEDS_h
#define _LEDS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include <FastLED.h>

#define NUM_LEDS    261

// side = 25, corner = 5
#define L0s 0
#define L0m 25
#define L1s 30
#define L1m 55
#define L2s 60
#define L2m 65
#define L3s 70
#define L3m 75
#define L4s 80
#define L4m 105

// side = 15, corner = 4
#define M0s 110
#define M0m 125
#define M1s 129
#define M1m 144
#define M2s 149
#define M2m 153
#define M3s 157
#define M3m 172
#define M4s 176
#define M4m 191

// side = 10, corner = 3
#define H0s 196
#define H0m 206
#define H1s 209
#define H1m 219
#define H2s 222
#define H2m 232
#define H3s 235
#define H3m 245
#define H4s 248
#define H4m 258

uint8_t addresses[3][8] = {
	{ L0s, L0m, L1s, L1m, L2m, L3s, L3m },
	{ M0s, M0m, M1s, M1m, M2m, M3s, M3m },
	{ H0s, H0m, H1s, H1m, H2m, H3s, H3m }
};



void blink_each_section_round_upwards(struct CRGB * LED, uint8_t speed, uint8_t color);
void blink_each_section_upwards(uint8_t speed, uint8_t color);
void blink_each_section_upwards(uint8_t speed, uint8_t color);

#endif

