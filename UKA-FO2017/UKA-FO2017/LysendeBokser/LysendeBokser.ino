/*
 Name:		LysendeBokser.ino
 Created:	9/25/2017 1:42:05 PM
 Author:	Haagon
*/



#include <Wire.h>
#include <ds3231.h>
#include <FastLED.h>

#include "RF_driver.h"
#include "rgb.h"

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif



#define delayTime 5

// RTC STUFF
#define BUFF_MAX 128
uint8_t time[8];
char recv[BUFF_MAX];

// Define which box is in use
#define BOX_ID 4
#define N_NODES 9

Rf_driver* rf;
rf_message_t msg_send;


CHSV hsv;
CRGB rgb;

unsigned long msg_recv_start_time;
unsigned long led_start_time;
unsigned long program_end_time;
bool initial_boot;

// the setup function runs once when you press reset or power the board
void setup() {
	delay(3000); // 3 second delay for recovery

	Serial.begin(115200);
	Serial.println(F("Lysende Boks"));
	
	// Set rgb pins as output
	pinMode(GREEN, OUTPUT);
	pinMode(BLUE, OUTPUT);
	pinMode(RED, OUTPUT);


	hsv.sat = 255;
	hsv.val = 255;
	hsv.hue = HUE_RED;
	
	// Fade inn different leds
	fade_up(hsv, delayTime);

	hsv.hue = HUE_GREEN;
	fade_up(hsv, delayTime);

	hsv.hue = HUE_BLUE;
	fade_up(hsv, delayTime);

	rf = new Rf_driver(BOX_ID, N_NODES);
	
	
	hsv.hue = 0;
	hsv.value = 255;
	hsv.saturation = 255;
	led_start_time = millis();
	msg_recv_start_time = 0;

	initial_boot = true;

	if (BOX_ID == 0)
	{
		Wire.begin();
		DS3231_init(DS3231_INTCN);
		memset(recv, 0, BUFF_MAX);
	}
}

// the loop function runs over and over again until power down or reset

