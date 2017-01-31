/*
 * CommandExecutor.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include <unordered_map>
#include <memory>
#include <thread>
#include "Process.h"
#include <atomic>
#include <mutex>

using namespace std;

class CommandExecutor {
public:
	CommandExecutor();
	virtual ~CommandExecutor();

	bool runCommand(string command);

	void stopCommand(int pid);
	void stopAllCommands(void);

	void subscribeForInformation(int pid,string ip, int port, int frequency);
private:
	atomic<bool> subscribed;
	mutex commandsMapMutex;
	unsigned int maxCommands;
	unordered_map<int,shared_ptr<Process>> runningCommands;
};

#endif /* COMMANDEXECUTOR_H_ */
