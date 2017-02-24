/*
 * StopCommandMessage.h
 *
 *  Created on: Feb 18, 2017
 *      Author: tetrys
 */

#ifndef STOPCOMMANDMESSAGE_H_
#define STOPCOMMANDMESSAGE_H_

#include "Message.h"

class StopCommandMessage : public Message {
public:
	StopCommandMessage(uint16_t port, uint16_t pid) {
		valid = true;

		messageSize = 0;
		messageID = STOP_COMMAND_MESSAGE;
		this->port = port;

		optional1 = pid;
	}

	StopCommandMessage(Message &obj) : Message(obj) {

	}

	/**
	 * returns the unique PID of the command to stop.
	 */
	uint16_t getPid(void) {
		return optional1;
	}
};



#endif /* STOPCOMMANDMESSAGE_H_ */
