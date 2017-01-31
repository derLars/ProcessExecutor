/*
 * Process.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef PROCESS_H_
#define PROCESS_H_

//Allows the use of a C++ string
#include <string>

//Allows the use of a C++ vector
#include <vector>

using namespace std;

class Process {
public:
	bool processValid;
	pid_t processID;
	string command;

	//Provided information after calling 'obtainProcessInformation'
	string timestamp;
	string cpuUsage;
	string memUsage;

	Process();

	Process(string command);

	virtual ~Process();

	/**
	 * Create a sub-process which calls the command.
	 * Create a reading- and writing-pipe between main process and subprocess.
	 */
	bool runProcess(string command);

	/**
	 * Read incoming message bytewise as long as the are bytes to read or the end of line is reached.
	 */
	string readFromProcess();

	/**
	 * Send a message to the sub-process through the pipe.
	 */
	void sendToProcess(string message);

	/**
	 * Obtain process information provided by the command 'top'.
	 * After calling this function the CPU usage and the memory usage is stored within
	 * the variables cpuUsage and memUsage.
	 *
	 * For a continuous update of these information the function needs to be called continuously.
	 */
	bool obtainProcessInformation();

	/**
	 * Force the process to terminate by SIGTERM
	 */
	void endProcess();

	/**
	 * Check if process is still alive.
	 */
	bool isAlive();

private:
	int sendPipe;
	int receivePipe;

	/**
	 * Helper function to split strings into substrings
	 * The delimiter must consists of only one character.
	 */
	vector<string> splitString(string str, char delimiter);

	/**
	 * Helper function to create a timestamp.
	 * hh:mm:ss
	 */
	string createTimestamp();
};



#endif /* PROCESS_H_ */
