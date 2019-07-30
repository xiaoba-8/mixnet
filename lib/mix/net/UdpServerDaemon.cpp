/*
 * UdpServerDaemon.cpp
 *
 *  Created on: 2018-9-1
 *      Author: xiaoba-8
 */

#include <mix/net/UdpServerSocket.h>
#include <mix/net/UdpServerDaemon.h>
#include <mix/net/net_errno.h>

#include <stdio.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>

#include "net_thread.h"

namespace mix
{

UdpServerDaemon::UdpServerDaemon(const char *ip, int port,
		i_udp_socket_handler *pHandler, bool bUseThread) :
	m_port(port), m_pSocketHandler(pHandler), m_bUseThread(bUseThread)
{
	if (ip != NULL)
		m_ip = ip;
	m_bRunning = false;
	m_toStop = false;
}

UdpServerDaemon::~UdpServerDaemon()
{
	while (m_bRunning)
	{
		Stop();
		usleep(50);
	}
}

void UdpServerDaemon::RunLoop()
{
	UdpServerSocket serverSocket(m_ip.c_str(), m_port);
	serverSocket.SetReuseAddress(true);
//	serverSocket.SetReadTimeout(5);

	m_bRunning = true;
	int nRet;
	char buffer[UDP_PACKAGE_MAX_LENGTH];
	char remoteAddr[UDP_REMOTE_IP_LENGTH];
	int remotePort;

	while (!m_toStop)
	{
		try
		{
			nRet = serverSocket.RecvFrom(buffer, 2000, remoteAddr, &remotePort);
			if (nRet <= 0)
				continue;
		}
		catch (net_exception &ex)
		{
			if (ex.GetErrNo() != NET_NUMBER + NET_SOCK_READ_TIMEOUT_NO)
				printf("Error: %s\n", ex.what());
			continue;
		}


		if (m_bUseThread)
		{
			net_thread *pThread = new net_thread(new ProcessThread(&serverSocket, buffer, nRet,
					remoteAddr, remotePort, m_pSocketHandler), false, true);
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
					ProcessThread process(&serverSocket, buffer, nRet,
							remoteAddr, remotePort, m_pSocketHandler);
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
}

void UdpServerDaemon::Stop()
{
	m_toStop = true;
}

UdpServerDaemon::ProcessThread::ProcessThread(Socket *pSocket, char *pBuffer,
		int len, char *remoteIp, int remotePort, i_udp_socket_handler *pHandler) :
	m_bCanceled(false), m_pSocket(pSocket), m_len(len),
	m_remotePort(remotePort), m_pSocketHandler(pHandler)
{
	memcpy(m_pBuffer, pBuffer, UDP_PACKAGE_MAX_LENGTH);
	memcpy(m_remoteIp, remoteIp, UDP_REMOTE_IP_LENGTH);
}

UdpServerDaemon::ProcessThread::~ProcessThread()
{
}

void UdpServerDaemon::ProcessThread::Run()
{
	m_pSocketHandler->Handler(m_pSocket, m_pBuffer, m_len, m_remoteIp,
			m_remotePort);
}

void UdpServerDaemon::ProcessThread::Stop()
{

}

void UdpServerDaemon::ProcessThread::Cancel()
{
	m_bCanceled = true;
}

bool UdpServerDaemon::ProcessThread::IsCanceled()
{
	return m_bCanceled;
}

}
