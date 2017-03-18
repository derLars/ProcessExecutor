/*
 * CommandExecutor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#include "CommandExecutor.h"
#include <iostream>

CommandExecutor::CommandExecutor(int port, int maxRunningCommands, int numThreads): running(true), maxRunningCommands(maxRunningCommands), updateRate(DEFAULT_UPDATE_RATE){
	network = make_shared<NetworkModul>(port);

	networkThread = thread(&NetworkModul::startConnection, network);

	mutex dummyMutex;
	unique_lock<mutex> lock(dummyMutex);
	while(!network->settedUp) {
		network->networkReady.wait(lock);
	}

	//while(!network->settedUp){}
	running = network->running;

	cout << "I want to create " << numThreads << " threads" << endl;
	for(int i=0; i<numThreads; i++) {
		msgThreadpool.push_back(thread(&CommandExecutor::processMessage, this));
	}

	updateThread = thread(&CommandExecutor::updateSubscriber, this);

	start = std::chrono::system_clock::now();

	networkThread.join();
	updateThread.join();

	for(int i=0; i<MSGTHREAD; i++) {
			msgThreadpool.at(i).join();
	}
}

CommandExecutor::~CommandExecutor() {
	running = false;
	network->stopConnection();
}

void CommandExecutor::processMessage() {
	while(running) {
		//take an incomming message from the list
		auto message = network->processMessage();

		//process the message, depending on the message type
		switch(message->getMessageID()) {
			case RUN_COMMAND_MESSAGE: {
				cout << "Processing runCommandMessage" << endl;

				 runCommand(static_pointer_cast<RunCommandMessage>(message));

				 updateSubscriberList(message);
				 break;
			}
			case STOP_COMMAND_MESSAGE: {
				cout << "Processing stopCommandMessage" << endl;

				stopCommand(static_pointer_cast<StopCommandMessage>(message));

				updateSubscriberList(message);
				break;
			}
			case STOP_ALL_COMMANDS_MESSAGE: {
				cout << "Processing stopAllCommandsMessage" << endl;

				stopAllCommands();

				updateSubscriberList(message);
				break;
			}
			case CHANGE_UPDATE_RATE_MESSAGE: {
				cout << "Processing changeUpdateRateMessage" << endl;

				changeUpdateRate(static_pointer_cast<ChangeUpdateRateMessage>(message));

				updateSubscriberList(message);
				break;
			}
			case STOP_SYSTEM_MESSAGE: {
				cout << "Processing stopSystemMessage" << endl;

				running = false;
				network->stopConnection();
				break;
			}
			default: {
				break;
			}
		}
	}
}

void CommandExecutor::updateSubscriberList(shared_ptr<Message> message) {
	//Check if host is already listed.
	//Otherwise establish TCP connection and add to list
	auto key = subscriberList.find (message->getIP());
	if( key == subscriberList.end() ) {
		auto clientFd = network->connectToClient(message->getIP(), message->getPort());

		if(clientFd > 0) {
			subscriberList[message->getIP()] = clientFd;
		}
	}
}

void CommandExecutor::updateSubscriber(void) {
	while(running) {
		//Wait for interval of updateRate
		std::this_thread::sleep_for(std::chrono::milliseconds(updateRate));

		//Get exclusive access to list of processes
		unique_lock<mutex> lock(commandMutex);

		if(!running) {
			continue;
		}

		if(runningProcess.size() == 0) {
			start = std::chrono::system_clock::now();
		}

		auto now = std::chrono::system_clock::now();
		auto elapsed = now - start;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(elapsed);

		//Obtain information about each process
		for(auto process : runningProcess) {
			if((process.second)->obtainProcessInformation()) {
				auto port 		= network->port;
				auto pid 		= stoi((process.second)->processID);
				auto cpuUsage 	= stoi((process.second)->cpuUsage);
				auto memUsage 	= stoi((process.second)->memUsage);
				auto timestamp 	= ms.count();
				cout << "timestamp: " << timestamp << endl;
				auto command 	= (process.second)->command;

				auto message = make_shared<ProcessInformationMessage>(port, pid, cpuUsage, memUsage, timestamp, command);

				//Send information to each subscriber
				for(auto subscriber : subscriberList) {
					auto clientFd = subscriber.second;

					//If subscriber is not reachable, remove it from list
					if(!network->sendMessage(clientFd, message)) {
						subscriberList.erase(subscriber.first);
					}
				}
			}
		}
	}
}

void CommandExecutor::changeUpdateRate(shared_ptr<ChangeUpdateRateMessage> message) {
	auto newUpdateRate = message->getUpdateRate();
	if(newUpdateRate >= MIN_UPDATE_RATE) {
		if(newUpdateRate <= MAX_UPDATE_RATE) {
			updateRate = newUpdateRate;
		}
	}
}

void CommandExecutor::runCommand(shared_ptr<RunCommandMessage> message) {
	//Get exclusive access to list of processes
	unique_lock<mutex> lock(commandMutex);

	 //If process is already running, stop it
	 auto alreadyRunning = runningProcess.find (message->getCommandID());
	 if(alreadyRunning != runningProcess.end()){
		 (alreadyRunning->second)->endProcess();
		 runningProcess.erase(message->getCommandID());
	 }

	 //Run process if maxRunningCommands is not reached yet.
	 if(runningProcess.size() < maxRunningCommands) {
		 runningProcess[message->getCommandID()] = make_shared<Process>(message->getCommandID(),message->getCommand());
	 }
}

void CommandExecutor::stopCommand(shared_ptr<StopCommandMessage> message) {
	//Get exclusive access to list of processes
	unique_lock<mutex> lock(commandMutex);

	//Find particular process, end it and remove it from list
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
	//Get exclusive access to list of processes
	unique_lock<mutex> lock(commandMutex);

	//End all processes
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
