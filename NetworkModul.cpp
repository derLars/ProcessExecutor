/*
 * NetworkModul.cpp
 *
 *  Created on: Jan 23, 2017
 *      Author: derlars
 */

#include "NetworkModul.h"

//Necessary for the 'cout' command
#include <iostream>

//Necessary for the 'read' and 'write' commands
#include <unistd.h>

//Necessary for the 'gethostbyname' call
#include <netdb.h>

//Necessary for the 'bzero' call
#include <string.h>

NetworkModul::NetworkModul(): NetworkModul(1337) {
	//Arbitrary defined standard port of 1337 in case no port is mentioned
}

NetworkModul::NetworkModul(int port) : running(false), port(port), tcpConnection(-1), sockfd(-1) {

}

NetworkModul::~NetworkModul() {
	stopConnection();
}

bool NetworkModul::startConnection() {
	struct sockaddr_in serv_addr;

	//create a socket
	//AF_INET: Use of IPv4 internet protocol
	//SOCK_STREAM: Two-way connection as byte stream
	//Last parameter 0: no particular protocol specified
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		cout << "ERROR opening socket" << endl;
		return false;
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
		return false;
	}

	running = true;
	return true;
}

void NetworkModul::waitForMessage() {
	struct sockaddr_in cli_addr;

	socklen_t clilen;

	int newsockfd;

	//start listening to socket
	listen(sockfd,5);
	clilen = sizeof(cli_addr);

	char buffer[256];
	int  n;

	while(running) {
		cout << "waiting for message to receive!" << endl;

		//wait for incoming connections
		newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
		string ip = inet_ntoa(cli_addr.sin_addr);

		//termine condition
		if(!running) {
			cout << "ending connection!" << endl;
			close(sockfd);
			return;
		}
		cout << "received connection" << endl;

		//something went wrong
		if (newsockfd < 0) {
			exit(1);
		}

		//connection is established,start communicating
		bzero(buffer,256);
		n = read( newsockfd,buffer,255 );

		if (n < 0) {
		  perror("ERROR reading from socket");
		  exit(1);
		}

		string message = buffer;
		message += ip;
		//write message into message queue and send a notification
		//to waiting threads
		messageMutex.lock();
			messageQueue.push(buffer);
			printf("Enqueued: %s\n",buffer);
		messageMutex.unlock();
		messageAvailable.notify_one();
	}

	close(sockfd);
}

string NetworkModul::processMessage() {
	unique_lock<mutex> lock(messageMutex);

	cout << "waiting for message to process!" << endl;

	//waiting for message incoming in message queue
	while(messageQueue.empty() && running) {
		messageAvailable.wait(lock);

		//terminate condition
		if(!running) {
			return "";
		}
	}

	auto message = messageQueue.front();
	messageQueue.pop();

	return message;
}

bool NetworkModul::sendMessage(string ip, int port, string message) {
	bool status = true;
	int sockfd;
	int n;
	struct sockaddr_in serv_addr;
	struct hostent *server;

	//create a socket
	//AF_INET: Use of IPv4 internet protocol
	//SOCK_STREAM: Two-way connection as byte stream
	//Last parameter 0: no particular protocol specified
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0) {
		cout << "could not create a socket!" << endl;
		status = false;
	}

	//define the server
	server = gethostbyname(ip.c_str());
	if(sockfd < 0) {
		cout << "could not identify a server!" << endl;
		status = false;
	}

	//set the given number of bytes to 0
	//AF_INET: Use of IPv4 internet protocol
	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;

	//htons(port): Define the port of server
	bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
	serv_addr.sin_port = htons(port);

	//connect to server
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		cout << "connection not able!" << endl;
		status = false;
	}

	//send message to server
	n = write(sockfd,message.c_str(),message.size());
	if(n < 0) {
		cout << "could not write to server!" << endl;
		status = false;
	}

	close(sockfd);

	return status;
}

void NetworkModul::stopConnection() {
	running = false;

	//send a message to own server connection to wake up the waiting connection thread
	sendMessage("localhost", port, "");

	//send notification to all threads
	messageAvailable.notify_all();
}
