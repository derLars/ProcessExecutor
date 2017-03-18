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
	RunCommandMessage(uint16_t port, uint16_t commandID, string command) {
		valid = true;

		messageSize = command.size();
		messageID = RUN_COMMAND_MESSAGE;
		this->port = port;

		optional1 = commandID;

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
	 * Return the command which needs to be executed.
	 */
	string getCommand(void) {
		return payload;
	}
};



#endif /* RUNCOMMANDMESSAGE_H_ */
