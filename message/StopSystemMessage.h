/*
 * StopSystemMessage.h
 *
 *  Created on: Feb 18, 2017
 *      Author: tetrys
 */

#ifndef STOPSYSTEMMESSAGE_H_
#define STOPSYSTEMMESSAGE_H_

#include "Message.h"

class StopSystemMessage : public Message {
public:
	StopSystemMessage() {
		valid = true;
		messageSize = 0;
	}

	StopSystemMessage(Message &obj) : Message(obj) {

	}
};



#endif /* STOPSYSTEMMESSAGE_H_ */
