// ws2811.h

#ifndef _WS2811_h
#define _WS2811_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif
#include <FastLED.h>


#define DATA_PIN    3
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG // Single ws2811 lights are rgb, strips are brg
#define NUM_LEDS    144 // 50 LEDS PER STRIP // 11 LEDS PER PLANK IN BENCH

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  600


#define OFFSET_BACK 72

 // Set color functions
void set_strip(CRGB* leds, uint8_t strip_number, CHSV color);
void set_each_strip_in_bench_with_color_diff(CRGB* leds, uint16_t delay_time, uint8_t hue, uint8_t hue_diff);
void set_tile(CRGB* leds, uint8_t tile_number, CHSV color);
void set_column_seat(CRGB* leds, uint8_t column_number, CHSV color);
void set_column_back(CRGB* leds, uint8_t column_number, CHSV color);
void set_pixel(CRGB* leds, uint8_t row, uint8_t column, CHSV color);
void set_rainbow_standing(CRGB* leds, CHSV color);
void set_rainbow_laying(CRGB* leds, CHSV color);

// Fill color functions
void fill_strip(CRGB* leds, uint8_t strip_number, uint16_t delay_time, CHSV color);
void fill_tile(CRGB* leds, uint8_t tile_number, uint16_t delay_time, CHSV color);
void fill_column_seat(CRGB* leds, uint8_t column_number, uint16_t delay_time, CHSV color);
void fill_column_back(CRGB* leds, uint8_t column_number, uint16_t delay_time, CHSV color);
void fill_column(CRGB* leds, uint8_t column_number, uint16_t delay_time, CHSV color);
// Show functions
void strole_strip_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void strole_strip_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void strole_column_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void strole_column_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_strip_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_strip_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_from_left(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_from_right(CRGB* leds, uint16_t delay_time, CHSV color);
void empty_strip_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void empty_strip_upwards(CRGB* leds, uint16_t delay_time, CHSV color);

void four_tile_show(CRGB* leds, uint16_t delay_time, CHSV color);

#endif

