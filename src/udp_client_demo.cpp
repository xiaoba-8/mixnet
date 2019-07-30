/*
 * tcp_client_demo.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: xiaoba-8
 */

#include <stdio.h>
#include <mix/net/Socket.h>

#define UDP_REMOTE_IP_LENGTH	100

int main(int argc, char *argv)
{
	mix::Socket socket(mix::SOCKET_UDP);

	char buffer[1024];
	memset(buffer, 'a', 1024);

	int nRet;
	char remoteAddr[UDP_REMOTE_IP_LENGTH];
	int remotePort;

	try
	{
		socket.SendTo(buffer, 1024, "127.0.0.1", 7000);

		memset(buffer, 0, 1024);
		nRet = socket.RecvFrom(buffer, 2000, remoteAddr, &remotePort);

		socket.Close();

		printf("Receive Response From Server: \n   %s\n", buffer);
	}
	catch (mix::net_exception &ex)
	{
		printf("Error Message: %s\n", ex.what());
	}

	return 0;
}
