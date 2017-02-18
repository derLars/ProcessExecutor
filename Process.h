/*
 * Process.h
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include <iostream>

//Allows the use of a C++ string
#include <string>

//Allows the use of a C++ vector
#include <vector>

//#include <boost/variant.hpp>

using namespace std;

class Process {
public:
	bool processValid;
	string processID;
	string originalID;
	string command;
	int commandID;

	//Provided information after calling 'obtainProcessInformation'
	string timestamp;
	string cpuUsage;
	string memUsage;

	Process();

	Process(int commandID, string command);

	virtual ~Process();

	/**
	 * Create a sub-process which calls the command.
	 * Create a reading- and writing-pipe between main process and subprocess.
	 *
	 * @param commandID a unique id for each command
	 * @param command the command to be executed
	 */
	bool runProcess(int commandID, string command);

	/**
	 * Read incoming message bytewise as long as the are bytes to read or the end of line is reached.
	 */
	string readFromProcess();

	/**
	 * Send a message to the sub-process through the pipe.
	 *
	 * @param message the message/input to the process
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
	int sendFd;
	int receiveFd;

	int receivePipe;

	string pipeToProcess;
	string pipeFromProcess;

	/**
	 * Helper function to split strings into substrings
	 * The delimiter must consists of only one character.
	 *
	 * @param str the string which needs to be cut
	 * @param delimiter the delimiter
	 */
	vector<string> splitString(string str, char delimiter);

	/**
	 * Helper function to create a timestamp.
	 * hh:mm:ss
	 */
	string createTimestamp();

	/**
	 * Helperfunction to obtain the PID of the process.
	 * This function should only be called directly after running an external
	 * process. The first information the process sends is the PID
	 *
	 * @param pipe pipe file descriptor
	 */
	string obtainPID(int pipe);

	/**
	 * Reading function, wrapped by 'readFromProcess' and 'obtainPID'
	 *
	 * @param pipe pipe file discriptor
	 */
	string readFromPipe(int pipe);
};



#endif /* PROCESS_H_ */
