// 
// 
// 

#include "RF_driver.h"

Rf_driver::Rf_driver(int id, int n_nodes) : id(id), n_nodes(n_nodes)
{
	radio = new RF24(7, 8);
	radio->begin();
	radio->setPALevel(RF24_PA_MAX);
	radio->setDataRate(RF24_250KBPS);
	//radio->enableDynamicPayloads();
	
	// Some dummy address on pipe 0
	radio->openReadingPipe(0, NODE_TYPE_BASE_ADDRESS + 0XAA);
	radio->openReadingPipe(1,NODE_TYPE_BASE_ADDRESS + id);
	radio->openReadingPipe(2, NODE_TYPE_BASE_ADDRESS + ADDRESS_BROADCAST_OFFSET);
	
	radio->enableDynamicAck();
	radio->setAutoAck(2, false);

	radio->startListening();
	Serial.println(F("Listening on the folling pipes and addressess :"));
	Serial.print(F("1 : "));
	Serial.print(id);
	Serial.print(F(", 2 : "));
	Serial.println(ADDRESS_BROADCAST_OFFSET);
}

uint64_t Rf_driver::getNextId(uint8_t current_id)
{
	if (current_id != ADDRESS_BROADCAST_OFFSET && current_mode < 0xA0)
	{
		if (current_mode == MSG_FILL_UP || current_mode == MSG_SWEEP_UP || current_mode == MSG_BLINK_UP)
		{
			return NODE_TYPE_BASE_ADDRESS + current_id + 1;
		}
		else if (current_mode == MSG_FILL_DOWN || current_mode == MSG_SWEEP_DOWN || current_mode == MSG_BLINK_DOWN)
		{
			return NODE_TYPE_BASE_ADDRESS + current_id - 1;
		}
		else if (current_mode == MSG_LOOP_UP)
		{
			if (current_id + 1 >= n_nodes)
				return NODE_TYPE_BASE_ADDRESS;
			return NODE_TYPE_BASE_ADDRESS + current_id + 1;
		}
		else if (current_mode == MSG_LOOP_DOWN)
		{
			if (current_id -1 < 0)
				return NODE_TYPE_BASE_ADDRESS + n_nodes -1;
			return NODE_TYPE_BASE_ADDRESS + current_id - 1;
		}
	}
	return NODE_TYPE_BASE_ADDRESS+ADDRESS_BROADCAST_OFFSET;
}

void Rf_driver::write(rf_message_t payload, uint8_t dst_id)
{
	

	// Serialize payload
	byte data[] = { payload.sender_id, payload.msg_type, payload.time, payload.hue, payload.saturation, payload.value };

	// Set address
	uint64_t address = NODE_TYPE_BASE_ADDRESS + dst_id;
	
	
	// Stop listening
	radio->stopListening();
	
	// Broadcast or targeted message
	bool broadcast = (dst_id == ADDRESS_BROADCAST_OFFSET) ? (true) : (false);
	if (broadcast) 
	{ 
		radio->setAutoAck(0, 0); 
		delay(1);
		radio->openWritingPipe(address);
		// Broadcast message 5 times
		radio->write(&data, 6, broadcast);
		delay(1);
		radio->write(&data, 6, broadcast);
		delay(1);
		radio->write(&data, 6, broadcast);
		delay(1);
		radio->write(&data, 6, broadcast); 
		delay(1);
		radio->write(&data, 6, broadcast);
	}
	else
	{
		radio->setAutoAck(0, 1);
		delay(1);
		radio->openWritingPipe(address);
		int attempt = 1;
		while (!radio->write(&data, 6, broadcast)) {
			Serial.print(F("Failed to send "));
			Serial.println(attempt);
			if (attempt >= 3) {
				address = getNextId(dst_id);
				if (address < NODE_TYPE_BASE_ADDRESS + n_nodes && address >= NODE_TYPE_BASE_ADDRESS)
				{
					radio->openWritingPipe(address);
					attempt = 0;
				}
				else
				{
					break;
				}
			}
			attempt++;
		}
	}
	radio->startListening();
}

bool Rf_driver::read(byte* pipe, rf_message_t * payload)
{
	if (radio->available(pipe))
	{
		// Read data
		byte buf[6];
		radio->read(buf, 6);

		// De-serialize payload
		payload->sender_id = buf[0];
		payload->msg_type = buf[1];
		payload->time = buf[2];
		payload->hue = buf[3];
		payload->saturation = buf[4];
		payload->value = buf[5];
		
		return true;
	}
	return false;
}