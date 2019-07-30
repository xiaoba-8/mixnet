/*
 * tcp_server_demo.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: xiaoba-8
 */

#include <stdio.h>
#include <mix/net/TcpServerDaemon.h>
#include <mix/net/i_tcp_socket_handler.h>

class TcpServerDemoHandler : public mix::i_tcp_socket_handler
{
public:
	virtual void Handler(mix::Socket *pSocket)
	{
		char buffer[500];

		int len = 0;
		int size = 0;
		do
		{
			len = pSocket->Recv(buffer, 500);
			size += len;
		} while (len > 0);

		printf("Received [%d] bytes\n", size);

		memset(buffer, 0, 500);
		sprintf(buffer, "OK, Received [%d] bytes", size);
		std::string response = buffer;

		pSocket->WriteUTF(response);

		pSocket->Close();
	}

	virtual std::string GetTaskId() { return "demo"; }
};

int main(int argc, char *argv[])
{
	TcpServerDemoHandler tcpHander;

	mix::TcpServerDaemon serverDaemon(NULL, 7000, &tcpHander, true);

	printf("Tcp Server Daemon Demo started!\n");

	serverDaemon.RunLoop();

	return 0;
}
