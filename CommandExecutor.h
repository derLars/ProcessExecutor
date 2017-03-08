/*
 * CommandExecutor.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef COMMANDEXECUTOR_H_
#define COMMANDEXECUTOR_H_

#include "NetworkModul.h"
#include "Process.h"

#include "message/RunCommandMessage.h"
#include "message/StopCommandMessage.h"
#include "message/StopAllCommandsMessage.h"
#include "message/ProcessInformationMessage.h"
#include "message/ChangeUpdateRateMessage.h"

#include <unordered_map>
#include <memory>
#include <thread>
#include <mutex>
//#include <chrono>
//#include <atomic>

#define MSGTHREAD 2

#define DEFAULT_UPDATE_RATE 2000
#define MIN_UPDATE_RATE 100
#define MAX_UPDATE_RATE 15000

using namespace std;

class CommandExecutor {
public:
	bool running;

	CommandExecutor(int port, int maxRunningCommands, int numThreads);
	virtual ~CommandExecutor();

	/**
	 * Only for testing reasons
	 */
	vector<int> getListOfRunningProcesses(void);
private:
	shared_ptr<NetworkModul> network;

	vector<thread> msgThreadpool;
	thread networkThread;
	thread updateThread;

	unordered_map<int,shared_ptr<Process>> runningProcess;
	unordered_map<string, int> subscriberList;

	mutex commandMutex;

	unsigned int maxRunningCommands;

	//interval in which the hosts will receive updates about the processes in ms
	unsigned int updateRate;

	std::chrono::system_clock::time_point start;

	/**
	 * Continuously running function which processes incomming messages,
	 * depending on the type of message
	 */
	void processMessage();

	/**
	 * Execute a particular command given in the message
	 */
	void runCommand(shared_ptr<RunCommandMessage> message);

	/**
	 * Stop a particular command given in the message
	 */
	void stopCommand(shared_ptr<StopCommandMessage> message);

	/**
	 * Stop all commands executed by the Daemon
	 */
	void stopAllCommands(void);

	/**
	 * Update the list of subscriber.
	 * In case a message arrived from a host which is not known yet,
	 * it will automatically become a subscriber
	 * In case the connection to a subscriber is lost,
	 * it will be removed from the list of subscribers
	 */
	void updateSubscriberList(shared_ptr<Message> message);

	/**
	 * The list of subscriber will be updated with the information
	 * about currently running processes
	 */
	void updateSubscriber(void);

	/**
	 * In case the requested update rate is within the allowed range,
	 * the update rate will be adapted
	 */
	void changeUpdateRate(shared_ptr<ChangeUpdateRateMessage> message);
};

#endif /* COMMANDEXECUTOR_H_ */
