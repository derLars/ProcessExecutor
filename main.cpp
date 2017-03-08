/*
 * main.cpp
 *
 *  Created on: Mar 4, 2017
 *      Author: derlars
 */

#include "CommandExecutor.h"

int main(int argc, char** argv) {
	int port = 5010;
	int maxNumOfCommands = 10;
	int numThreads = 2;

	if(argc > 1) {
		for(int i=1; i<argc; i++) {
			if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "-help" || std::string(argv[i]) == "--h" || std::string(argv[i]) == "--help") {
				std::cout << "Please enter the following parameter:" << std::endl;
				std::cout << "-p [port number]" << std::endl;
				std::cout << "-n [max number of processes]" << std::endl;
				std::cout << "-t [number of working threads]" << std::endl << std::endl;
				std::cout << "example: ./" << argv[0] << " -p 5010 -n 10 -t 3" << std::endl;

				return -1;
			} else if(std::string(argv[i]) == "-p" && i+1 < argc) {
				port = stoi(argv[i+1]);
			} else if(std::string(argv[i]) == "-n" && i+1 < argc) {
				maxNumOfCommands = stoi(argv[i+1]);
			} else if(std::string(argv[i]) == "-t" && i+1 < argc) {
				numThreads = stoi(argv[i+1]);
			}
		}
	}
	if(port < 1024 || port > 65535) {
		std::cout << "Please enter a valid port between 1024 and 65535!" << std::endl;
		return -1;
	}
	if(maxNumOfCommands < 1 || maxNumOfCommands > 100) {
		std::cout << "Please enter a valid number of maximum processes between 1 and 100!" << std::endl;
		return -1;
	}
	if(numThreads < 1 || numThreads > 15) {
		std::cout << "Please enter a valid number of working threads between 1 and 15!" << std::endl;
		return -1;
	}

	cout << "System is running with " << numThreads << " threads and listens to port " << port << endl;
	CommandExecutor commandExecutor(port, maxNumOfCommands, numThreads);

	return 1;
}



