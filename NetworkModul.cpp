/*
 * NetworkModul.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: derlars
 */

#include "NetworkModul.h"
#include "message/StopSystemMessage.h"

//Necessary for the 'cout' command
#include <iostream>

//Necessary for the 'read' and 'write' commands
#include <unistd.h>

//Necessary for the 'gethostbyname' call
#include <netdb.h>

//Necessary for the 'bzero' call
#include <string.h>

#define HEADER_SIZE 12

NetworkModul::NetworkModul(): NetworkModul(1337) {
	//Arbitrary defined standard port of 1337 in case no port is mentioned
}

NetworkModul::NetworkModul(int port) : settedUp(false), running(false), port(port), tcpConnection(-1), sockfd(-1) {

}

NetworkModul::~NetworkModul() {
	stopConnection();
}

void NetworkModul::startConnection() {
	running = true;
	struct sockaddr_in serv_addr;

	//create a socket
	//AF_INET: Use of IPv4 internet protocol
	//SOCK_STREAM: Two-way connection as byte stream
	//Last parameter 0: no particular protocol specified
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "ERROR opening socket" << endl;
		running = false;
	}

	//set the given number of bytes to 0
	//AF_INET: Use of IPv4 internet protocol
	//INADDR_ANY: No restrictions for incoming connections
	//htons(port): Define the port
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	//bind the socket to port
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		cout << "ERROR binding" << endl;
		running = false;
	}

	settedUp = true;
	if(running) {
		waitForMessage();
	}
}

void NetworkModul::waitForMessage() {
	struct sockaddr_in cli_addr;

	socklen_t clilen;

	int newsockfd;

	//start listening to socket
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	char header[HEADER_SIZE];
	int  n;

	while(running) {
		cout << "waiting for message to receive!" << endl;

		//wait for incoming connections
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		string ip = inet_ntoa(cli_addr.sin_addr);

		cout << "received connection" << endl;

		//something went wrong
		if (newsockfd < 0) {
			continue;
		}

		//connection is established,start communicating
		bzero(header,HEADER_SIZE);
		n = read( newsockfd,header,HEADER_SIZE );

		if (n < 0) {
		  perror("ERROR reading HEADER from socket");
		  continue;
		}

		auto message = make_shared<Message>(header, ip);

		auto payloadSize = message->getMessageSize();
		if(payloadSize) {
			char payload[payloadSize];

			bzero(payload,payloadSize);
			n = read(newsockfd,payload,payloadSize);

			if (n < 0) {
				perror("ERROR reading PAYLOAD from socket");
				continue;
			}

			if(payload[n] != 'n') {
				char modPayload[n];

				strncpy(modPayload, payload, n);
				message->setPayload(modPayload);
			}else{
				message->setPayload(payload);
			}
		}

		//termine condition
		if(!running) {
			cout << "ending connection!" << endl;
			return;
		}

		//write message into message queue and send a notification
		//to waiting threads
		messageMutex.lock();
			messageQueue.push(message);
		messageMutex.unlock();
		messageAvailable.notify_one();
	}

	close(sockfd);
}

shared_ptr<Message> NetworkModul::processMessage() {
	unique_lock<mutex> lock(messageMutex);

	cout << "waiting for message to process!" << endl;

	//waiting for message incoming in message queue
	while(messageQueue.empty() && running) {
		messageAvailable.wait(lock);

		//terminate condition
		if(!running) {
			return make_shared<Message>();
		}
	}

	auto message = messageQueue.front();
	messageQueue.pop();

	return message;
}

int NetworkModul::connectToClient(string ip, int port) {
	int sockFd = -1;

	struct sockaddr_in cli_addr;
	struct hostent *client;

	//create a socket
	//AF_INET: Use of IPv4 internet protocol
	//SOCK_STREAM: Two-way connection as byte stream
	//Last parameter 0: no particular protocol specified
	sockFd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockFd < 0) {
		cout << "could not create a socket!" << endl;
	}

	//define the client
	client = gethostbyname(ip.c_str());
	if(client < 0) {
		cout << "could not identify a client!" << endl;
		sockFd = -1;
	}

	//set the given number of bytes to 0
	//AF_INET: Use of IPv4 internet protocol
	bzero((char *) &cli_addr, sizeof(cli_addr));
	cli_addr.sin_family = AF_INET;

	//htons(port): Define the port of client
	bcopy((char *)client->h_addr, (char *)&cli_addr.sin_addr.s_addr, client->h_length);
	cli_addr.sin_port = htons(port);

	//connect to server
	if (connect(sockFd,(struct sockaddr *) &cli_addr,sizeof(cli_addr)) < 0) {
		cout << "connection not able!" << endl;
		sockFd = -1;
	}

	cout << "sockFd created: " << sockFd << endl;
	return sockFd;
}

void  NetworkModul::disconnectFromClient(int sockFd) {
	close(sockFd);
}

bool NetworkModul::sendMessage(int sockFd, shared_ptr<Message> message) {
	bool status = true;
	int n;

	//send message to server
	n = write(sockFd,message->getByteMessage().data(),HEADER_SIZE+message->getMessageSize());
	if(n < 0) {
		cout << "could not write to client!" << endl;
		status = false;
	}

	return status;
}

void NetworkModul::stopConnection() {
	if(running) {
		running = false;

		//send a message to own server connection to wake up the waiting connection thread
		//TODO: Create an particular Messagetype for this case
		auto clientFd = connectToClient("localhost",port);
		auto stopMessage = make_shared<StopSystemMessage>();

		sendMessage(clientFd,stopMessage);

		disconnectFromClient(clientFd);
		//send notification to all threads
		messageAvailable.notify_all();
	}
}
