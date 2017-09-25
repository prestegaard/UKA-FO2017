// 
// 
// 
#include "rgb.h"

void set_color(CHSV hsv)
{
	CRGB rgb;
	hsv2rgb_rainbow(hsv, rgb);
	analogWrite(RED, rgb.r);
	analogWrite(GREEN, rgb.g);
	analogWrite(BLUE, rgb.b);
}

void fade_up(CHSV hsv, uint8_t  speed)
{
	CRGB rgb;
	for (uint8_t i = 0; i<255; i++)
	{
		hsv.value = i;
		hsv2rgb_rainbow(hsv, rgb);
		analogWrite(RED, rgb.r);
		analogWrite(GREEN, rgb.g);
		analogWrite(BLUE, rgb.b);
		delay(speed);
	}
}

void fade_down(CHSV hsv, uint8_t speed)
{
	CRGB rgb;
	for (uint8_t i = 0; i<255; i++)
	{
		hsv.value = 255 - i;
		hsv2rgb_rainbow(hsv, rgb);
		analogWrite(RED, rgb.r);
		analogWrite(GREEN, rgb.g);
		analogWrite(BLUE, rgb.b);
		delay(speed);
	}
}

void color_off()
{
	analogWrite(RED, 0);
	analogWrite(GREEN, 0);
	analogWrite(BLUE, 0);
}

