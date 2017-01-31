/*
 * CommandExecutor.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#include "CommandExecutor.h"
#include <iostream>

CommandExecutor::CommandExecutor() {
	subscribed = false;
	maxCommands = 10;
	runningCommands = unordered_map<int, shared_ptr<Process>>();
}

CommandExecutor::~CommandExecutor() {
	// TODO Auto-generated destructor stub
}

bool CommandExecutor::runCommand(string command) {
	if(runningCommands.size() < maxCommands) {
		auto process = make_shared<Process>(command);
		if (process->processValid) {
			runningCommands.emplace(process->processID, process);
			std::cout << process->readFromProcess() << std::endl;

			process->obtainProcessInformation();

			std::cout << process->cpuUsage << std::endl;
			std::cout << process->memUsage << std::endl;
			process->endProcess();
			return true;
		}
	}
	return false;
}

void CommandExecutor::stopCommand(int pid) {
	commandsMapMutex.lock();
	for (auto& process: runningCommands) {
		if(process.first == pid) {
			process.second->endProcess();
			runningCommands.erase(pid);
			break;
		}
	}
	commandsMapMutex.unlock();
}

void CommandExecutor::stopAllCommands(void) {
	commandsMapMutex.lock();
	subscribed = false;
	for (auto& process: runningCommands) {
		process.second->endProcess();
		runningCommands.erase(process.first);
	}
	commandsMapMutex.unlock();
}

void CommandExecutor::subscribeForInformation(int pid, string ip, int port, int frequency) {
	if(!subscribed) {
		std::thread subscriber([&](){

				shared_ptr<Process> process;

				commandsMapMutex.lock();
				for (auto& pr: runningCommands) {
					if(pr.first == pid) {
						process = pr.second;
						break;
					}
				}
				commandsMapMutex.unlock();

				if(process == nullptr) {
					return;
				}

				while(subscribed) {
					if(!process->obtainProcessInformation()) {
						subscribed = false;
					} else {
						//TODO: send data to subscriber
					}
				}
			}
		);
	}

}
