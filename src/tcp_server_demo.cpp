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

#include <mix/net/TcpServerSocket.h>


int main(int argc, char *argv[])
{
	mix::TcpServerSocket serverSocket(7000);
	serverSocket.SetReuseAddress(true);

	mix::Socket *pSocket;

	printf("Tcp Server Demo started!\n");

	while (true)
	{
		try
		{
			pSocket = serverSocket.Accept(5);
		}
		catch (mix::net_exception &e)
		{
			continue;
		}
		if (pSocket == NULL) continue;

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
