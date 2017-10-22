// 
// 
// 

#include "ws2811.h"

void set_strip(CRGB * leds, uint8_t strip_number, CHSV color)
{
	
	for (uint8_t i = 0; i<12; i++)

	{
		leds[strip_number * 12 + i] = color;
	}
	FastLED.show();

}

void set_each_strip_in_bench_with_color_diff(CRGB * leds, uint16_t delay_time, uint8_t hue, uint8_t hue_diff)
{
	for (uint8_t i = 0; i<12; i++)
	{
		CHSV color_output;
		hue + (hue_diff * i) > 255 ? color_output.hue = hue + (hue_diff * i) - 255 : color_output.hue = hue + (hue_diff * i);
		set_strip(leds, i, color_output);
		delay(delay_time);
	}
	
	}

void set_tile(CRGB * leds, uint8_t tile_number, CHSV color)
{
	for (uint8_t j = 0; j <= 4; j += 2) // EVERY SECOND ROW
	{
		for (uint8_t i = 0; i < 6; i++) // COLUMN
		{
			switch (tile_number)
			{
			case 0:
				leds[i + (j * 12)] = color;
				leds[i + 6 + ((j+1) * 12)] = color;
				break;
			case 1:
				leds[i + 6 + (j * 12)] = color;
				leds[i + ((j + 1) * 12)] = color;
				break;
			case 3:
				leds[i + (j * 12) + OFFSET_BACK] = color;
				leds[i + 6 + ((j + 1) * 12) + OFFSET_BACK] = color;
				break;
			case 2: 
				leds[i + 6 + (j * 6) + OFFSET_BACK] = color;
				leds[i + ((j + 1) * 12) + OFFSET_BACK] = color;
				break;
			}
		}
	}
	FastLED.show();
}

void set_column_seat(CRGB * leds, uint8_t column_number, CHSV color)
{
	leds[column_number] = color;
	leds[23 - column_number] = color;
	leds[24 + column_number] = color;
	leds[47 - column_number] = color;
	leds[48 + column_number] = color;
	leds[71 - column_number] = color;
	FastLED.show();
}

void set_column_back(CRGB * leds, uint8_t column_number, CHSV color)
{
	leds[column_number + OFFSET_BACK] = color;
	leds[23 - column_number + OFFSET_BACK] = color;
	leds[24 + column_number + OFFSET_BACK] = color;
	leds[47 - column_number + OFFSET_BACK] = color;
	leds[48 + column_number + OFFSET_BACK] = color;
	leds[71 - column_number + OFFSET_BACK] = color;
	FastLED.show();
}

void set_pixel(CRGB * leds, uint8_t row, uint8_t column, CHSV color)
{
	if ((row % 2) == 0)
		leds[column + (row * 12)] = color;
	else
		leds[11 - column + (row * 12)] = color;
	FastLED.show();
}

void set_column_seat(CRGB * leds, uint8_t column_number, uint16_t delay_time, CHSV color)
{
}

void set_column_back(CRGB * leds, uint8_t column_number, uint16_t delay_time, CHSV color)
{
}

void fill_strip(CRGB * leds, uint8_t strip_number, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		set_strip(leds, i, color);
	}
}

void fill_tile(CRGB * leds, uint8_t tile_number, uint16_t delay_time, CHSV color)
{
}

void fill_column_seat(CRGB * leds, uint8_t column_number, uint16_t delay_time, CHSV color)
{
}

void fill_column_back(CRGB * leds, uint8_t column_number, uint16_t delay_time, CHSV color)
{
}

void fill_column(CRGB * leds, uint8_t column_number, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 255; i++)
	{
		set_column_seat(leds, column_number, CHSV(color.hue, color.sat, i));
		set_column_back(leds, column_number, CHSV(color.hue, color.sat, i));
		delayMicroseconds(delay_time);
		
	}
}


void strole_strip_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (int8_t row = 11; row >= -1; row--)
	{
		if (row < 11)
			set_strip(leds, row + 1, CHSV(0, 0, 0));
		if (row >= 0)
			set_strip(leds, row, color);
		FastLED.show();
		delay(delay_time);
	}

}
void strole_column_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (int8_t i = 11; i >= -1; i--)
	{
		if (i < 11) { set_column_seat(leds, i + 1, CHSV(0, 0, 0)); } // Clean up last column
		if (i >= 0) { set_column_seat(leds, i, color); }; // Set column
		FastLED.show();
		delay(delay_time);
	}
}

void strole_strip_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint8_t row = 0; row < 13; row++)
	{
		if(row < 12)
			set_strip(leds, row, color);
		if (row > 0)
			set_strip(leds, row - 1, CHSV(0, 0, 0));
		FastLED.show();
		delay(delay_time);
	}
}

void strole_column_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 13; i++)
	{
		if (i > 0) { set_column_seat(leds, i - 1, CHSV(0, 0, 0)); } // Clean up last column
		if (i < 12) { set_column_seat(leds, i, color); }; // Set column
		FastLED.show();
		delay(delay_time);
	}

}

void fill_strip_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint8_t i = 0; i < 12; i++)
	{
		set_strip(leds, 11-i, color);
		delay(delay_time);
	}
}

void fill_strip_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	{
		for (uint8_t i = 0; i < 12; i++)
		{
			set_strip(leds, i, color);
			delay(delay_time);
		}
	}
}

void fill_from_left(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (uint8_t column = 0; column < 12; column++)
	{
		set_column_back(leds, column, color);
		set_column_seat(leds, column, color);
		delay(delay_time);
	}
}

void fill_from_right(CRGB * leds, uint16_t delay_time, CHSV color)
{
	for (int8_t column = 11; column >= 0; column--)
	{
		set_column_back(leds, column, color);
		set_column_seat(leds, column, color);
		delay(delay_time);
	}
}
void empty_strip_downwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
	
}

void empty_strip_upwards(CRGB * leds, uint16_t delay_time, CHSV color)
{
}

void four_tile_show(CRGB * leds, uint16_t delay_time, CHSV color)
{
	set_tile(leds, 0, color);
	set_tile(leds, 1, color);
	set_tile(leds, 2, color);
	set_tile(leds, 3, color);
	delay(delay_time);
	// Set all, then blink inverterted
	for (uint8_t i = 0; i < 2; i++) {
		for(uint8_t tile = 0; tile < 4; tile++)
		{
			set_tile(leds, tile, CHSV(0, 0 ,0));
			if (tile > 0)
				set_tile(leds, tile - 1, color);
			if (tile == 3)
				set_tile(leds, 0, color);
			delay(delay_time);
		}
	}
}

void set_rainbow_standing(CRGB * leds, CHSV color)
{
	CHSV color_rainbow = color;
	for (uint8_t column = 0; column < 12; column++)
	{
		set_column_seat(leds, column, color_rainbow);
		set_column_back(leds, column, color_rainbow);
	    color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}

void set_rainbow_laying(CRGB * leds, CHSV color)
{
	CHSV color_rainbow = color;
	for (uint8_t row = 0; row < 12; row++)
	{
		set_strip(leds, row, color_rainbow);
		color_rainbow.hue + 21 > 255 ? color_rainbow.hue = color_rainbow.hue + 21 - 255 : color_rainbow.hue += 21;
	}
}


