/*
 * TcpServerDaemon.cpp
 *
 *  Created on: 2018-7-13
 *      Author: xiaoba-8
 */

#include <mix/net/TcpServerSocket.h>
#include <mix/net/TcpServerDaemon.h>

#include "net_thread.h"

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

namespace mix
{

TcpServerDaemon::TcpServerDaemon(const char *ip, int port, i_tcp_socket_handler *pHandler, bool bUseThread)
: m_port(port), m_pSocketHandler(pHandler), m_bUseThread(bUseThread)
{
	if (ip != NULL)
		m_ip = ip;

	m_bRunning = false;
	m_toStop = false;
}

TcpServerDaemon::~TcpServerDaemon()
{
	if (m_bRunning)
	{
		Stop();
	}
}

void TcpServerDaemon::RunLoop()
{
	TcpServerSocket serverSocket(m_ip.c_str(), m_port);
	serverSocket.SetReuseAddress(true);

	Socket *pSocket;
	m_bRunning = true;

	while (!m_toStop)
	{
		try
		{
			pSocket = serverSocket.Accept(5);
		}
		catch (net_exception &e)
		{
			continue;
		}
		if (pSocket == NULL) continue;

		if (m_bUseThread)
		{
			net_thread *pThread = new net_thread(new ProcessThread(pSocket, m_pSocketHandler), false, true);
			pThread->Start();
		}
		else
		{
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
					ProcessThread process(pSocket, m_pSocketHandler);
					process.Run();

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
	}

	m_bRunning = false;
	return;
}

void TcpServerDaemon::Stop()
{
	m_toStop = false;
//	sleep(2);
}

TcpServerDaemon::ProcessThread::ProcessThread(Socket *pSocket, i_tcp_socket_handler *pHandler)
: m_pSocket(pSocket), m_pSocketHandler(pHandler), m_bCanceled(false)
{

}

TcpServerDaemon::ProcessThread::~ProcessThread()
{
	if (!m_pSocket->IsClosed())
		m_pSocket->Close();

	delete m_pSocket;
}

void TcpServerDaemon::ProcessThread::Run()
{
	m_pSocketHandler->Handler(m_pSocket);
}

void TcpServerDaemon::ProcessThread::Stop()
{

}

void TcpServerDaemon::ProcessThread::Cancel()
{
	m_bCanceled = true;
}

bool TcpServerDaemon::ProcessThread::IsCanceled()
{
	return m_bCanceled;
}

}
