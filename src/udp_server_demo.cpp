/*
 * tcp_server_demo.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: xiaoba-8
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <mix/net/UdpServerSocket.h>


#define UDP_PACKAGE_MAX_LENGTH	2000
#define UDP_REMOTE_IP_LENGTH	100

int main(int argc, char *argv[])
{
	mix::UdpServerSocket serverSocket(7000);
	serverSocket.SetReuseAddress(true);
//	serverSocket.SetReadTimeout(5);

	int nRet;
	char buffer[UDP_PACKAGE_MAX_LENGTH];
	char remoteAddr[UDP_REMOTE_IP_LENGTH];
	int remotePort;

	printf("Udp Server Demo started!\n");

	while (true)
	{
		try
		{
			nRet = serverSocket.RecvFrom(buffer, 2000, remoteAddr, &remotePort);
			if (nRet <= 0)
				continue;
		}
		catch (mix::net_exception &ex)
		{
			printf("Error: %s\n", ex.what());
			continue;
		}

		pid_t pid = fork();

		if (pid == 0)
		{
			pid = fork();
			if (pid > 0)
			{
				exit(0);
			}
			else if (pid == 0)
			{
				char buffer[500];
				int len = nRet;

				printf("Received Pkg length = %d from %s:%d\n", len, remoteAddr, remotePort);

				memset(buffer, 0, 500);
				sprintf(buffer, "OK, Received [%d] bytes", len);
				std::string response = buffer;

				serverSocket.SendTo(response, (const char *)remoteAddr, remotePort);

				exit(0);
			}
			else
			{
				perror("fork error");
			}
		}
		else if (pid > 0)
		{
			if (waitpid(pid, NULL, 0) != pid)
			{
				perror("waitpid error");
			}
		}
		else
		{
			perror("fork error");
		}
	}

	return 0;
}
