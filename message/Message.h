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
#include <iostream>
using namespace std;


#define RUN_COMMAND_MESSAGE 1
#define STOP_COMMAND_MESSAGE 2
#define STOP_ALL_COMMANDS_MESSAGE 3
#define CHANGE_UPDATE_RATE_MESSAGE 4
#define PROCESS_INFORMATION_MESSAGE 5
#define STOP_SYSTEM_MESSAGE 6

#define IP_HEADER_SIZE 20
#define HEADER_SIZE 20
#define MAX_PAYLOAD 1460


class Message {
public:
	Message(char header[HEADER_SIZE], string ip) {
		messageSize = (header[0] << 8) | (header[1] & 0xff);
		messageID 	= (header[2] << 8) | (header[3] & 0xff);
		port 		= (header[4] << 8) | (header[5] & 0xff);
		optional1 	= (header[6] << 8) | (header[7] & 0xff);
		optional2 	= (header[8] << 8) | (header[9] & 0xff);
		optional3 	= (header[10] << 8) | (header[11] & 0xff);
		optional4	= (header[12] << 8) | (header[13] & 0xff);
		optional5	= (header[14] << 8) | (header[15] & 0xff);

		optional6	= (header[16] << 24) | (header[17] << 16) | (header[18] << 8) | (header[19] & 0xff);

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
		optional4 	= 0;
		optional5 	= 0;
		optional6	= 0;

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
		optional4 	= obj.optional4;
		optional5 	= obj.optional5;
		optional6 	= obj.optional6;

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

	/**
	 * Return the size of the payload
	 */
	uint16_t getMessageSize(void) {
		return messageSize;
	}

	/**
	 * Return the message ID
	 */
	uint16_t getMessageID(void) {
		return messageID;
	}

	/**
	 * Return the IP of the sender
	 */
	string getIP(void) {
		return ip;
	}

	/**
	 * Return the port of the sender
	 */
	uint16_t getPort(void) {
		return port;
	}

	/**
	 * Return the entire message as byte stream
	 */
	vector<char> getByteMessage(void) {
		vector<char> stream;

		cout << "messageSize: " << messageSize << endl;
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

		byte = (optional4 >> 8) & 0xff;
		stream.push_back(byte);
		byte = optional4 & 0xff;
		stream.push_back(byte);

		byte = (optional5 >> 8) & 0xff;
		stream.push_back(byte);
		byte = optional5 & 0xff;
		stream.push_back(byte);

		byte = (optional6 >> 24) & 0xff;
		stream.push_back(byte);
		byte = (optional6 >> 16) & 0xff;
		stream.push_back(byte);
		byte = (optional6 >> 8) & 0xff;
		stream.push_back(byte);
		byte = optional6 & 0xff;
		stream.push_back(byte);

		for(char c : payload) {
			stream.push_back(c);
		}

		return stream;
	}

	/**
	 * Return validity of the message
	 */
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
	uint16_t optional4;
	uint16_t optional5;
	uint32_t optional6;

	string ip;
	string payload;

	bool valid;
};

#endif /* MESSAGE_H_ */
