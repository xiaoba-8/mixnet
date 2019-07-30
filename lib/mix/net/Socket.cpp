/*
 * Socket.cpp
 *
 *  Created on: 2018-7-7
 *      Author: xiaoba-8
 */

#include <iostream>
#include <string.h>
#include <errno.h>
#include <termios.h>
#include <mix/net/net_utils.h>
#include <mix/net/Socket.h>
#include <mix/net/net_errno.h>

#include "net_mutex.h"

namespace mix
{

#define UDP_BUFFER_SIZE	2000

static net_mutex s_mutex;

Socket::Socket(SocketType socketType) : m_host(""), m_port(0), m_reference(false)
{
	CreateSocket(socketType);
}

Socket::Socket(SOCKET socket) : m_socket(socket), m_reference(true)
{

}

Socket::Socket(SocketType socketType, bool init) : m_host(""), m_port(0), m_reference(false)
{
	if (init)
		CreateSocket(socketType);
}

Socket::Socket(const char *host, int port, int timeout, SocketType socketType) : m_host(""), m_port(0)
{
	CreateSocket(socketType);
	Connect(host, port, timeout);
}

Socket::Socket(const char *host, int port, const char *localhost, int localport, SocketType socketType) : m_host(""), m_port(0)
{
	CreateSocket(socketType);
	Bind(localhost, localport);
	Connect(host, port, 0);
}

Socket::Socket(SOCKET socket, const char *host, int port, const char *remoteAddr, int remotePort)
{
	m_socket = socket;
	if (host != NULL)
		m_host = host;
	m_port = port;
	m_remoteAddr = remoteAddr;
	m_remotePort = remotePort;
	m_closed = false;
}

Socket::~Socket()
{
	if (!m_closed && !m_reference)
		Close();
}

void Socket::CreateSocket(SocketType socketType)
{
	m_readTimeout = 0;
	m_writeTimeout = 0;

	switch (socketType)
	{
	case SOCKET_TCP:
		m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		break;
	case SOCKET_UDP:
		m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		break;
	}
	if (m_socket == -1)
		throw net_exception(NET_NUMBER + NET_SOCK_CREATE_NO, "Create Socket Error [%s]", strerror(errno));

	m_closed = false;
}

void Socket::Bind(const char *host, int port)
{
	struct sockaddr_in addr;

	if (host != NULL)
		m_host = host;
	m_port = port;

	int on = 1;

	setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	if (bind(m_socket, (struct sockaddr *)&GetSockAddr(host, port, addr), sizeof(struct sockaddr)) == -1)
		throw net_exception(NET_NUMBER + NET_SOCK_BIND_NO, "Socket Bind Error [%s]", strerror(errno));
}

void Socket::Connect(struct sockaddr *addr, int timeout)
{
	if (((struct sockaddr_in *)(addr))->sin_port <= 0)
		throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Error [%s]", "No remote port");

	if (timeout > 0)
	{
		SetNonblocking(true);
		if (connect(m_socket, addr, sizeof(struct sockaddr)) == -1)
		{
			if (errno != EINPROGRESS)
				throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Error [%s]", strerror(errno));

			timeval tm;
			tm.tv_usec = 0;
			tm.tv_sec = timeout;

			fd_set set;
			FD_ZERO(&set);
			FD_SET(m_socket, &set);

			int error = 0;
			socklen_t len = sizeof(socklen_t);

			int sel = select(m_socket+1, NULL, &set, NULL, &tm);
			if (sel > 0)
			{
				//printf("getsockopt %d %d\n", error, len);
				if (getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len) == 0)
				{
					//printf("getsockopt %d %d\n", error, len);
					if (error != 0)
					{
						SetNonblocking(false);
						throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Error: [%d] [%s]", error, strerror(error));
					}
					else
					{
						SetNonblocking(false);
					}
				}
				else
				{
					throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Error [%s]", strerror(errno));
				}
			}
			else if (sel == 0)
			{
				throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Timeout");
			}
			else
			{
				throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Error [%s]", strerror(errno));
			}
		}
	}
	else
	{
		if (connect(m_socket, addr, sizeof(struct sockaddr)) == -1)
			throw net_exception(NET_NUMBER + NET_SOCK_CONNECT_NO, "Socket Connect Error [%s]", strerror(errno));
	}
}

void Socket::Connect(const char *host, int port, int timeout)
{
	struct sockaddr_in addr;
	GetSockAddr(host, port, addr);

	Connect((struct sockaddr *)&addr, timeout);

	if (host != NULL)
		m_remoteAddr = host;
	m_remotePort = port;
}

