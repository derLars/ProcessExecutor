/*
 * ProcessInformationMessage.h
 *
 *  Created on: Feb 18, 2017
 *      Author: tetrys
 */

#ifndef PROCESSINFORMATIONMESSAGE_H_
#define PROCESSINFORMATIONMESSAGE_H_

#include "Message.h"

class ProcessInformationMessage : public Message {
public:

	ProcessInformationMessage(uint16_t port, uint16_t pid, uint16_t cpuUsage, uint16_t memUsage, uint16_t timestamp, string command) {
		valid = true;

		messageID = PROCESS_INFORMATION_MESSAGE;
		this->port = port;

		optional1 = pid;
		optional2 = cpuUsage;
		optional3 = memUsage;
		optional6 = timestamp / 100;

		payload = command;

		messageSize = payload.size();
	}

	ProcessInformationMessage(Message &obj) : Message(obj) {

	}

	/**
	 * Return information about the process ID of the process
	 */
	uint16_t getPid(void) {
		return optional1;
	}

	/**
	 * Return information about the CPU usage of the process
	 */
	uint16_t getCpuUsage(void) {
		return optional2;
	}

	/**
	 * Return information about the memory usage of the process
	 */
	uint16_t getMemUsage(void) {
		return optional3;
	}

	/**
	 * Return the timestamp of the current record
	 */
	uint32_t getTimeStamp(void) {
		return optional6;
	}

	/**
	 * Return the command of the process
	 */
	string getCommand(void) {
		return payload;
	}

private:

};

#endif /* PROCESSINFORMATIONMESSAGE_H_ */
