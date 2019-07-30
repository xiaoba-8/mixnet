/*
 * net_thread.cpp
 *
 *  Created on: 2019-4-13
 *      Author: xiaoba-8
 */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#endif

#include <iostream>

#include "net_thread.h"
#include <mix/net/net_errno.h>
#include <mix/net/net_exception.h>

namespace mix
{

net_thread::net_thread(Task *pTask, bool detach, bool bDeleteOnFinish)
:m_running(false)
,m_bDeleteOnFinish(bDeleteOnFinish)
,m_bDestructor(false)
,m_joined(false)
,m_detach(detach)
,m_pTask(pTask)
{
#ifdef _WIN32

#else

	pthread_attr_init(&m_attr);

	if (detach)
		pthread_attr_setdetachstate(&m_attr,PTHREAD_CREATE_DETACHED);
	else
		pthread_attr_setdetachstate(&m_attr,PTHREAD_CREATE_JOINABLE);

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

	memcpy(&m_mutex, &mutex, sizeof(pthread_mutex_t));
	memcpy(&m_cond, &cond, sizeof(pthread_cond_t));
#endif
}


net_thread::~net_thread()
{
	m_bDestructor = true;
	if (m_running)
	{
		SetRunning(false);
	}
#ifdef _WIN32
	if (m_threadId)
		::CloseHandle(m_threadId);
#else
	pthread_attr_destroy(&m_attr);
	Join();
#endif
}


threadfunc_t STDPREFIX net_thread::StartThread(threadparam_t zz)
{
	net_thread *p = (net_thread *)zz;

#ifndef _WIN32
	int oldstate;
	pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &oldstate);
	int oldtype;
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, &oldtype);
#endif


	if (p -> m_running)
	{
		p -> Run();
	}
	else
	{
		printf("thread over\n");
	}
	p -> SetRunning(false); // if return
	if (p -> IsDeleteOnFinish() && !p -> IsDestructor())
	{
		//std::cout << "Destroy Thread Class :" << p->GetRunableName() << std::endl;
		delete p;
	}
#ifdef _WIN32
	_endthreadex(0);
#else
	pthread_exit(0);
#endif
	return (threadfunc_t)NULL;
}

bool net_thread::Start()
{
	m_joined = false;
	m_running = true;
#ifdef _WIN32
	m_threadId = (HANDLE)_beginthreadex(NULL, 0, &StartThread, this, 0, &m_dwThreadId);
#else
	if (pthread_create(&m_threadId, &m_attr, StartThread, this) != 0)
	{
		if (errno == EAGAIN)
		{

		}
		printf("----------%d [%s]\n", errno, strerror(errno));
		perror("Thread: create failed");
		SetRunning(false);

		return false;
	}
	else
	{
		return true;
	}

#endif
}

void net_thread::Stop()
{
	SetRunning(false);

#ifdef _WIN32
	TerminateThread(m_threadId, 0);
#else

	if (pthread_cancel(m_threadId) == -1)
	{
		perror("Thread: cancel failed");
	}

#endif
}

void net_thread::Run()
{
	if (m_pTask != NULL)
	{
		m_pTask->Run();

		if (m_pTask->IsDeleteOnFinish())
		{
			delete m_pTask;
			m_pTask = NULL;
		}
	}
}

unsigned long int net_thread::GetId()
{
	return m_threadId;
}

bool net_thread::IsRunning()
{
 	return m_running;
}


void net_thread::SetRunning(bool x)
{
 	m_running = x;
}


bool net_thread::IsDeleteOnFinish()
{
	return m_bDeleteOnFinish;
}


void net_thread::SetDeleteOnFinish(bool x)
{
	m_bDeleteOnFinish = x;
}


bool net_thread::IsDestructor()
{
	return m_bDestructor;
}


void net_thread::Pause()
{
#ifdef _WIN32
#else
	pthread_mutex_lock(&m_mutex);
	m_pause = true;
	pthread_mutex_unlock(&m_mutex);
#endif
}

void net_thread::Resume()
{
#ifdef _WIN32
	return ::GetCurrentThreadId();
#else
	pthread_mutex_lock(&m_mutex);
	m_pause = false;
	pthread_cond_signal(&m_cond);
	pthread_mutex_unlock(&m_mutex);
#endif
}

unsigned long int net_thread::GetCurrentThreadId()
{
#ifdef _WIN32
	return ::GetCurrentThreadId();
#else
	return pthread_self();
#endif
}

void net_thread::Join()
{
#ifdef _WIN32

#else
	if (m_threadId > 0 && !m_joined && !m_detach)
	{
		m_joined = true;
		pthread_join(m_threadId, NULL);
	}
#endif
}

void net_thread::Join(unsigned long millisTime)
{
#ifdef _WIN32

#else
	if (m_threadId > 0 && !m_detach)
	{
		if (millisTime == 0)
			pthread_join(m_threadId, NULL);
		else
		{
			unsigned long k = 0;
			while (IsRunning() && k < millisTime)
			{
				usleep(10000);
				k += 10;
			}

			if (!IsRunning())
			{
				pthread_join(m_threadId, NULL);
			}
		}
	}
#endif
}

#ifndef _WIN32
void net_thread::InitCancelHandler()
{
	struct sigaction act, oact;
	act.sa_handler = &net_thread::CancelHandler;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	act.sa_restorer = NULL;

	sigaction(SIGUSR1, &act, &oact);
}

void net_thread::CancelHandler(int sig)
{
	pthread_exit(PTHREAD_CANCELED);
}


void net_thread::SetPriority(int priority)
{
	struct sched_param param;

	int policy = SCHED_RR;
	param.__sched_priority = priority;

	if (priority <= 0)
	{
		policy = SCHED_OTHER;
		param.__sched_priority = 0;
	}

	if (pthread_setschedparam(m_threadId, policy, &param) != 0)
	{
		throw net_exception(NET_NUMBER + NET_PTHREAD_SET_SCHED_NO, strerror(errno));
	}
}

int net_thread::GetPriority()
{
	struct sched_param param;
	int policy;

	if (pthread_getschedparam(m_threadId, &policy, &param) == 0)
	{
		return param.__sched_priority;
	}
	else
	{
		throw net_exception(NET_NUMBER + NET_PTHREAD_GET_SCHED_NO, strerror(errno));
	}
}

#endif
}
