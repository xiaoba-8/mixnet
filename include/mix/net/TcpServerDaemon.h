/*
 * TcpServerDaemon.h
 *
 *  Created on: 2018-7-13
 *      Author: xiaoba-8
 */

#ifndef SERVERDAEMON_H_
#define SERVERDAEMON_H_

#include <string>
#include "i_tcp_socket_handler.h"

#include "Task.h"


namespace mix
{

class TcpServerDaemon
{
public:
	TcpServerDaemon(const char *ip, int port, i_tcp_socket_handler *pHandler, bool bUseThread);

	virtual ~TcpServerDaemon();

	virtual void RunLoop();
	virtual void Stop();

protected:
	class ProcessThread;

private:
	std::string m_ip;
	int m_port;
	bool m_bRunning;
	bool m_toStop;
	i_tcp_socket_handler *m_pSocketHandler;
	bool m_bUseThread;
};

class TcpServerDaemon::ProcessThread : public Task
{
public:
	ProcessThread(Socket *pSocket, i_tcp_socket_handler *pHandler);
	virtual ~ProcessThread();

	virtual void Run();
	virtual void Stop();

	virtual bool IsDeleteOnFinish() { return true; }

	virtual void Cancel();
	virtual bool IsCanceled();
private:
	Socket *m_pSocket;
	i_tcp_socket_handler *m_pSocketHandler;
	bool m_bCanceled;
};
}

#endif /* SERVERDAEMON_H_ */
