/*
 * CommandExecutor.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include "NetworkModul.h"

#include <unordered_map>
#include <memory>
#include <thread>
#include "Process.h"
#include <atomic>
#include <mutex>

#define MSGTHREAD 2

using namespace std;

class CommandExecutor {
public:
	CommandExecutor(int port, int maxRunningCommands);
	virtual ~CommandExecutor();

	bool runCommand(int commandID, string command);

	void stopCommand(int commandID);
	void stopAllCommands(void);

	void updateSubscriber(string ip, int port);
private:
	shared_ptr<NetworkModul> network;


	thread networkThread;
	vector<thread> msgThreadpool;




	void processMessage();

	atomic<bool> subscribed;
	mutex commandMutex;
	unsigned int maxRunningCommands;
	unordered_map<int,shared_ptr<Process>> runningCommands;
	unordered_map<string, int> subscriber;

	bool running;

	vector<string> splitString(string str, string delimiter);

	int subscribeRate;
};

#endif /* COMMANDEXECUTOR_H_ */
