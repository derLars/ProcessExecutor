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

	ProcessInformationMessage(uint16_t port, uint16_t pid, uint16_t cpuUsage, uint16_t memUsage, vector<int> listOfProcesses) {
		valid = true;

		messageID = PROCESS_INFORMATION_MESSAGE;
		this->port = port;

		optional1 = pid;
		optional2 = cpuUsage;
		optional3 = memUsage;

		payload = "";

		for(auto process : listOfProcesses) {
			if(process != listOfProcesses.front()){
				payload += " ";
			}
			payload += to_string(process);
		}

		messageSize = payload.size();
	}

	ProcessInformationMessage(Message &obj) : Message(obj) {

	}

	/**
	 *
	 */
	uint16_t getPid(void) {
		return optional1;
	}

	/**
	 *
	 */
	uint16_t getCpuUsage(void) {
		return optional2;
	}

	/**
	 *
	 */
	uint16_t getMemUsage(void) {
		return optional3;
	}

	vector<int> getListOfProcesses(void) {
		return splitString(payload, ' ');
	}

private:
	vector<int> splitString(string str, char delimiter) {
		vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
		vector<int> result;
		string currentWord = "";

		//iterate each character and compare with given delimiter
		for(auto& chr : cstr) {
			if(chr == delimiter && currentWord.size()) {
				result.push_back(stoi(currentWord));
				currentWord = "";
			}else if(chr != delimiter){
				currentWord += chr;
			}
		}

		//put last word into vector
		if(currentWord.size()){
			result.push_back(stoi(currentWord));
		}

		return result;
	}
};



#endif /* PROCESSINFORMATIONMESSAGE_H_ */
