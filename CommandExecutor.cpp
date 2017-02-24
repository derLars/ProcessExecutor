/*
 * CommandExecutor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#include "CommandExecutor.h"
#include <iostream>

CommandExecutor::CommandExecutor(int port, int maxRunningCommands): maxRunningCommands(maxRunningCommands), running(true) {
	network = make_shared<NetworkModul>(port);

	networkThread = thread(&NetworkModul::startConnection, network);

	while(!network->settedUp){}
	running = network->running;

	subscribeRate = 1000;

	for(int i=0; i<MSGTHREAD; i++) {
		msgThreadpool.push_back(thread(&CommandExecutor::processMessage, this));
	}
}

CommandExecutor::~CommandExecutor() {
	network->stopConnection();
}

void CommandExecutor::processMessage() {
	while(running) {
		auto message = network->processMessage();

		switch(message->getMessageID()) {
			case RUN_COMMAND_MESSAGE: {
				cout << "Processing runCommandMessage" << endl;
				//auto runCommandMessage = static_pointer_cast<RunCommandMessage>(message);

				 updateSubscriber(message);

				 runCommand(static_pointer_cast<RunCommandMessage>(message));

				 break;
			}
			case STOP_COMMAND_MESSAGE: {
				cout << "Processing stopCommandMessage" << endl;

				updateSubscriber(message);

				stopCommand(static_pointer_cast<StopCommandMessage>(message));
				break;
			}
			case STOP_ALL_COMMANDS_MESSAGE: {
				cout << "Processing stopAllCommandsMessage" << endl;

				updateSubscriber(message);

				stopAllCommands();
				break;
			}
			case CHANGE_UPDATE_RATE_MESSAGE: {
				cout << "Processing changeUpdateRateMessage" << endl;

				updateSubscriber(message);
				break;
			}
		}
	}
}

void CommandExecutor::sendMessage(int port, string message) {
	//network->sendMessage("localhost", port, message);
}

void CommandExecutor::updateSubscriber(shared_ptr<Message> message) {
	//Add subscriber
	 auto key = subscriber.find (message->getIP());
	 if( key == subscriber.end() ) {
		 auto clientFd = network->connectToClient(message->getIP(), message->getPort());
		 subscriber[message->getIP()] = clientFd;
	 }
}

/*vector<string> CommandExecutor::splitString(string str, string delimiter) {
	vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
	vector<char> deli(delimiter.c_str(), delimiter.c_str() + delimiter.size() + 1);

	vector<string> result;

	string currentWord = "";
	string currentSubWord = "";
	//iterate each character and compare with given delimiter

	unsigned int i = 0;
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
}*/

void CommandExecutor::runCommand(shared_ptr<RunCommandMessage> message) {
	unique_lock<mutex> lock(commandMutex);

	 //End current running process
	 auto alreadyRunning = runningProcess.find (message->getCommandID());
	 if(alreadyRunning != runningProcess.end()){
		 (alreadyRunning->second)->endProcess();
		 runningProcess.erase(message->getCommandID());
	 }

	 //Run process
	 if(runningProcess.size() < maxRunningCommands) {
		 runningProcess[message->getCommandID()] = make_shared<Process>(message->getCommandID(),message->getCommand());
	 }
}

void CommandExecutor::stopCommand(shared_ptr<StopCommandMessage> message) {
	unique_lock<mutex> lock(commandMutex);

	for (auto& currentProcess: runningProcess) {
		if(message->getPid() == stoi(currentProcess.second->processID)) {
			cout << "Found process to end!" << endl;
			(currentProcess.second)->endProcess();
			runningProcess.erase(currentProcess.second->commandID);
			break;
		}
	}
}

void CommandExecutor::stopAllCommands(void) {
	unique_lock<mutex> lock(commandMutex);

	for (auto& currentProcess: runningProcess) {
		currentProcess.second->endProcess();
	}
	runningProcess.clear();
}

vector<int> CommandExecutor::getListOfRunningProcesses(void) {
	vector<int> returnList;

	for (auto& currentProcess: runningProcess) {
		returnList.push_back(stoi(currentProcess.second->processID));
	}
	return returnList;
}
