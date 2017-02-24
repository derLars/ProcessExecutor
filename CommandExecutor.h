/*
 * CommandExecutor.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include "NetworkModul.h"
#include "message/RunCommandMessage.h"
#include "message/StopCommandMessage.h"
#include "message/StopAllCommandsMessage.h"
#include "message/ProcessInformationMessage.h"

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
	bool running;

	CommandExecutor(int port, int maxRunningCommands);
	virtual ~CommandExecutor();

	void runCommand(shared_ptr<RunCommandMessage> message);

	void stopCommand(shared_ptr<StopCommandMessage> message);
	void stopAllCommands(void);

	void updateSubscriber(shared_ptr<Message> message);

	void sendMessage(int port, string message);

	vector<int> getListOfRunningProcesses(void);
private:
	shared_ptr<NetworkModul> network;


	thread networkThread;
	vector<thread> msgThreadpool;

	void processMessage();

	atomic<bool> subscribed;
	mutex commandMutex;

	unsigned int maxRunningCommands;

	unordered_map<int,shared_ptr<Process>> runningProcess;
	unordered_map<string, int> subscriber;

	//vector<string> splitString(string str, string delimiter);

	int subscribeRate;
};

#endif /* COMMANDEXECUTOR_H_ */
