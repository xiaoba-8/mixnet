/*
 * UdpServerDaemon.h
 *
 *  Created on: 2018-9-1
 *      Author: xiaoba-8
 */

#ifndef UDPSERVERDAEMON_H_
#define UDPSERVERDAEMON_H_

#include "UdpServerDaemon.h"
#include <string>
#include "i_udp_socket_handler.h"

#include "Task.h"

namespace mix
{

#define UDP_PACKAGE_MAX_LENGTH	2000
#define UDP_REMOTE_IP_LENGTH	100

class UdpServerDaemon
{
public:
	UdpServerDaemon(const char *ip, int port, i_udp_socket_handler *pHandler, bool bUseThread);

	virtual ~UdpServerDaemon();

	virtual void RunLoop();
	virtual void Stop();

protected:
	class ProcessThread;
private:
	std::string m_ip;
	int m_port;
	bool m_bRunning;
	bool m_toStop;
	i_udp_socket_handler *m_pSocketHandler;
	bool m_bUseThread;
};

class UdpServerDaemon::ProcessThread : public Task
{
public:
	ProcessThread(Socket *pSocket, char *pBuffer, int len, char *remoteIp, int remotePort, i_udp_socket_handler *pHandler);
	virtual ~ProcessThread();

	virtual void Run();
	virtual void Stop();

	virtual bool IsDeleteOnFinish() { return true; }

	virtual void Cancel();
	virtual bool IsCanceled();
private:
	bool m_bCanceled;
	Socket *m_pSocket;
	char m_pBuffer[UDP_PACKAGE_MAX_LENGTH];
	int m_len;
	char m_remoteIp[UDP_REMOTE_IP_LENGTH];
	int m_remotePort;
	i_udp_socket_handler *m_pSocketHandler;
};

}

#endif /* UDPSERVERDAEMON_H_ */
