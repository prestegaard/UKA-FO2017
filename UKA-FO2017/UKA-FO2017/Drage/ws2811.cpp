// 
// 
// 

#include "ws2811.h"

void set_strip(CRGB * leds, uint8_t strip_number, CHSV color)
{

	for (uint8_t i = 0; i<50; i++)

	{
		leds[strip_number * 50 + i] = color;
	}
	FastLED.show();

}

void set_body(CRGB * leds, CHSV color)
{
	for (uint16_t i = BODY_START; i < BODY_START + BODY_NUMBER_OF_LEDS; i++)
	{
		leds[i] = color;
	}
	FastLED.show();
}

void set_body_row(CRGB * leds, uint8_t row, CHSV color)
{
	leds[row] = color;
	leds[99 - row] = color;
	leds[100 + row] = color;
	leds[199 - row] = color;
	leds[200 + row] = color;
	leds[299 - row] = color;
	leds[300 + row] = color;
	leds[399 - row] = color;
	FastLED.show();
}

void set_wing(CRGB * leds, uint8_t wing, CHSV color)
{
	if (wing == WING_RIGHT)
	{
		for (uint16_t i = RIGHT_WING_START; i < RIGHT_WING_START + WING_NUMBER_OF_LEDS; i++)
		{
			leds[i] = color;
		}
	}
	else if (wing == WING_LEFT)
	{
		for (uint16_t i = LEFT_WING_START; i < LEFT_WING_START + WING_NUMBER_OF_LEDS; i++)
		{
			leds[i] = color;
		}
	}
	FastLED.show();
}

void set_head(CRGB * leds, CHSV color)
{
	for (uint16_t i = HEAD_START; i < HEAD_START + HEAD_NUMBER_OF_LEDS; i++)
	{
		leds[i] = color;
	}
	FastLED.show();
}




void fade_head_in(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		set_head(leds, CHSV(color.hue, color.sat, i));
		delay(delay_time);
	}
}

void fade_head_out(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 255; i > 0; i--)
	{
		set_head(leds, CHSV(color.hue, color.sat, i));
		delay(delay_time);
	}
	set_head(leds, CHSV(color.hue, color.sat, 0));
	delay(delay_time);
}


void fill_body_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (int8_t i = BODY_NUMBER_OF_ROWS -1; i >= 0; i--)
	{
		set_body_row(leds, i, color);
		delay(delay_time);
	}
}

void fill_body_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	{
		for (uint8_t i = 0; i < BODY_NUMBER_OF_ROWS; i++)
		{
			set_body_row(leds, i, color);
			delay(delay_time);
		}
	}
}


void empty_strip_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{

}

void empty_strip_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
}


