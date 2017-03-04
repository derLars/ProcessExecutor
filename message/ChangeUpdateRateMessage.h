/*
 * ChangeUpdateRateMessage.h
 *
 *  Created on: Feb 18, 2017
 *      Author: tetrys
 */

#ifndef CHANGEUPDATERATEMESSAGE_H_
#define CHANGEUPDATERATEMESSAGE_H_

#include "Message.h"

class ChangeUpdateRateMessage : public Message {
public:
	ChangeUpdateRateMessage(uint16_t port, uint16_t updateRate) {
		valid = true;

		messageSize = 0;
		messageID = STOP_COMMAND_MESSAGE;
		this->port = port;

		optional1 = updateRate;
	}

	ChangeUpdateRateMessage(Message &obj) : Message(obj) {

	}

	/**
	 * Return requested update rate
	 */
	uint16_t getUpdateRate(void) {
		return optional1;
	}
};

#endif /* CHANGEUPDATERATEMESSAGE_H_ */
