/*
 * CommandExecutor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#include "CommandExecutor.h"
#include <iostream>

CommandExecutor::CommandExecutor(int port, int maxRunningCommands): maxRunningCommands(maxRunningCommands) {
	network = make_shared<NetworkModul>(port);

	networkThread = thread(&NetworkModul::startConnection, network);

	for(int i=0; i<MSGTHREAD; i++) {
		msgThreadpool.push_back(thread(&NetworkModul::processMessage, network));
	}

	//runningCommands = unordered_map<int, shared_ptr<Process>>();

}

CommandExecutor::~CommandExecutor() {
	// TODO Auto-generated destructor stub
}

void CommandExecutor::processMessage() {
	while(running) {
		auto messageStream = network->processMessage();
		auto message = splitString(messageStream, "|__|");

		if(message[0] == "runCommand" && message.size() == 6) {
			runCommand(stoi(message[2],0), message[3]);

			subscribeRate = stoi(message[4],0);
			updateSubscriber(message[5], stoi(message[1],0));
		}else if(message[0] == "stopCommand" && message.size() == 3){
			stopCommand(stoi(message[1],0));
		}else if(message[0] == "stopAllCommands" && message.size() == 2){
			stopAllCommands();
		}else if(message[0] == "subscribeRate" && message.size() == 4){
			subscribeRate = stoi(message[2],0);
			updateSubscriber(message[3], stoi(message[1],0));
		}
	}
}

void CommandExecutor::updateSubscriber(string ip, int port) {
	subscriber[ip] = port;
}

vector<string> CommandExecutor::splitString(string str, string delimiter) {
	vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
	vector<char> deli(delimiter.c_str(), delimiter.c_str() + delimiter.size() + 1);

	vector<string> result;

	string currentWord = "";
	string currentSubWord = "";
	//iterate each character and compare with given delimiter

	int i = 0;
	for(auto& chr : cstr) {
		if(chr == deli[i]) {
			i++;
			if(i >= delimiter.size()) {
				result.push_back(currentWord);
				currentWord = "";
				currentSubWord = "";
				i=0;
			}else{
				currentSubWord += chr;
			}
		}else{
			if(currentSubWord.size()) {
				currentWord += currentSubWord;
				currentSubWord = "";
			}

			currentWord += chr;

			i=0;
		}

	}

	if(currentSubWord.size()) {
		currentWord += currentSubWord;
	}

	//put last word into vector
	if(currentWord.size()){
		result.push_back(currentWord);
	}

	return result;
}

bool CommandExecutor::runCommand(int commandID, string command) {
	unique_lock<mutex> lock(commandMutex);

	auto currentProcess = runningCommands.find(commandID);
	if(currentProcess != runningCommands.end()) {
		(currentProcess->second)->endProcess();
		runningCommands.erase(commandID);
	}

	if(runningCommands.size() < maxRunningCommands) {
		auto process = make_shared<Process>(commandID, command);
		if (process->processValid) {
			runningCommands.emplace(commandID, process);
			return true;
		}
	}
	return false;
}

void CommandExecutor::stopCommand(int commandID) {
	unique_lock<mutex> lock(commandMutex);

	auto currentProcess = runningCommands.find(commandID);
	if(currentProcess != runningCommands.end()) {
		(currentProcess->second)->endProcess();
		runningCommands.erase(commandID);
	}
}

void CommandExecutor::stopAllCommands(void) {
	unique_lock<mutex> lock(commandMutex);

	for (auto& currentProcess: runningCommands) {
		currentProcess.second->endProcess();
		runningCommands.erase(currentProcess.first);
	}
}
