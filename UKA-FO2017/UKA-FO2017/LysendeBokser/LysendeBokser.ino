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
#define BOX_ID 1
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

	color_off();
}

// the loop function runs over and over again until power down or reset

byte pipe;
rf_message_t recv_payload;
uint8_t program = 0;

void loop() {
	if (rf->read(&pipe, &recv_payload)  && BOX_ID != 0)
	{
		
		if (pipe == 7)
		{ 
			Serial.println(F("ERROR: RECEIVED BAD MESSAGE"));
			Serial.print(F("Received data in pipe "));
			Serial.println(pipe);
			Serial.println(F("Payload: "));
			Serial.print(F("Sender Id:"));
			Serial.println(recv_payload.sender_id);
			Serial.print(F("MSG_TYPE: "));
			Serial.println(recv_payload.msg_type);
			Serial.print(F("Time: "));
			Serial.println(recv_payload.time);
			Serial.print(F("Hue: "));
			Serial.println(recv_payload.hue);
			Serial.print(F("Sat: "));
			Serial.println(recv_payload.saturation);
			Serial.print(F("Value: "));
			Serial.println(recv_payload.value);
			Serial.println(F("\n"));
		}
		else
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
			msg_send.sender_id = BOX_ID;

			Serial.println(F("RECEIVED MESSAGE"));
			Serial.println(F("Payload: "));
			Serial.print(F("MSG_TYPE: "));
			Serial.println(recv_payload.msg_type);
			Serial.print(F("Time: "));
			Serial.println(recv_payload.time);
			Serial.print(F("Hue: "));
			Serial.println(recv_payload.hue);

			delay(2000);

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
					set_color(hsv);
					delay(recv_payload.time);
					color_off();
					rf->write(msg_send, BOX_ID + 1);
					break;

				case MSG_BLINK_DOWN:
					set_color(hsv);
					delay(recv_payload.time);
					color_off();
					rf->write(msg_send, BOX_ID - 1);
					break;

				// Broadcasted messages that apply for all nodes
				case MSG_RAINBOW:
					delay(8);
					rf_message_t msg_dummy;
					byte dummy;
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
					break;

				case MSG_FADE:
					fade_up(hsv, recv_payload.time);
					break;

				case MSG_ON:
					set_color(hsv);
					break;

				case MSG_OFF:
					color_off();
					break;
			}
		}
	}
	else if (BOX_ID == 0) {

		Serial.println(F("SENDING MESSAGE"));
		
		// Set sender ID
		msg_send.sender_id = BOX_ID;


		
		

		// Set how many times the current program should be used
		uint8_t number_of_times = random(5, 30);
		for (uint8_t i = 0; i < number_of_times; i++)
		{
			// Set color for message
			msg_send.hue = hsv.hue;
			msg_send.saturation = hsv.saturation;
			msg_send.value = hsv.value;
			program = 3;
			switch (program)
			{
			case 0: // FILL UP
				msg_send.time = random(5, 50);
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
				delay(255 * 8 * msg_send.time + 100);
				break;

			case 1: // SWEEP UP
				msg_send.time = random(5, 50);
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
				delay(255 * 8 * msg_send.time + 100);
				break;

			case 2: // BLINK UP
				//Turn off all lights on all nodes
			/*	msg_send.msg_type = MSG_OFF;
				rf->write(msg_send, ADDRESS_BROADCAST_OFFSET);
				color_off();
*/
				msg_send.time = random(300, 1000);
				/*delay(1000);*/
				set_color(hsv);
				delay(msg_send.time);
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
				delay(3 * 16 * msg_send.time + 100);
				break;

			case 3: // RAINBOW
				uint8_t rainbow_delay = random(50, 250);
				msg_send.msg_type = MSG_RAINBOW;
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
	
		if (++program > 3)
			program = 0;
		program_end_time = millis();

	}
	else if(millis() - msg_recv_start_time > 15 * 1000)
	{ // If nothing received for one minute
		if (millis() - led_start_time > 100)
		{
			hsv.value = 255;
			hsv.saturation = 255;
			Serial.println(F("RAINBOW"));
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
