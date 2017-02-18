/*
 * Message.h
 *
 *  Created on: Feb 10, 2017
 *      Author: tetrys
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <string>
#include <vector>

using namespace std;


#define RUN_COMMAND_MESSAGE 1
#define MAX_PAYLOAD 1468


class Message {
public:
	Message(char header[12], string ip) {
		messageSize = (header[0] << 8) | (header[1] & 0xff);
		messageID 	= (header[2] << 8) | (header[3] & 0xff);
		port 		= (header[4] << 8) | (header[5] & 0xff);
		optional1 	= (header[6] << 8) | (header[7] & 0xff);
		optional2 	= (header[8] << 8) | (header[9] & 0xff);
		optional3 	= (header[10] << 8) | (header[11] & 0xff);

		this->ip = ip;
		valid = true;

	}

	Message() {
		messageSize = 0;
		messageID 	= 0;
		port 		= 0;
		optional1 	= 0;
		optional2 	= 0;
		optional3 	= 0;
		ip 			= "";
		valid = false;
	}

	Message(const Message &obj) {
		messageSize = obj.messageSize;
		messageID 	= obj.messageID;
		port 		= obj.port;
		optional1 	= obj.optional1;
		optional2 	= obj.optional2;
		optional3 	= obj.optional3;

		payload 	= obj.payload;
		ip 			= obj.ip;
		valid 		= obj.valid;
	}

	void setPayload(string payload) {
		this->payload = payload;
	}

	string getPayload(void) {
		return payload;
	}

	uint16_t getMessageSize(void) {
		return messageSize;
	}

	uint16_t getMessageID(void) {
		return messageID;
	}

	string getIP(void) {
		return ip;
	}

	uint16_t getPort(void) {
		return port;
	}

	vector<char> getByteMessage(void) {
		vector<char> stream;

		char byte = (messageSize >> 8) & 0xff;
		stream.push_back(byte);
		byte = messageSize & 0xff;
		stream.push_back(byte);

		byte = (messageID >> 8) & 0xff;
		stream.push_back(byte);
		byte = messageID & 0xff;
		stream.push_back(byte);

		byte = (port >> 8) & 0xff;
		stream.push_back(byte);
		byte = port & 0xff;
		stream.push_back(byte);

		byte = (optional1 >> 8) & 0xff;
		stream.push_back(byte);
		byte = optional1 & 0xff;
		stream.push_back(byte);

		byte = (optional2 >> 8) & 0xff;
		stream.push_back(byte);
		byte = optional2 & 0xff;
		stream.push_back(byte);

		byte = (optional3 >> 8) & 0xff;
		stream.push_back(byte);
		byte = optional3 & 0xff;
		stream.push_back(byte);

		for(char c : payload) {
			stream.push_back(c);
		}

		return stream;
	}

	bool isValid(void) {
		return valid;
	}

protected:

	uint16_t messageSize;
	uint16_t messageID;
	uint16_t port;
	uint16_t optional1;
	uint16_t optional2;
	uint16_t optional3;

	string ip;
	string payload;

	bool valid;
};

/**
 *
 */
class RunCommandMessage : public Message {
public:
	RunCommandMessage(string ip, uint16_t port, uint16_t commandID, uint16_t rootRight, string command) {
		valid = true;

		this->ip = ip;

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
	 * returns the unique commandID of the command to execute
	 */
	uint16_t getCommandID(void) {
		return optional1;
	}

	bool hasRootRight(void) {
		return optional2 > 0;
	}

	string getCommand(void) {
		return payload;
	}
};



#endif /* MESSAGE_H_ */
