/*
 Name:		RF_TEST.ino
 Created:	8/17/2017 10:03:11 PM
 Author:	Haagon
*/

// the setup function runs once when you press reset or power the board

#include "RF_driver.h"
#include <FastLED.h>

// Change me to select receiver or transmitter
//#define ROLE_WRITE 

#define SENDER_ID 0
#define RECEIVER_ID 1
#define N_NODES 2

Rf_driver* rf_recv;
Rf_driver* rf_send;
rf_message_t msg_targeted;
rf_message_t msg_broadcast;

void setup() {
	delay(3000); // 3 second delay for recovery
	
	Serial.begin(115200);
	Serial.println(F("RF Testbench"));
#ifdef ROLE_WRITE
	rf_send = new Rf_driver(SENDER_ID, N_NODES);
	Serial.println(F("Trasmitting node"));
	msg_targeted.sender_id = rf_send->id;
	msg_targeted.time = 255;
	msg_targeted.msg_type = MSG_ON;
	msg_targeted.hue = 255;
	msg_targeted.saturation = 255;
	msg_targeted.value = 0;

	msg_broadcast.sender_id = rf_send->id;
	msg_broadcast.time = 255;
	msg_broadcast.msg_type = MSG_ON;
	msg_broadcast.hue = 255;
	msg_broadcast.saturation = 255;
	msg_broadcast.value = 255;
	
#else	
	rf_recv = new Rf_driver(RECEIVER_ID, N_NODES);
	Serial.println(F("Receiving node"));
#endif
}

// the loop function runs over and over again until power down or reset
bool msg_type = 1;
bool have_printed_message = 0;
void loop() {
#ifdef ROLE_WRITE
	if (msg_type)
	{
		msg_targeted.value++;
		rf_send->write(msg_targeted, RECEIVER_ID);
		Serial.println(F("Sending targeted message"));
		Serial.print(F("msg_value: "));
		Serial.println(msg_targeted.value);
		msg_type = 0;
	}
	else
	{
		msg_broadcast.value--;
		rf_send->write(msg_broadcast, ADDRESS_BROADCAST_OFFSET);
		Serial.println(F("Sending broadcast message"));
		Serial.print(F("msg_value: "));
		Serial.println(msg_broadcast.value);
		msg_type = 1;
	}
	delay(5000);
#else

	// Read all the good stuff
	byte pipe;
	rf_message_t payload;
	if (rf_recv->read(&pipe, &payload))
	{
		Serial.print(F("Received data in pipe "));
		Serial.println(pipe);
		Serial.println(F("Payload: "));
		Serial.print(F("Sender Id:"));
		Serial.println(payload.sender_id);
		Serial.print(F("MSG_TYPE: "));
		Serial.println(payload.msg_type);
		Serial.print(F("Time: "));
		Serial.println(payload.time);
		Serial.print(F("Hue: "));
		Serial.println(payload.hue);
		Serial.print(F("Sat: "));
		Serial.println(payload.saturation);
		Serial.print(F("Value: "));
		Serial.println(payload.value);
		Serial.println(F("\n"));
		have_printed_message = false;
	}
	else
	{
		if (!have_printed_message)
		{
			Serial.println(F("No data available"));
			have_printed_message = true;
		}
		
	}

#endif
	

}
