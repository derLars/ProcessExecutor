/*
 * MessageTest.h
 *
 *  Created on: Feb 10, 2017
 *      Author: tetrys
 */

#ifndef MESSAGETEST_H_
#define MESSAGETEST_H_

#include "../message/Message.h"

class MessageTest : public ::testing::Test {
public:
	~MessageTest() {}
	MessageTest() {}
protected:

};

TEST_F(MessageTest, RunCommandMessage) {
	string receiver = "127.0.0.1";
	int port = 2047;
	int commandID = 1337;
	int updateRate = 333;
	int rootRight = 0;
	string command = "This is_a_command -I -have to send";

	RunCommandMessage originalMessage(receiver, port,commandID, updateRate, rootRight, command);

	auto stream = originalMessage.getByteMessage();

	auto payloadSize = originalMessage.getMessageSize();
	vector<char> payload(&stream[12] ,&stream[12 + payloadSize]);

	Message rawMessage(stream.data(),receiver);
	rawMessage.setPayload(payload.data());

	auto convertedBack = static_cast<RunCommandMessage>(rawMessage);

	EXPECT_TRUE(originalMessage.getMessageSize() 	== convertedBack.getMessageSize());
	EXPECT_TRUE(originalMessage.getMessageID() 		== convertedBack.getMessageID());
	EXPECT_TRUE(originalMessage.getPort() 			== convertedBack.getPort());
	EXPECT_TRUE(originalMessage.getUpdateRate() 	== convertedBack.getUpdateRate());
	EXPECT_TRUE(originalMessage.hasRootRight() 		== convertedBack.hasRootRight());
	EXPECT_TRUE(originalMessage.getCommand() 		== convertedBack.getCommand());
	ASSERT_EQ(originalMessage.getIP(), convertedBack.getIP());
}



#endif /* MESSAGETEST_H_ */
