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
	CommandExecutor commandExecutor;
};

/*TEST_F(CommandExecutorTestCase, CommandExecutor) {
	ASSERT_TRUE(commandExecutor.runCommand("./testScript.sh"));
	//ASSERT_TRUE(classUnderTest.addNum(4,5) == 9);
	//ASSERT_TRUE(classUnderTest.subNum(4,5) == 9);
}*/

#endif /* COMMANDEXECUTORTESTCASE_H_ */
