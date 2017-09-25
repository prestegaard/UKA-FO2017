/*
 Name:		LysendeBokser.ino
 Created:	9/25/2017 1:42:05 PM
 Author:	Haagon
*/


#include <FastLED.h>

#include "RF_driver.h"
#include "rgb.h"

FASTLED_USING_NAMESPACE
#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif



#define delayTime 5


// Define which box is in use
#define BOX_ID 0
#define N_NODES 9

Rf_driver* rf;
rf_message_t msg_send;


CHSV hsv;
CRGB rgb;

unsigned long msg_recv_start_time;
unsigned long led_start_time;
unsigned long program_end_time;

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
}

// the loop function runs over and over again until power down or reset

byte pipe;
rf_message_t recv_payload;
uint8_t program = 0;

void loop() {
	if (rf->read(&pipe, &recv_payload))
	{
		msg_recv_start_time = millis();

		hsv.hue = recv_payload.hue;
		hsv.saturation = recv_payload.saturation;
		hsv.value = recv_payload.value;
		
		msg_send.hue = recv_payload.hue;
		msg_send.saturation = recv_payload.saturation;
		msg_send.value = recv_payload.value;
		msg_send.msg_type = recv_payload.msg_type;
		msg_send.time = recv_payload.time;
		msg_send.sender_id  = BOX_ID;


		switch (recv_payload.msg_type)
		{
			// Targeted message types. These messages flows upwards
			case MSG_FILL_UP:
				fade_up(hsv, recv_payload.time);
				rf->write(msg_send, BOX_ID + 1);
				break;

			case MSG_FILL_DOWN:
				fade_up(hsv, recv_payload.time);
				rf->write(msg_send, BOX_ID - 1);
				break;
				
			case MSG_SWEEP_UP:
				fade_up(hsv, recv_payload.time);
				rf->write(msg_send, BOX_ID + 1);
				fade_down(hsv, recv_payload.time);
				break;

			case MSG_SWEEP_DOWN:
				fade_up(hsv, recv_payload.time);
				rf->write(msg_send, BOX_ID - 1);
				fade_down(hsv, recv_payload.time);
				break;

			case MSG_BLINK_UP:
				delay(recv_payload.time);
				set_color(hsv);
				delay(recv_payload.time);
				color_off;
				delay(recv_payload.time);
				rf->write(msg_send, BOX_ID + 1);
				break;

			case MSG_BLINK_DOWN:
				delay(recv_payload.time);
				set_color(hsv);
				delay(recv_payload.time);
				color_off;
				delay(recv_payload.time);
				rf->write(msg_send, BOX_ID - 1);
				break;

			// Broadcasted messages that apply for all nodes
			case MSG_RAINBOW:
				if (hsv.hue + (25 * BOX_ID) > 255)
					hsv.hue = hsv.hue + (25 * BOX_ID) - 255;
				else
					hsv.hue += 25 * BOX_ID;
				set_color(hsv);
				break;

			case MSG_FADE:
				fade_up(hsv, recv_payload.time);
				break;

			case MSG_ON:
				set_color(hsv);
				break;

			case MSG_OFF:
				hsv.value = 0;
				set_color(hsv);
				break;
		}
		hsv.value = 255;
	}
	else if (BOX_ID == 0) {

		// Set color for message
		msg_send.hue = hsv.hue;
		msg_send.saturation = hsv.saturation;
		msg_send.value = hsv.value;

		// Set sender ID
		msg_send.sender_id = BOX_ID;

		// Set how many times the current program should be used
		uint8_t number_of_times = random(5, 30);
		for (uint8_t i = 0; i < number_of_times; i++)
		{
			switch (program)
			{
			case 0: // FILL UP
				msg_send.time = random(5, 50);
				msg_send.msg_type = MSG_FILL_UP;
				fade_up(hsv, msg_send.time);
				rf->write(msg_send, BOX_ID + 1);
				break;

			case 1: // SWEEP UP
				msg_send.time = random(50, 200);
				msg_send.msg_type = MSG_SWEEP_UP;
				fade_up(hsv, msg_send.time);
				rf->write(msg_send, BOX_ID + 1);
				fade_down(hsv, msg_send.time);
				break;

			case 2: // BLINK UP
				//Turn off all lights on all nodes
				msg_send.msg_type = MSG_OFF;
				rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);
				color_off();

				msg_send.time = random(50, 200);
				delay(msg_send.time);
				set_color(hsv);
				delay(msg_send.time);
				color_off();

				msg_send.msg_type = MSG_BLINK_UP;
				rf->write(msg_send, BOX_ID + 1);
				break;

			case 3: // RAINBOW
				uint8_t rainbow_delay = random(50, 250);
				for (uint8_t j = 0; j < 255; j++)
				{
					hsv.hue = j;
					msg_send.msg_type = MSG_RAINBOW;
					rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);
					set_color(hsv);
					delay(rainbow_delay);
				}
				break;
			}
			uint8_t next_hue = random(0, 100);
			hsv.hue + next_hue > 255 ? hsv.hue = hsv.hue + next_hue - 255 : hsv.hue += next_hue;
		}
	
		if (++program > 3)
			program = 0;
		program_end_time = millis();

	}
	else if(millis() - msg_recv_start_time > 6000) 
	{ // If nothing received for one minute
		if (millis() - led_start_time > 100)
		{
			if (hsv.hue < 255)
			{
				hsv.hue++;
			}
			else
			{
				hsv.hue = 0;
			}
			set_color(hsv);
			led_start_time = millis();
		}
				
	}
}
