/*
 * UdpServerSocket.cpp
 *
 *  Created on: 2018-9-1
 *      Author: xiaoba-8
 */


extern "C"
{
#include <stddef.h>
#include <errno.h>
}

#include <mix/net/UdpServerSocket.h>
#include <mix/net/net_errno.h>

namespace mix
{

UdpServerSocket::UdpServerSocket() : Socket(SOCKET_UDP)
{
	m_reuseOn = false;
}

UdpServerSocket::UdpServerSocket(int port) : Socket(SOCKET_UDP)
{
	m_reuseOn = false;
	Bind(NULL, port);
}

UdpServerSocket::UdpServerSocket(const char *host, int port) : Socket(SOCKET_UDP)
{
	m_reuseOn = false;
	Bind(host, port);
}


UdpServerSocket::~UdpServerSocket()
{

}

void UdpServerSocket::Bind(const char *host, int port, int backlog)
{
	Socket::Bind(host, port);
}


void UdpServerSocket::SetReuseAddress(bool on)
{
	m_reuseOn = on;
	int opt = on? 1 : 0;

	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (void *)&opt, sizeof(int)) < 0)
		throw mix::net_exception(NET_NUMBER + NET_SOCK_SETOPT_NO, "Socket setsockopt Error [%s]", strerror(errno));
}

bool UdpServerSocket::GetReuseAddress()
{
	return m_reuseOn;
}

}
