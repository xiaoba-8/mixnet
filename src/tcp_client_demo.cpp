/*
 * tcp_client_demo.cpp
 *
 *  Created on: Jul 24, 2019
 *      Author: xiaoba-8
 */

#include <stdio.h>
#include <mix/net/Socket.h>

int main(int argc, char *argv)
{
	mix::Socket socket(mix::SOCKET_TCP);

	char buffer[1024];
	memset(buffer, 'a', 1024);

	try
	{
		socket.Connect("127.0.0.1", 7000, 5);
		socket.SetWriteTimeout(5);
		socket.SetReadTimeout(5);

		for (int i = 0; i < 2; i++)
		{
			socket.WriteUntil(buffer, 1024);
		}

		socket.ShutdownOutput();

		std::string response = socket.ReadUTF();
		socket.ShutdownInput();

		socket.Close();

		printf("Receive Response From Server: \n   %s\n", response.c_str());
	}
	catch (mix::net_exception &ex)
	{
		printf("Error Message: %s\n", ex.what());
	}

	return 0;
}
