// 
// 
// 

#include "leds.h"


// Set color functions
void set_section(CRGB * leds, uint16_t section_start, uint16_t section_end, CHSV color)
{
	for (uint16_t i = section_start; i <=  section_end; i++)
	{
		leds[i] = color;
	}
	FastLED.show();
}

void set_all(CRGB * leds, CHSV color)
{
	for (uint16_t i = 0; i < NUM_LEDS; i++)
	{
		leds[i] = color;
	}
	FastLED.show();
}

// Fill color functions
void fade_section(CRGB * leds, uint16_t section_start, uint16_t section_end, uint16_t delay_time, CHSV color)
{
	for (uint16_t i = 0; i < 255; i++)
	{
		set_section(leds, section_start, section_end, CHSV(color.hue, color.sat, i));
		delay(delay_time);
	}
}

void fade_all(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint16_t i = 0; i < 255; i++)
	{
		set_all(leds, CHSV(color.hue, color.sat, i));
		delay(delay_time);
	}
}

void fill_section(CRGB * leds, uint16_t section_start, uint16_t section_end, uint16_t delay_time, CHSV color)
{
	if (section_end > section_start) // Fill upwards
	{
		for (uint16_t i = section_start; i <= section_end; i++)
		{
			leds[i] = color;
			FastLED.show();
			delay(delay_time);
		}
	}
	else // Fill downwards
	{
		for (uint16_t i = section_start; i >= section_end; i--)
		{
			leds[i] = color;
			FastLED.show();
			delay(delay_time);
		}
	}
}

// Show functions
void blink_section(CRGB * leds, uint16_t section_start, uint16_t section_end, uint16_t delay_time, CHSV color)
{
}

void fade_color_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	fade_section(leds, LOW_START, LOW_END, delay_time, color);
	fade_section(leds, MID_START, MID_END, delay_time, color);
	fade_section(leds, TOP_START, TOP_END, delay_time, color);
	fade_section(leds, BOX_START, BOX_END, delay_time, color);
}

void fade_color_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{

	fade_section(leds, BOX_START, BOX_END, delay_time, color);
	fade_section(leds, TOP_START, TOP_END, delay_time, color);
	fade_section(leds, MID_START, MID_END, delay_time, color);
	fade_section(leds, LOW_START, LOW_END, delay_time, color);
}

void sinelon(CRGB * leds, CHSV color)
{
	unsigned long start_time = millis();
	while (millis() - start_time < 10000)
	{
		// a colored dot sweeping back and forth, with fading trails
		fadeToBlackBy(leds, TOP_END, 20);
		int pos = beatsin16(13, 0, TOP_END);
		leds[pos] += CHSV(color.hue, 255, 192);
		FastLED.delay(1000 / FRAMES_PER_SECOND);
	}
}

void juggle(CRGB * leds)
{
	unsigned long start_time = millis();
	while (millis() - start_time < 10000)
	{
		// eight colored dots, weaving in and out of sync with each other
		fadeToBlackBy(leds, TOP_END, 20);
		byte dothue = 0;
		for (int i = 0; i < 8; i++) {
			leds[beatsin16(i + 7, 0, TOP_END)] |= CHSV(dothue, 200, 255);
			dothue += 32;
		}
		FastLED.delay(1000 / FRAMES_PER_SECOND);
	}
}
