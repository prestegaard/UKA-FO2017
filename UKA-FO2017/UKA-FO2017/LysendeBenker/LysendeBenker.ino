/*
 Name:		LysendeBenker.ino
 Created:	9/29/2017 3:04:12 PM
 Author:	Haagon
*/

// the setup function runs once when you press reset or power the board
#include "ws2811.h"
//#include "RF_driver.h"
#include <FastLED.h>

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
	#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif


	// Define which box is in use
#define BOX_ID 0
#define N_NODES 3

//Rf_driver* rf;
//rf_message_t msg_send;


#define IR_SENSOR_1_PIN A4
#define IR_SENSOR_2_PIN A5


CRGB leds[NUM_LEDS];




bool last_sensor_value;
bool current_sensor_value;
bool sensor_must_turn_high_again = false;

void setup() {
	// 3 second delay for recovery
	delay(3000); 
	Serial.begin(115200);
	pinMode(IR_SENSOR_1_PIN, INPUT);
	// tell FastLED about the LED strip configuration
	FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

	// set master brightness control
	FastLED.setBrightness(BRIGHTNESS);

	/*rf = new Rf_driver(BOX_ID, N_NODES);*/
	set_each_strip_in_bench_with_color_diff(leds, 200, 70, 30);
	
}

// the loop function runs over and over again until power down or reset

// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void(*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow_standing, rainbow_standing, rainbow_standing, sinelon, rainbow_laying, rainbow_laying, rainbow_laying, strole_up_and_down, fill_sideways, rainbow_standing, rainbow_standing, strole_up_and_down };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//byte pipe;
//rf_message_t recv_payload;

uint8_t last_sensor = 0;
void loop()
{
	//strole_column_upwards(leds, 60, CHSV(gHue, 255, BRIGHTNESS));
	//gHue += 30;
	//delay(500);

	//strole_column_downwards(leds, 60, CHSV(255 - gHue, 255, BRIGHTNESS));
	//gHue += 30;
	//delay(500);
	//
	//fill_strip_upwards(leds, 60, CHSV(gHue, 255, BRIGHTNESS));
	//gHue += 30;
	//delay(500);
	//
	//fill_strip_downwards(leds, 60, CHSV(255 - gHue, 255, BRIGHTNESS));
	//gHue += 30;
	//delay(500);

	//
	

	// Call the current pattern function once, updating the 'leds' array
	//fill_sideways();
	gPatterns[gCurrentPatternNumber]();
	//four_tile_show(leds, 1000, CHSV(gHue, 255, 255));

	// send the 'leds' array out to the actual LED strip
	FastLED.show();
	// insert a delay to keep the framerate modest
	FastLED.delay(1000 / FRAMES_PER_SECOND);


	//// do some periodic updates
	//if (!sensor_changed_to_low)
	//{
	EVERY_N_MILLISECONDS(20) { gHue++; } // slowly cycle the "base color" through the rainbow
	

	EVERY_N_SECONDS(10) { nextPattern(); } // change patterns periodically
	EVERY_N_BSECONDS(1) 
	{
		Serial.print(F("SENSOR: "));
		Serial.println(digitalRead(IR_SENSOR_1_PIN));
		if ((digitalRead(IR_SENSOR_1_PIN) == LOW) && (last_sensor == HIGH))
		{
			last_sensor =  LOW;
			for (uint8_t j = 0; j < 4; j++)
			{
				for (uint8_t i = 0; i < 12; i++)
				{
					set_strip(leds, i, CHSV(gHue, 255, 255));
				}
				delay(150);
				for (uint8_t i = 0; i < 12; i++)
				{
					set_strip(leds, i, CHSV(0, 0, 0));
				}
				delay(500);
			}
		}
	 }
	else
		last_sensor = digitalRead(IR_SENSOR_1_PIN);
}

#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

uint8_t sensor()
{
	return !digitalRead(IR_SENSOR_1_PIN);
}

void nextPattern()
{
	// add one to the current pattern number, and wrap around at the end
	gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE(gPatterns);
}

void rainbow_standing()
{
	set_rainbow_standing(leds, CHSV(gHue, 255, 255));
}

void rainbow_laying()
{
	set_rainbow_laying(leds, CHSV(gHue, 255, 255));
}

void sinelon()
{
	// a colored dot sweeping back and forth, with fading trails
	fadeToBlackBy(leds, NUM_LEDS, 20);
	int pos = beatsin16(13, 0, NUM_LEDS - 1);
	leds[pos] += CHSV(gHue, 255, 192);
}

void strole_up_and_down()
{
	strole_strip_upwards(leds, 50, CHSV(gHue, 255, 255));
	delay(1000);
	strole_strip_downwards(leds, 50, CHSV(gHue, 255, 255));
	delay(1000);
	gHue += 100;
}

void fill_sideways()
{
	fill_from_left(leds, 50, CHSV(gHue, 255, 255));
	delay(4000);
	gHue += 100;
	fill_from_right(leds, 50, CHSV(gHue, 255, 255));
	delay(4000);
	
}

void four_tiles_blink_inverted()
{
	four_tile_show(leds, 300, CHSV(gHue, 255, 255));
}