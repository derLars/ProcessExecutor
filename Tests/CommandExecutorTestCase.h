/*
 * CommandExecutorTestCase.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef COMMANDEXECUTORTESTCASE_H_
#define COMMANDEXECUTORTESTCASE_H_

//INCLUDE THE CLASS UNDER TEST HERE
#include "../CommandExecutor.h"

class CommandExecutorTestCase : public ::testing::Test {
public:
	~CommandExecutorTestCase() {}
	CommandExecutorTestCase() {}
protected:
	//INITIALIZE YOUR CLASS UNDER TEST HERE
	//CommandExecutor commandExecutor(1337,10);
	unique_ptr<CommandExecutor> commandExecutor;
};

TEST_F(CommandExecutorTestCase, CommandExecutor) {
	cout << "~~~~~~~~~~ Start test: CommandExecutorTestCase modul ~~~~~~~~~~" << endl;
	auto maxRunningCommands = 10;

	commandExecutor = make_unique<CommandExecutor>(1337,maxRunningCommands);

	NetworkModul network(1550);

	//------ Testcase 1: Run a command ------
	auto runCommandMessage = make_shared<RunCommandMessage>(1550,1000, 0,"./loadTest.sh");
	auto client = network.connectToClient("localhost",1337);
	network.sendMessage(client, runCommandMessage);
	network.disconnectFromClient(client);

	system("sleep 2");
	auto list = commandExecutor->getListOfRunningProcesses();

	auto pidFirst = 0;
	if(list.size()) {
		pidFirst = list[0];
	}

	EXPECT_NE(pidFirst, 0);
	EXPECT_EQ(list.size(), 1);

	//------ Testcase 2: Run same command again ------
	/*
	client = network.connectToClient("localhost",1337);
	network.sendMessage(client, runCommandMessage);
	network.disconnectFromClient(client);

	system("sleep 2");
	list = commandExecutor->getListOfRunningProcesses();

	auto pidSecond = 0;
	if(list.size()) {
		pidSecond = list[0];
	}

	EXPECT_NE(pidSecond, 0);
	EXPECT_EQ(list.size(), 1);
	EXPECT_NE(pidFirst, pidSecond);


	//------ Testcase 3: Stop command ------

	if(list.size()) {
		cout << "Process to end: " << pidSecond << endl;
		auto stopCommandMessage = make_shared<StopCommandMessage>(1550,pidSecond);
		client = network.connectToClient("localhost",1337);
		network.sendMessage(client, stopCommandMessage);
		network.disconnectFromClient(client);

		system("sleep 2");
	}
	list = commandExecutor->getListOfRunningProcesses();
	EXPECT_EQ(list.size(), 0);

	system("sleep 2");

	//------ Testcase 4: Run several commands ------
	vector<shared_ptr<RunCommandMessage>> runCommandMessageList;

	int commandID = 1000;
	for(auto i=0; i<maxRunningCommands; i++) {
		commandID = 1000 + i;
		client = network.connectToClient("localhost",1337);

		network.sendMessage(client, make_shared<RunCommandMessage>(1550,commandID, 0,"./loadTest.sh"));
		network.disconnectFromClient(client);
		system("sleep 1");

		list = commandExecutor->getListOfRunningProcesses();
		EXPECT_EQ(list.size(), i+1);
	}

	system("sleep 2");

	auto stopAllCommandsMessage = make_shared<StopAllCommandsMessage>(1550);

	client = network.connectToClient("localhost",1337);
	network.sendMessage(client, stopAllCommandsMessage);
	network.disconnectFromClient(client);

	system("sleep 5");

	list = commandExecutor->getListOfRunningProcesses();
	EXPECT_EQ(list.size(), 0);

	system("sleep 5");
	*/
	cout << "~~~~~~~~~~ Finish test: CommandExecutorTestCase modul ~~~~~~~~~~" << endl;
}

#endif /* COMMANDEXECUTORTESTCASE_H_ */
