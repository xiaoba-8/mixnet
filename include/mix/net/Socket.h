/*
 * Socket.h
 *
 *  Created on: 2018-7-7
 *      Author: xiaoba-8
 */

#ifndef SOCKET_H_
#define SOCKET_H_

#include "socket_common.h"

#include <string.h>
#include <string>

namespace mix
{

enum SocketType
{
	SOCKET_TCP,
	SOCKET_UDP
};

class Socket
{
public:
	Socket(SocketType socketType);
	Socket(SOCKET socket);
	Socket(SocketType socketType, bool init);

	Socket(const char *host, int port, int timeout = 0, SocketType socketType = SOCKET_TCP);
	Socket(const char *host, int port, const char *localhost, int localport, SocketType socketType = SOCKET_TCP);
	Socket(SOCKET socket, const char *host, int port, const char *remoteAddr, int remotePort);

	virtual ~Socket();

	virtual void Bind(const char *host, int port);
	virtual void Connect(const char *host, int port, int timeout = 0);

	virtual void Connect(struct sockaddr *addr, int timeout = 0);

	virtual void Write(std::string buf);

	virtual void WriteUntil(const char *pBuffer, size_t size);

	virtual void WriteUTF(std::string buf);

	template<typename T>
	void WriteNumber(T x)
	{
		char *pBuffer = new char[sizeof(T)];

		memcpy((void *)pBuffer, (const void *)&x, sizeof(T));
		WriteUntil(pBuffer, sizeof(T));

		delete[] pBuffer;
	}

	virtual void WriteChar(char x);
	virtual void WriteShort(short x);
	virtual void WriteInt(int x);
	virtual void WriteLong(long x);
	virtual void WriteUInt64(uint64_t x);
	virtual void WriteDouble(double x);
	virtual void WriteFloat(float x);

	virtual std::string Read(size_t size);

	virtual void ReadUntil(char *pBuffer, size_t size);

	virtual std::string ReadUTF(int maxLen=200);

	template<typename T>
	T ReadNumber()
	{
		char *pBuffer = new char[sizeof(T)];
		T x;

		ReadUntil(pBuffer, sizeof(T));
		memcpy(&x, (const void *)pBuffer, sizeof(T));

		delete[] pBuffer;
		return x;
	}

	virtual char ReadChar();
	virtual short ReadShort();
	virtual int ReadInt();
	virtual long ReadLong();
	virtual uint64_t ReadUInt64();
	virtual double ReadDouble();
	virtual float ReadFloat();

	virtual int Recv(char *pBuffer, size_t size);
	virtual int Send(const char *pBuffer, size_t size);

	virtual void SetReadTimeout(int timeout);
	virtual void SetWriteTimeout(int timeout);

	virtual void ShutdownInput();
	virtual void ShutdownOutput();
	virtual void Shutdown();

	virtual void SetNonblocking(bool bNb);

	virtual void Close();
	virtual bool IsClosed();

	virtual std::string GetLocalIp();
	virtual int GetLocalPort();

	virtual std::string GetRemoteIp();
	virtual int GetRemotePort();

	virtual struct sockaddr_in GetRemoteAddr();

	virtual void Flush();

	virtual SOCKET GetSocketFd();

	static struct sockaddr_in &GetSockAddr(const char *host, int port, struct sockaddr_in &addr);
	static std::string GetStringAddr(struct sockaddr_in addr);
	static void GetStringAddr(struct sockaddr_in addr, char *strAddr);

	virtual int SendTo(std::string msg, const struct sockaddr *addr, socklen_t addrlen);
	virtual int SendTo(const char *msg, size_t len, const struct sockaddr *addr, socklen_t addrlen);

	virtual int SendTo(std::string msg, const char *remoteaddr, int remoteport);
	virtual int SendTo(const char *msg, size_t len, const char *remoteaddr, int remoteport);

	virtual int RecvFrom(std::string &msg, struct sockaddr *addr, socklen_t *addrlen);
	virtual int RecvFrom(char *msg, size_t len, struct sockaddr *addr, socklen_t *addrlen);

	virtual int RecvFrom(std::string &msg, char *remoteaddr, int *remoteport);
	virtual int RecvFrom(char *msg, size_t len, char *remoteaddr, int *remoteport);

protected:

	void CreateSocket(SocketType socketType);

	SOCKET m_socket;

	int m_readTimeout;
	int m_writeTimeout;

	std::string m_host;
	int m_port;

	std::string m_remoteAddr;
	int m_remotePort;
	bool m_closed;
	bool m_reference;
};

}

#endif /* SOCKET_H_ */
