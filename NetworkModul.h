/*
 * NetworkModul.h
 *
 *  Created on: Jan 23, 2017
 *      Author: derlars
 */

#ifndef NETWORKMODUL_H_
#define NETWORKMODUL_H_

//#include <memory>
//#include <stdio.h>
//#include <stdlib.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netinet/ip.h>
#include <arpa/inet.h>

//Allows the use of a C++ queue
#include <queue>

//For threadsafe behavior
#include <mutex>
#include <condition_variable>

using namespace std;


class NetworkModul {
public:
	NetworkModul();
	NetworkModul(int port);

	virtual ~NetworkModul();

	/**
	 * Create a connection.
	 * This function is not waiting for incoming messages.
	 * After creating the connection the function 'waitForMessage()' should be called.
	 */
	bool startConnection();

	/**
	 * End the connection.
	 */
	void stopConnection();

	/**
	 * Wait for incoming connections. This function is non-terminating during runtime.
	 * It should be called by a thread dedicated for receiving messages.
	 * This function can be terminated by calling 'stopConnection()' from another thread.
	 *
	 */
	void waitForMessage();

	/**
	 * Process a received message.
	 * This function is blocking and should called by a separate thread (or more)
	 * which is responsible for processing incoming messages.
	 */
	string processMessage();

	/**
	 * Send a message to a server
	 * ip: [string] requires the IP address of the server
	 * port: [int] requires the port of the server
	 */
	bool sendMessage(string ip, int port, string message);

private:
	queue<string> messageQueue;

	mutex messageMutex;
	condition_variable messageAvailable;

	bool running;
	int port;

	int tcpConnection;
	int sockfd;
};

#endif /* NETWORKMODUL_H_ */
