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

	networkModul.startConnection();

	bool processed = false;
	std::thread receiver(&NetworkModul::waitForMessage, &networkModul);

	if(!processed) {
		string messageToSend = "This is a testmessage";

		processed = true;

		networkModul.sendMessage("localhost", port, messageToSend);
		string receivedMessage = networkModul.processMessage();

		ASSERT_EQ(messageToSend, receivedMessage);
		networkModul.stopConnection();
	}

	receiver.join();


	cout << "~~~~~~~~~~ Finish test: Network modul ~~~~~~~~~~" << endl;
}

#endif /* NETWORKMODULTEST_H_ */