void Socket::SetNonblocking(bool bNb)
{
#ifdef _WIN32
	unsigned long l = bNb ? 1 : 0;
	int n = ioctlsocket(m_socket, FIONBIO, &l);
	if (n != 0)
	{
		throw net_exception(BASE_SOCK_IOCTL_NO, "ioctlsocket(FIONBIO) Error!");
	}
#else
	if (bNb)
	{
		if (fcntl(m_socket, F_SETFL, O_NONBLOCK) == -1)
		{
			throw net_exception(NET_NUMBER + NET_SOCK_FCNTL_NO, "fcntl(F_SETFL, O_NONBLOCK) Error! %s", strerror(errno));
		}
	}
	else
	{
		if (fcntl(m_socket, F_SETFL, 0) == -1)
		{
			throw net_exception(NET_NUMBER + NET_SOCK_FCNTL_NO, "fcntl(F_SETFL, O) Error! %s", strerror(errno));
		}
	}
#endif
}

void Socket::Write(std::string buf)
{
	WriteUntil(buf.c_str(), buf.length());
}

void Socket::WriteUntil(const char *pBuffer, size_t size)
{
	size_t leftSize = size;

	while (leftSize > 0)
	{
		leftSize -= Send((char *)pBuffer+(size-leftSize), leftSize);
	}
}

void Socket::WriteUTF(std::string buf)
{
	WriteInt(buf.length());
	Write(buf);
}

void Socket::WriteChar(char x)
{
	WriteNumber(x);
}

void Socket::WriteShort(short x)
{
	x = htons(x);
	WriteNumber(x);
}

void Socket::WriteInt(int x)
{
	x = htonl(x);
	WriteNumber(x);
}

void Socket::WriteLong(long x)
{
	WriteNumber(htonl(x));
}

void Socket::WriteUInt64(uint64_t x)
{
	WriteNumber(htonll(x));
}

void Socket::WriteDouble(double x)
{
	WriteNumber<double>(hton_double(x));
}

void Socket::WriteFloat(float x)
{
	WriteNumber<double>(x);
}

std::string Socket::Read(size_t size)
{
	std::string result;
	char *pBuffer = new char[size];

	ReadUntil(pBuffer, size);
	result.append(pBuffer, size);

	delete []pBuffer;

	return result;
}

void Socket::ReadUntil(char *pBuffer, size_t size)
{
	int len = 0;
	int leftSize = size;

	while (leftSize > 0)
	{
		len = Recv(pBuffer+(size-leftSize), leftSize);
		if (len < 0)
			throw net_exception(NET_NUMBER + NET_SOCK_READ_NO, "Socket Read Error [%s]", strerror(errno));
		else if (len == 0)
			throw net_exception(NET_NUMBER + NET_SOCK_READ_NO, "Socket Read Error [Connection is Closed]");
		leftSize -= len;
	}
}

std::string Socket::ReadUTF(int maxLen)
{
	int len = ReadInt();
	if (maxLen > 0 && len > maxLen)
		throw net_exception(NET_NUMBER + NET_SOCK_READ_NO, "Socket Read Error [%s]", "the UTF length exceed the max length!");
	return Read(len);
}

char Socket::ReadChar()
{
	return ReadNumber<char>();
}

short Socket::ReadShort()
{
	return ntohs(ReadNumber<short>());
}

int Socket::ReadInt()
{
	return ntohl(ReadNumber<int>());
}

long Socket::ReadLong()
{
	return ntohl(ReadNumber<long>());
}

uint64_t Socket::ReadUInt64()
{
	return ntohll(ReadNumber<uint64_t>());
}

double Socket::ReadDouble()
{
	return ntoh_double(ReadNumber<double>());
}

float Socket::ReadFloat()
{
	return ReadNumber<float>();
}

int Socket::Recv(char *pBuffer, size_t size)
{
	if (m_readTimeout > 0)
	{
		SetNonblocking(true);

		timeval tm;
		tm.tv_usec = 0;
		tm.tv_sec = m_readTimeout;

		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		int error = 0;
		int len = sizeof(int);
		if (select(m_socket+1, &set, NULL, NULL, &tm) > 0)
		{
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
			if (error != 0)
			{
				SetNonblocking(false);
				throw net_exception(NET_NUMBER + NET_SOCK_READ_NO, "Socket Read Error [%d][%s]", error, strerror(error));
			}
			else
			{
				int nRet = recv(m_socket, pBuffer, size, 0);
				SetNonblocking(false);
				return nRet;
			}
		}
		else
		{
			SetNonblocking(false);
			throw net_exception(NET_NUMBER + NET_SOCK_READ_TIMEOUT_NO, "Socket Read Timeout!");
		}
	}
	else
	{
		return recv(m_socket, pBuffer, size, 0);
	}
}

