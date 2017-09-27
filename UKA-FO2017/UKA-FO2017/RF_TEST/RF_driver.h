// RF_driver.h

#ifndef _RF_DRIVER_h
#define _RF_DRIVER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include <SPI.h>

#include <RF24_config.h>
#include <RF24.h>
#include <nRF24L01.h>

#define NODE_TYPE_BOX 0xF0F0F0F000
#define NODE_TYPE_BENCH 0xF0F0F0F100

// Change me to define what type of node I am
#define NODE_TYPE_BASE_ADDRESS NODE_TYPE_BOX

#define ADDRESS_BROADCAST_OFFSET 0XF

// Flow message types
#define MSG_FILL_UP 0x00
#define MSG_FILL_DOWN 0x01
#define MSG_SWEEP_UP 0x02
#define MSG_SWEEP_DOWN 0x03
#define MSG_BLINK_UP 0x04
#define MSG_BLINK_DOWN 0x05
#define MSG_LOOP_UP 0x06
#define MSG_LOOP_DOWN 0x07

// Non-flow message types
#define MSG_RAINBOW 0xA0
#define MSG_FADE 0xA1
#define MSG_ON 0xA2
#define MSG_OFF 0xA3

struct rf_message_t
{
	uint8_t sender_id;
	uint8_t msg_type;
	uint8_t time; // Function varies with msg_type

	uint8_t hue;
	uint8_t saturation;
	uint8_t value;
};

class Rf_driver : public RF24
{
public:
	uint8_t id;
	uint8_t n_nodes;
	uint8_t current_mode;

	RF24* radio; //Radio uses pin 7 & 8

	Rf_driver(int id, int n_nodes);
	/*
	@return 1 if commands should be sent to increasing id, -1 for decreasing
	*/
	uint64_t getNextId(uint8_t current_id);

	void write(rf_message_t payload, uint8_t id);
	/*
	@return true if data available
	*/
	bool read(byte* pipe, rf_message_t* payload);

};

#endif

