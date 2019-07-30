/*
 * tcp_server_demo.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: xiaoba-8
 */

#include <mix/net/UdpServerDaemon.h>
#include <mix/net/i_udp_socket_handler.h>
#include <mix/net/UdpServerSocket.h>

#include <stdio.h>
class UdpSocketHandler : public mix::i_udp_socket_handler
{
public:
	virtual void Handler(mix::Socket *pSocket, char *pBuffer, int len,
			char *remoteIp, int remotePort)
	{
		char buffer[500];

		printf("Received Pkg length = %d from %s:%d\n", len, remoteIp, remotePort);

		memset(buffer, 0, 500);
		sprintf(buffer, "OK, Received [%d] bytes", len);
		std::string response = buffer;

		pSocket->SendTo(response, (const char *)remoteIp, remotePort);
	}

	virtual std::string GetTaskId() { return "demo"; }
};

int main(int argc, char *argv[])
{
	UdpSocketHandler udpHander;

	mix::UdpServerDaemon serverDaemon(NULL, 7000, &udpHander, true);

	printf("Udp Server Daemon Demo started!\n");

	serverDaemon.RunLoop();

	return 0;
}
