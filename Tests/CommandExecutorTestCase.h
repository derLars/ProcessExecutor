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

	commandExecutor = make_unique<CommandExecutor>(1337,10);

	NetworkModul network(1550);
	auto runCommandMessage = make_shared<RunCommandMessage>("localhost",1550,1000, 0,"./loadTest.sh");

	auto client = network.connectToClient("localhost",1337);
	network.sendMessage(client, runCommandMessage);
	network.disconnectFromClient(client);

	system("sleep 10");


	ASSERT_TRUE(commandExecutor->running);






	cout << "~~~~~~~~~~ Finish test: CommandExecutorTestCase modul ~~~~~~~~~~" << endl;
}

#endif /* COMMANDEXECUTORTESTCASE_H_ */
