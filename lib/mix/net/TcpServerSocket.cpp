/*
 * TcpServerSocket.cpp
 *
 *  Created on: 2018-7-12
 *      Author: xiaoba-8
 */

#include <errno.h>
#include <mix/net/TcpServerSocket.h>
#include <mix/net/net_errno.h>

namespace mix
{

TcpServerSocket::TcpServerSocket() : Socket(SOCKET_TCP)
{
	m_reuseOn = false;
}

TcpServerSocket::TcpServerSocket(int port) : Socket(SOCKET_TCP)
{
	m_reuseOn = false;
	Bind(NULL, port);
}

TcpServerSocket::TcpServerSocket(const char *host, int port) : Socket(SOCKET_TCP)
{
	m_reuseOn = false;
	Bind(host, port);
}


TcpServerSocket::~TcpServerSocket()
{

}

void TcpServerSocket::Bind(const char *host, int port, int backlog)
{
	Socket::Bind(host, port);

	if (listen(m_socket, backlog) == -1)
		throw mix::net_exception(NET_NUMBER + NET_SOCK_LISTEN_NO, "Socket Listen Error [%s]", strerror(errno));
}

Socket *TcpServerSocket::Accept(int timeout)
{
	struct sockaddr_in remoteAddr;
	socklen_t size = sizeof(struct sockaddr_in);

	SOCKET clientSocket = 0;
	if (timeout > 0)
	{
		SetNonblocking(true);

		timeval tm;
		tm.tv_usec = 0;
		tm.tv_sec = timeout;

		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		int error = 0;
		int nret = 0;
		socklen_t len;
		if ((nret = select(m_socket+1, &set, NULL, NULL, &tm)) > 0)
		{
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
			if (error != 0)
			{
				SetNonblocking(false);
			}
			else
			{
				clientSocket = accept(m_socket, (struct sockaddr *)&remoteAddr, &size);

				if (clientSocket < 0)
					throw mix::net_exception(NET_NUMBER + NET_SOCK_ACCEPT_NO, NET_SOCK_ACCEPT_MSG, strerror(errno));

				SetNonblocking(false);
			}
		}
		else
		{
			if (nret < 0)
				throw mix::net_exception(NET_NUMBER + NET_SOCK_SELECT_NO, NET_SOCK_SELECT_MSG, strerror(errno));
			SetNonblocking(false);
		}
	}
	else
	{
		clientSocket = accept(m_socket, (struct sockaddr *)&remoteAddr, &size);

		if (clientSocket < 0)
			throw mix::net_exception(NET_NUMBER + NET_SOCK_ACCEPT_NO, NET_SOCK_ACCEPT_MSG, strerror(errno));

	}

	if (clientSocket <= 0)
		return NULL;

	return new Socket(clientSocket, m_host.c_str(), m_port, inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));
}

void TcpServerSocket::Close()
{
	Socket::Close();
}

void TcpServerSocket::SetReuseAddress(bool on)
{
	m_reuseOn = on;
	int opt = on? 1 : 0;

	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(int)) < 0)
		throw mix::net_exception(NET_NUMBER + NET_SOCK_SETOPT_NO, "Socket setsockopt Error [%s]", strerror(errno));
}

bool TcpServerSocket::GetReuseAddress()
{
	return m_reuseOn;
}

}