byte pipe;
rf_message_t recv_payload;
uint8_t program = 0;
bool fade_up_or_down = 1;
uint8_t number_of_program_cycles = 0;
void loop() {
	if (rf->read(&pipe, &recv_payload)  && BOX_ID != 0)
	{
		if (!initial_boot)
		{
			Serial.print(F("RECEIVED MESSAGE ON PIPE:  "));
			Serial.println(pipe);
			Serial.print(F("MESSAGE TIME (USED FOR CHECK): "));
			Serial.println(recv_payload.time);
		}
		if ((pipe == 0 || pipe == 1) && (recv_payload.time != 0))
		{
			hsv.hue = recv_payload.hue;
			hsv.saturation = recv_payload.saturation;
			hsv.value = recv_payload.value;

			msg_send.hue = recv_payload.hue;
			msg_send.saturation = recv_payload.saturation;
			msg_send.value = recv_payload.value;
			msg_send.msg_type = recv_payload.msg_type;
			msg_send.time = recv_payload.time;
			msg_send.sender_id = BOX_ID;

			Serial.println(F("Payload: "));
			Serial.print(F("MSG_TYPE: "));
			Serial.println(recv_payload.msg_type);
			Serial.print(F("Time: "));
			Serial.println(recv_payload.time);
			Serial.print(F("Hue: "));
			Serial.println(recv_payload.hue);

			rf_message_t msg_dummy;
			byte dummy;

			switch (recv_payload.msg_type)
			{
				// Targeted message types. These messages flows upwards
				case MSG_FILL_UP:
					fade_up(hsv, recv_payload.time);
					rf->write(msg_send, BOX_ID + 1);
					initial_boot = false;
					break;

				case MSG_FILL_DOWN:
					fade_up(hsv, recv_payload.time);
					rf->write(msg_send, BOX_ID - 1);
					initial_boot = false;
					break;
					
				case MSG_SWEEP_UP:
					fade_up(hsv, recv_payload.time);
					rf->write(msg_send, BOX_ID + 1);
					fade_down(hsv, recv_payload.time);
					initial_boot = false;
					break;

				case MSG_SWEEP_DOWN:
					fade_up(hsv, recv_payload.time);
					rf->write(msg_send, BOX_ID - 1);
					fade_down(hsv, recv_payload.time);
					initial_boot = false;
					break;

				case MSG_BLINK_UP:
					set_color(hsv);
					delay(recv_payload.time * 10);
					color_off();
					rf->write(msg_send, BOX_ID + 1);
					initial_boot = false;
					break;

				case MSG_BLINK_DOWN:
					set_color(hsv);
					delay(recv_payload.time);
					color_off();
					rf->write(msg_send, BOX_ID - 1);
					initial_boot = false;
					break;

				// Broadcasted messages that apply for all nodes
				case MSG_RAINBOW:
					delay(8);
					
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					for (uint8_t j = 0; j < 255; j++)
					{
						hsv.hue = j;
						if (hsv.hue + (25 * BOX_ID) > 255)
							hsv.hue = hsv.hue + (25 * BOX_ID) - 255;
						else
							hsv.hue += 25 * BOX_ID;
						set_color(hsv);
						delay(recv_payload.time);
					}
					initial_boot = false;
					break;

				case MSG_FADE:
					delay(8);
					
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					
					fade_up(hsv, recv_payload.time);
					initial_boot = false;
					break;

				case MSG_ON:
					delay(8);

					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);

					set_color(hsv);
					initial_boot = false;
					break;

				case MSG_OFF:
					delay(8);

					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					rf->read(&dummy, &msg_dummy);
					
					color_off();
					initial_boot = false;
					break;
			}
			msg_recv_start_time = millis();
		}
	}
	else if (BOX_ID == 0) {
		Serial.println(F("SENDING MESSAGE"));
		
		// Set sender ID
		msg_send.sender_id = BOX_ID;
		
		// Read RTC
		char buff[BUFF_MAX];
		struct ts t;
		DS3231_get(&t);
		snprintf(buff, BUFF_MAX, "%d.%02d.%02d %02d:%02d:%02d", t.year,
			t.mon, t.mday, t.hour, t.min, t.sec);
		Serial.println(buff);
		
		if (t.mon < 10 || t.mday < 4 || (t.hour >= 17 && t.hour <= 23) || (t.hour >= 0 && t.hour <= 5))
		{
			Serial.println("MONTH LESS THEN OCTOBER AND LATER THAN 17");

			// Set how many times the current program should be used
			uint8_t number_of_times = random(0, 5);
			uint8_t rainbow_delay;
			uint8_t delay_iteration = 0;
			for (uint8_t i = 0; i < number_of_times; i++)
			{
				// Set color for message
				msg_send.hue = hsv.hue;
				msg_send.saturation = hsv.saturation;
				msg_send.value = hsv.value;
				
				switch (program)
				{
				case 0: // FILL UP
					msg_send.time = random(3, 15);
					msg_send.msg_type = MSG_FILL_UP;
					fade_up(hsv, msg_send.time);
					rf->write(msg_send, BOX_ID + 1);
					Serial.println(F("Payload: "));
					Serial.print(F("MSG_TYPE: "));
					Serial.println(msg_send.msg_type);
					Serial.print(F("Time: "));
					Serial.println(msg_send.time);
					Serial.print(F("Hue: "));
					Serial.println(msg_send.hue);
					for (delay_iteration = 0; delay_iteration < 9; delay_iteration++)
					{
						delay((255 * msg_send.time) + 150);
					}
					break;

				case 1: // SWEEP UP
					msg_send.time = random(2, 15);
					msg_send.msg_type = MSG_SWEEP_UP;
					fade_up(hsv, msg_send.time);
					rf->write(msg_send, BOX_ID + 1);
					fade_down(hsv, msg_send.time);
					Serial.println(F("Payload: "));
					Serial.print(F("MSG_TYPE: "));
					Serial.println(msg_send.msg_type);
					Serial.print(F("Time: "));
					Serial.println(msg_send.time);
					Serial.print(F("Hue: "));
					Serial.println(msg_send.hue);
					for (delay_iteration = 0; delay_iteration < 9; delay_iteration++)
					{
						delay((255 * msg_send.time) + 150);
					}
					break;

				case 2: // BLINK UP
					//Turn off all lights on all nodes
				/*	msg_send.msg_type = MSG_OFF;
					rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);
					color_off();
	*/
					msg_send.time = random(60, 200);
					/*delay(1000);*/
					set_color(hsv);
					delay(msg_send.time * 10);
					color_off();

					msg_send.msg_type = MSG_BLINK_UP;
					rf->write(msg_send, BOX_ID + 1);

					Serial.println(F("Payload: "));
					Serial.print(F("MSG_TYPE: "));
					Serial.println(msg_send.msg_type);
					Serial.print(F("Time: "));
					Serial.println(msg_send.time);
					Serial.print(F("Hue: "));
					Serial.println(msg_send.hue);
					for (delay_iteration = 0; delay_iteration < 10; delay_iteration++)
					{
						delay((4 * msg_send.time) + 150);
						Serial.print("DELAY BLINK");
						Serial.println(delay_iteration);
					}
					break;

				case 3: // RAINBOW
					
					msg_send.msg_type = MSG_RAINBOW;
					rainbow_delay = random(10, 250);
					msg_send.time = rainbow_delay;
					msg_send.hue = 0;
					rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);

					Serial.println(F("Payload: "));
					Serial.print(F("MSG_TYPE: "));
					Serial.println(msg_send.msg_type);
					Serial.print(F("Time: "));
					Serial.println(rainbow_delay);
					Serial.print(F("Hue: "));
					Serial.println(0);

					delay(10);
					for (uint8_t j = 0; j < 255; j++)
					{
						hsv.hue = j;
						set_color(hsv);

						delay(rainbow_delay);
					}
					delay(1000);
					break;

				case 4: //FADE
					msg_send.msg_type = MSG_FADE;
					msg_send.time = random(3, 100);
					rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);
					delay(10);
					fade_up(hsv, msg_send.time);
					delay(5 * 1000);
					break;

			}

				Serial.print("CURRENT HUE: ");
				Serial.print(hsv.hue);
				uint8_t next_hue = random(0, 100);
				hsv.hue + next_hue > 255 ? hsv.hue = hsv.hue + next_hue - 255 : hsv.hue += next_hue;
				Serial.print(" NEXT HUE: ");
				Serial.print(next_hue);
				Serial.print(" UPDATED HUE: ");
				Serial.println(hsv.hue);


			}
		}
		else
		{
			color_off();
			msg_send.msg_type = MSG_OFF;
			rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);
			delay(5 * 1000);
		}
		program = random(0, 4);
		program_end_time = millis();

	}
	if (((millis() - msg_recv_start_time > 60 * 1000) || (initial_boot == true)) && BOX_ID != 0)
	{ // If nothing received for one minute or no incomming messages on radio
		
		if (program == 0) // RAINBOW
		{
			hsv.value = 255;
			hsv.saturation = 255;
			if (millis() - led_start_time > 100)
			{
				// CALCULATE NEXT COLOR IN RAINBOW
				if (++hsv.hue == 255)
				{
					hsv.hue = 0;
					Serial.print(F("RAINBOW: "));
					Serial.println(number_of_program_cycles);
					// GO TO NEXT PROGRAM IF NEEDED
					if (++number_of_program_cycles > 3)
					{
						program = 1;
						number_of_program_cycles = 0;
						fade_up_or_down = false; // Start by fading down
					}
				}
				set_color(hsv);
				led_start_time = millis();
			}
		}
		else if (program == 1) // FADE UP AND DOWN
		{
			if (millis() - led_start_time > 5)
			{
				if (fade_up_or_down == true) // FADE UP
				{
					if (++hsv.value == 255)
					{
						fade_up_or_down = false;
					}
				}
				else // FADE DOWN
				{
					if (--hsv.value == 0)
					{
						Serial.print(F("FADE DOWN, VALUE = 0: "));
						Serial.println(number_of_program_cycles);
						Serial.print(F("HUE: "));
						Serial.println(hsv.hue);
						fade_up_or_down = true;
						uint8_t next_hue = random(0, 100); // CHANGE TO SOME OTHER COLOR
						hsv.hue + next_hue > 255 ? hsv.hue = hsv.hue + next_hue - 255 : hsv.hue += next_hue;
						if (++number_of_program_cycles > 10)
						{
							program = 0;
							number_of_program_cycles = 0;
						}
					}
				}
				set_color(hsv);
				led_start_time = millis();
			}
		}
		
	}
}
