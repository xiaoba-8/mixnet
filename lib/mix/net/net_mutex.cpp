/*
 * net_mutex.cpp
 *
 *  Created on: 2019-2-8
 *      Author: xiaoba-8
 */

#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include "net_mutex.h"

namespace mix
{

#ifdef _WIN32

net_mutex::net_mutex()
{
	m_mutex = CreateMutex(NULL,FALSE,NULL);
}

net_mutex::~net_mutex()
{
	CloseHandle(m_mutex);
}

void net_mutex::lock()
{
	WaitForSingleObject(m_mutex,INFINITE);
}

void net_mutex::unlock()
{
	ReleaseMutex(m_mutex);
}

#else

net_mutex::net_mutex() : m_mutex(PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP)
{
}
net_mutex::~net_mutex()
{
	pthread_mutex_destroy(&m_mutex);
}

void net_mutex::lock()
{
	pthread_mutex_lock(&m_mutex);
}

void net_mutex::unlock()
{
	pthread_mutex_unlock(&m_mutex);
}

#endif

}
