/*
 * StopAllCommandsMessage.h
 *
 *  Created on: Feb 18, 2017
 *      Author: tetrys
 */

#ifndef STOPALLCOMMANDSMESSAGE_H_
#define STOPALLCOMMANDSMESSAGE_H_

#include "Message.h"

class StopAllCommandsMessage : public Message {
public:
	StopAllCommandsMessage(uint16_t port) {
		valid = true;

		messageSize = 0;
		messageID = STOP_ALL_COMMANDS_MESSAGE;
		this->port = port;

	}

	StopAllCommandsMessage(Message &obj) : Message(obj) {

	}
};

#endif /* STOPALLCOMMANDSMESSAGE_H_ */
