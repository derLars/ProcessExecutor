/*
 * ProcessTest.h
 *
 *  Created on: Jan 22, 2017
 *      Author: Lars Schwensen
 */

#ifndef PROCESSTEST_H_
#define PROCESSTEST_H_

#include "../Process.h"

class ProcessTest : public ::testing::Test {
public:
	~ProcessTest() {}
	ProcessTest() {}
protected:
	Process process;
};

TEST_F(ProcessTest, Process) {
	cout << "~~~~~~~~~~ Start test: Process modul ~~~~~~~~~~" << endl;

	//------ Testcase 1: Initial condition ------
	EXPECT_FALSE(process.processValid) << "No process was executed!";

	//------ Testcase 2: Run a continuous process and stop it ------
	process.runProcess(950,"./top");
	EXPECT_TRUE(process.processValid) << "One continuous process was executed!";

	process.isAlive();
	EXPECT_TRUE(process.processValid) << "Process should be still alive!";

	process.endProcess();
	process.isAlive();

	EXPECT_FALSE(process.processValid) << "Process was ended and should not be alive!";


	//------ Testcase 3: Read the output of a process ------

	process.runProcess(1000, "./readTest.sh");
	//string pid = process.readFromProcess(6);
	//cout << "received PID: " << pid << endl;

	string received = process.readFromProcess();
	cout << "received Msg: " << received << endl;
	EXPECT_EQ(received,"This text is the output of the process\n");

	process.endProcess();


	//------ Testcase 4: write test ------

	process.runProcess(1050, "./writeTest.sh");

	process.sendToProcess("This text is the input to the process\n");
	//process.sendToProcess("This text is the input to the process\n");
	//received = process.readFromProcess();
	//cout << "received 1 Msg: " << received << endl;

	//process.sendToProcess("This text is the input to the process\n");
	received = process.readFromProcess();
	cout << "received 2 Msg: " << received << endl;

	EXPECT_EQ(received,"This text is the input to the process\n");
	//EXPECT_FALSE(process.isAlive());
	process.endProcess();


	//------ Testcase 5: load test ------
	process.runProcess(1100, "./loadTest.sh");
	EXPECT_EQ(process.cpuUsage,"-1");
	EXPECT_EQ(process.memUsage,"-1");

	process.obtainProcessInformation();
	cout << "Obtained process information:" << endl;
	cout << "at time: " << process.timestamp << endl;
	cout << "cpu: " << process.cpuUsage << endl;
	cout << "mem: " << process.memUsage << endl;
	EXPECT_NE(process.cpuUsage,"-1");
	EXPECT_NE(process.memUsage,"-1");

	process.endProcess();

	cout << "~~~~~~~~~~ Finish test: Process modul ~~~~~~~~~~" << endl;
}



#endif /* PROCESSTEST_H_ */