int Socket::Send(const char *pBuffer, size_t size)
{
	if (m_writeTimeout > 0)
	{
		SetNonblocking(true);

		timeval tm;
		tm.tv_usec = 0;
		tm.tv_sec = m_writeTimeout;

		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		int error = 0;
		int len = sizeof(int);
		if (select(m_socket+1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
			if (error != 0)
			{
				SetNonblocking(false);
				throw net_exception(NET_NUMBER + NET_SOCK_SEND_NO, "Socket Send Error [%d] [%s]", error, strerror(error));
			}
			else
			{
				int nRet = send(m_socket, pBuffer, size, MSG_NOSIGNAL);
				SetNonblocking(false);
				return nRet;
			}
		}
		else
		{
			SetNonblocking(false);
			throw net_exception(NET_NUMBER + NET_SOCK_WRITE_TIMEOUT_NO, "Socket Write Timeout!");
		}
	}
	else
	{
		return send(m_socket, pBuffer, size, MSG_NOSIGNAL);
	}
}

void Socket::SetReadTimeout(int timeout)
{
	m_readTimeout = timeout;
}

void Socket::SetWriteTimeout(int timeout)
{
	m_writeTimeout = timeout;
}

void Socket::ShutdownInput()
{
	shutdown(m_socket, SHUT_RD);
}

void Socket::ShutdownOutput()
{
	shutdown(m_socket, SHUT_WR);
}

void Socket::Shutdown()
{
	shutdown(m_socket, SHUT_RDWR);
	//close(m_socket);
	m_closed = true;
}

void Socket::Flush()
{
	usleep(500000);
}

void Socket::Close()
{
	//shutdown(m_socket, SHUT_RDWR);
	close(m_socket);
	m_closed = true;
}

bool Socket::IsClosed()
{
	return m_closed;
}

struct sockaddr_in &Socket::GetSockAddr(const char *host, int port, struct sockaddr_in &addr)
{
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);
	bzero(&(addr.sin_zero), 8);

	if (host == NULL || host[0] == 0)
		addr.sin_addr.s_addr = INADDR_ANY;
	else
	{
		s_mutex.lock();
		struct hostent *pEnt;
		if ((pEnt = gethostbyname(host)) == NULL)
			throw net_exception(NET_NUMBER + NET_GETHOSTBYNAME_NO, "gethostbyname(%s) Error!", host);
		addr.sin_addr = *((struct in_addr *)pEnt->h_addr);
		s_mutex.unlock();
	}
	return addr;
}

std::string Socket::GetStringAddr(struct sockaddr_in addr)
{
	char buffer[100];
	GetStringAddr(addr, buffer);
	return buffer;
}

void Socket::GetStringAddr(struct sockaddr_in addr, char *strAddr)
{
	/**
	int a1 = (addr.sin_addr.s_addr>>24) & 0x0ff;
	int a2 = (addr.sin_addr.s_addr>>16) & 0x0ff;
	int a3 = (addr.sin_addr.s_addr>>8) & 0x0ff;
	int a4 = (addr.sin_addr.s_addr) & 0x0ff;
	sprintf(strAddr, "%d.%d.%d.%d", a4, a3, a2, a1);
	**/

	strcpy(strAddr, inet_ntoa(addr.sin_addr));
}

std::string Socket::GetLocalIp()
{
	return m_host;
}

int Socket::GetLocalPort()
{
	return m_port;
}

struct sockaddr_in Socket::GetRemoteAddr()
{
	struct sockaddr_in addr;
	socklen_t len;

	if (getpeername(m_socket, (struct sockaddr *)&addr, &len) == 0)
	{

	}
	else
	{
		addr.sin_port = 0;
	}

	return addr;
}

std::string Socket::GetRemoteIp()
{
	if (m_remoteAddr.size() == 0)
	{
		struct sockaddr_in addr = GetRemoteAddr();
		m_remoteAddr = inet_ntoa(addr.sin_addr);
	}
	return m_remoteAddr;
}

