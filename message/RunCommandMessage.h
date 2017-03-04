/*
 * RunCommandMessage.h
 *
 *  Created on: Feb 18, 2017
 *      Author: tetrys
 */

#ifndef RUNCOMMANDMESSAGE_H_
#define RUNCOMMANDMESSAGE_H_

#include "Message.h"

class RunCommandMessage : public Message {
public:
	RunCommandMessage(uint16_t port, uint16_t commandID, uint16_t rootRight, string command) {
		valid = true;

		messageSize = command.size();
		messageID = RUN_COMMAND_MESSAGE;
		this->port = port;

		optional1 = commandID;
		optional2 = rootRight;

		if(messageSize > MAX_PAYLOAD) {
			valid = false;
		}

		payload = command;
	}

	RunCommandMessage(Message &obj) : Message(obj) {

	}

	/**
	 * Return the unique commandID of the command to execute.
	 */
	uint16_t getCommandID(void) {
		return optional1;
	}

	/**
	 * Return the demand for root rights.
	 * 0 = no root
	 * 1 = root
	 */
	bool hasRootRight(void) {
		return optional2 > 0;
	}

	/**
	 * Return the command which needs to be executed.
	 */
	string getCommand(void) {
		return payload;
	}
};



#endif /* RUNCOMMANDMESSAGE_H_ */
