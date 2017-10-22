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
#define NUM_LEDS    900 // 50 LEDS PER STRIP 
// 400 body
// 100 wings
// 400 head

#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  600

#define BODY_START 0
#define BODY_NUMBER_OF_LEDS 400
#define BODY_NUMBER_OF_ROWS 50


#define WING_NUMBER_OF_LEDS 50
#define RIGHT_WING_START 400
#define LEFT_WING_START 450
#define WING_RIGHT 0
#define WING_LEFT 1

#define HEAD_START 500
#define HEAD_NUMBER_OF_LEDS 400

// Set color functions
void set_strip(CRGB* leds, uint8_t strip_number, CHSV color);
void set_body(CRGB* leds,  CHSV color);
void set_body_row(CRGB* leds, uint8_t row, CHSV color);
void set_wing(CRGB* leds, uint8_t wing, CHSV color);
void set_head(CRGB* leds, CHSV color);
void set_rainbow_standing(CRGB* leds, CHSV color);
void set_rainbow_laying(CRGB* leds, CHSV color);

// Fade color functions
void fade_head_in(CRGB* leds, uint16_t delay_time, CHSV color);
void fade_head_out(CRGB* leds, uint16_t delay_time, CHSV color);

// Show functions
void strole_strip_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void strole_strip_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void strole_column_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void strole_column_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_body_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_body_upwards(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_from_left(CRGB* leds, uint16_t delay_time, CHSV color);
void fill_from_right(CRGB* leds, uint16_t delay_time, CHSV color);
void empty_strip_downwards(CRGB* leds, uint16_t delay_time, CHSV color);
void empty_strip_upwards(CRGB* leds, uint16_t delay_time, CHSV color);

void four_tile_show(CRGB* leds, uint16_t delay_time, CHSV color);

#endif