int Socket::GetRemotePort()
{
	if (m_remoteAddr.size() == 0)
	{
		struct sockaddr_in addr = GetRemoteAddr();
		m_remotePort = ntohs(addr.sin_port);
	}
	return m_remotePort;
}

SOCKET Socket::GetSocketFd()
{
	return m_socket;
}

int Socket::SendTo(std::string msg, const struct sockaddr *addr, socklen_t addrlen)
{
	return SendTo(msg.c_str(), msg.size(), addr, addrlen);
}

int Socket::SendTo(const char *msg, size_t size, const struct sockaddr *addr, socklen_t addrlen)
{
	if (m_writeTimeout > 0)
	{
		SetNonblocking(true);

		timeval tm;
		tm.tv_usec = 0;
		tm.tv_sec = m_writeTimeout;

		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		int error = 0;
		int len = sizeof(int);
		if (select(m_socket+1, NULL, &set, NULL, &tm) > 0)
		{
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
			if (error != 0)
			{
				SetNonblocking(false);
				throw mix::net_exception(NET_NUMBER + NET_SOCK_SEND_NO, "Socket Send Error!");
			}
			else
			{
				int nRet = sendto(m_socket, msg, size, 0, addr, addrlen);
				SetNonblocking(false);
				return nRet;
			}
		}
		else
		{
			SetNonblocking(false);
			throw mix::net_exception(NET_NUMBER + NET_SOCK_WRITE_TIMEOUT_NO, "Socket Write Timeout!");
		}
	}
	else
	{
		return sendto(m_socket, msg, size, 0, addr, addrlen);
	}
}

int Socket::SendTo(std::string msg, const char *remoteaddr, int remoteport)
{
	struct sockaddr_in addr;
	GetSockAddr(remoteaddr, remoteport, addr);

	return SendTo(msg, (const struct sockaddr *)&addr, sizeof(addr));
}

int Socket::SendTo(const char *msg, size_t len, const char *remoteaddr, int remoteport)
{
	struct sockaddr_in addr;
	GetSockAddr(remoteaddr, remoteport, addr);

	return SendTo(msg, len, (const struct sockaddr *)&addr, sizeof(addr));
}

int Socket::RecvFrom(std::string &msg, struct sockaddr *addr, socklen_t *addrlen)
{
	char buffer[UDP_BUFFER_SIZE];

	int nRet = RecvFrom(buffer, UDP_BUFFER_SIZE, addr, addrlen);

	if (nRet >= 0)
		msg.assign(buffer, nRet);
	return nRet;
}

int Socket::RecvFrom(char *buffer, size_t size, struct sockaddr *addr, socklen_t *addrlen)
{
	if (m_readTimeout > 0)
	{
		SetNonblocking(true);

		timeval tm;
		tm.tv_usec = 0;
		tm.tv_sec = m_readTimeout;

		fd_set set;
		FD_ZERO(&set);
		FD_SET(m_socket, &set);

		int error = 0;
		int len = sizeof(int);
		if (select(m_socket+1, &set, NULL, NULL, &tm) > 0)
		{
			getsockopt(m_socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
			if (error != 0)
			{
				SetNonblocking(false);
				throw mix::net_exception(NET_NUMBER + NET_SOCK_READ_NO, "Socket Read Error!");
			}
			else
			{
				int nRet;
				nRet = recvfrom(m_socket, buffer, size, 0, addr, addrlen);
				SetNonblocking(false);
				return nRet;
			}
		}
		else
		{
			SetNonblocking(false);
			throw mix::net_exception(NET_NUMBER + NET_SOCK_READ_TIMEOUT_NO, "Socket Read Timeout!");
		}
	}
	else
	{
		return recvfrom(m_socket, buffer, size, 0, addr, addrlen);
	}
}

int Socket::RecvFrom(std::string &msg, char *remoteaddr, int *remoteport)
{
	char buffer[UDP_BUFFER_SIZE];

	int nRet = RecvFrom(buffer, UDP_BUFFER_SIZE, remoteaddr, remoteport);

	if (nRet >= 0)
		msg.assign(buffer, nRet);
	return nRet;
}

int Socket::RecvFrom(char *msg, size_t size, char *remoteaddr, int *remoteport)
{
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	int nRet = RecvFrom(msg, size, (struct sockaddr *)&addr, &addrlen);

	if (nRet >= 0)
	{
		*remoteport = ntohs(addr.sin_port);
		GetStringAddr(addr, remoteaddr);
	}

	return nRet;
}

}
