/*
 * NetworkModulTest.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef NETWORKMODULTEST_H_
#define NETWORKMODULTEST_H_

//INCLUDE THE CLASS UNDER TEST HERE
#include "../NetworkModul.h"
#include <thread>
class NetworkModulTest : public ::testing::Test {
public:
	~NetworkModulTest() {}
	NetworkModulTest() {}
protected:
	//INITIALIZE YOUR CLASS UNDER TEST HERE
	NetworkModul networkModul;
};

TEST_F(NetworkModulTest, NetworkModul) {
	cout << "~~~~~~~~~~ Start test: Network modul ~~~~~~~~~~" << endl;

	//------ Testcase 1: sending and receiving ------
	int port = 1337;

	std::thread receiver(&NetworkModul::startConnection, &networkModul);

	system("sleep 1");

	auto sentMessage = make_shared<RunCommandMessage>(port,1000, 0,"This is a testmessage\n");
	//networkModul.sendMessage(static_pointer_cast<Message>(testMessage));

	auto serverFd = networkModul.connectToClient("localhost",port);

	networkModul.sendMessage(serverFd,sentMessage);

	networkModul.disconnectFromClient(serverFd);

	cout << "checkpoint" << endl;
	auto receivedMessage = networkModul.processMessage();

	//------ Testcase 2: interpret plain data ------
	ASSERT_EQ(sentMessage->getMessageID(), receivedMessage->getMessageID());
	ASSERT_EQ(sentMessage->getMessageSize(), receivedMessage->getMessageSize());
	ASSERT_EQ(sentMessage->getPayload(), receivedMessage->getPayload());


	//------ Testcase 3: cast to original message type ------
	auto castedMessage = static_pointer_cast<RunCommandMessage>(receivedMessage);

	ASSERT_EQ(sentMessage->getCommandID(), castedMessage->getCommandID());
	ASSERT_EQ(sentMessage->hasRootRight(), castedMessage->hasRootRight());
	ASSERT_EQ(sentMessage->getCommand(), castedMessage->getCommand());


	networkModul.stopConnection();
	receiver.join();


	cout << "~~~~~~~~~~ Finish test: Network modul ~~~~~~~~~~" << endl;
}

#endif /* NETWORKMODULTEST_H_ */
