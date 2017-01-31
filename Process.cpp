/*
 * Process.cpp
 *
 *  Created on: Jan 21, 2017
 *      Author: Lars Schwensen
 */

#include "Process.h"

//Following includes are necessary for the
//call and maintenance of the external processes
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

//provides a timestamp
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

Process::Process() {
	processID = -1;
	processValid = false;
	sendPipe = -1;
	receivePipe = -1;

	cpuUsage = "-1";
	memUsage = "-1";
}

Process::Process(string command) : Process() {
	runProcess(command);
}

Process::~Process() {
	endProcess();
}

bool Process::runProcess(string command) {
	processValid = true;

	int inputPipe[2];
	int outputPipe[2];

	//Open inputPipe
	if (pipe(inputPipe) == -1) {
		processValid = false;
	}

	//Open outputPipe
	if (pipe(outputPipe) == -1) {
		close(inputPipe[0]);
		close(inputPipe[1]);

		processValid = false;
	}

	auto pid = fork();

	if (!processValid || pid < 0) {
		//Something went wrong during the duplication of the process
		close(inputPipe[0]);
		close(inputPipe[1]);
		close(outputPipe[0]);
		close(outputPipe[1]);

		processValid = false;
	} else if (pid == 0) {
		//Entered by the child process

		//The child process does only need one end of the pipes
		//so the other end can be closed on child-side
		close(inputPipe[1]);
		close(outputPipe[0]);

		//map the pipe
		dup2(inputPipe[0], 0);
		dup2(outputPipe[1], 1);
		dup2(::open("/dev/null", O_RDONLY), 2);

		/// Close all other descriptors for the safety sake.
		for (int i = 3; i < 4096; ++i)
		   ::close(i);

		//setsid creates a new process group in which the calling process is the process leader.
		setsid();

		//Execute the command
		//The current process will be replaced by the process which will be created by the execution.
		execl("/bin/sh", "sh", "-c", command.c_str(), NULL);

		_exit(1);
	}

	//The parent process does only need one end of the pipes
	close(inputPipe[0]);
	close(outputPipe[1]);

	sendPipe 	= inputPipe[1];
	receivePipe = outputPipe[0];

	this->command = command;
	processID = pid;

	return processValid;
}

string Process::readFromProcess() {
	string outputString = "";
	if(processValid) {
		char character = ' ';
		int bytes = 0;
		do {
			bytes = read(receivePipe, &character, sizeof(character));
			outputString += character;
			if(character == '\n') {
				break;
			}
		}while(bytes > 0);
	}

	return outputString;
}

void Process::sendToProcess(string message) {
	write(sendPipe, message.c_str(), message.size());
}

bool Process::obtainProcessInformation() {
	if(isAlive()) {
		//Define the command 'top -n 1 -b -p PID | grep PID'
		// -n 1			-> Only one iteration
		// -b			-> Batch mode (readable output)
		// -p PID		-> Process of interest.
		// | grep PID	-> filter header
		auto topCommand = "top -n 1 -b -p " + to_string(processID) + "  | grep " + to_string(processID);

		//prepare variables
		auto numOfBytes = 128;
		char buffer[numOfBytes];
		string processInformation = "";

		//open pipe and read information
		auto pipe = popen(topCommand.c_str(), "r");
		if (!pipe) {
			return false;
		}
		try {
			while (!feof(pipe)) {
				if (fgets(buffer, numOfBytes, pipe) != NULL)
					processInformation += buffer;
			}
		} catch (...) {
			pclose(pipe);
			throw;
		}
		pclose(pipe);

		//provide the obtained information
		auto vec = splitString(processInformation, ' ');
		if(vec.size() > 7) {
			cpuUsage = vec[6];
			memUsage = vec[7];
			timestamp = createTimestamp();
			return true;
		}
	}
	return false;
}

void Process::endProcess() {
	kill(processID,SIGTERM);

	auto status = -1;
	while(wait(&status) > 0) { }
	processValid = false;
}

bool Process::isAlive() {
	//sending a 0 to the process provides the information if
	//a process is still alive and if a user is allowed to send signals to the process.
	//If both is true, the function call returns 0.
	if(kill(processID,0) == 0) {
		processValid = true;
		return true;
	}
	processValid = false;
	return false;
}

vector<string> Process::splitString(string str, char delimiter) {
	vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
	vector<string> result;
	string currentWord = "";

	//iterate each character and compare with given delimiter
	for(auto& chr : cstr) {
		if(chr == delimiter && currentWord.size()) {
			result.push_back(currentWord);
			currentWord = "";
		}else if(chr != delimiter){
			currentWord += chr;
		}
	}

	//put last word into vector
	if(currentWord.size()){
		result.push_back(currentWord);
	}

	return result;
}

string Process::createTimestamp() {
    auto now = chrono::system_clock::now();
    auto in_time_t = chrono::system_clock::to_time_t(now);

    stringstream ss;
    ss << put_time(localtime(&in_time_t), "%X");
    return ss.str();
}