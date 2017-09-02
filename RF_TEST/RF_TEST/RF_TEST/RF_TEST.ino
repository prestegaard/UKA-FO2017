/*
 Name:		RF_TEST.ino
 Created:	8/17/2017 10:03:11 PM
 Author:	Haagon
*/

// the setup function runs once when you press reset or power the board

#include "RF_driver.h"
#include <FastLED.h>

#define ROLE_WRITE

Rf_driver* rf_recv;
Rf_driver* rf_send;
rf_message_t msg;

void setup() {
	delay(3000); // 3 second delay for recovery
	
	Serial.begin(115200);
	Serial.println(F("RF Testbench"));
	rf_recv = new Rf_driver(1, 2);
	rf_send = new Rf_driver(0, 2);

	msg.sender_id = rf_send->id;
	msg.time = 0;
	msg.msg_type = MSG_ON;
	msg.hue = 130;
	msg.saturation = 255;
	msg.value = 100;
}

// the loop function runs over and over again until power down or reset
void loop() {
	EVERY_N_MILLIS(100) {
		
	}
#ifdef ROLE_WRITE
	msg.hue++;
	rf_send->write(msg, rf_recv->id);
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
	}
	else
	{
		Serial.println(F("No data available"));
	}

#endif
	delay(5000);

}
