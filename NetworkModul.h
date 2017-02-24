/*
 * NetworkModul.h
 *
 *  Created on: Jan 23, 2017
 *      Author: derlars
 */

#ifndef NETWORKMODUL_H_
#define NETWORKMODUL_H_

#include "message/Message.h"
#include "message/RunCommandMessage.h"

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

#include <memory>

using namespace std;


class NetworkModul {
public:
	bool settedUp;
	bool running;

	NetworkModul();
	NetworkModul(int port);

	virtual ~NetworkModul();

	/**
	 * Create a connection.
	 * Set up the TCP connection.
	 * It should be called by a thread dedicated for receiving messages.
	 */
	void startConnection();

	/**
	 * End the connection.
	 */
	void stopConnection();

	/**
	 * Process a received message.
	 * This function is blocking and should called by a separate thread (or more)
	 * which is responsible for processing incoming messages.
	 */
	shared_ptr<Message> processMessage();

	/**
	 * Establish a connection to client
	 */
	int connectToClient(string ip, int port);

	/**
	 * Close already established connection to client
	 */
	void disconnectFromClient(int sockFd);

	/**
	 * Send a message to a server
	 *
	 * @param ip IP address of the server
	 * @param port port of the server
	 * @param message message to send
	 **/
	bool sendMessage(int sockFd, shared_ptr<Message> message);

private:
	//queue<Message> messageQueue;
	queue<shared_ptr<Message>> messageQueue;
	mutex messageMutex;
	condition_variable messageAvailable;

	int port;

	int tcpConnection;
	int sockfd;

	/**
	 * Wait for incoming connections. This function is non-terminating during runtime.
	 * This function can be terminated by calling 'stopConnection()' from another thread.
	 * Is called by StartConnection()
	 */
	void waitForMessage();
};

#endif /* NETWORKMODUL_H_ */
