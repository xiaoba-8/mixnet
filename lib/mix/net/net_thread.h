/*
 * net_thread.h
 *
 *  Created on: 2019-4-13
 *      Author: xiaoba-8
 */

#ifndef THREAD_H_
#define THREAD_H_

#ifdef _WIN32
#else
#include <pthread.h>
#endif

#ifdef _WIN32

typedef unsigned threadfunc_t;
typedef void * threadparam_t;
#define STDPREFIX __stdcall
#else

typedef void * threadfunc_t;
typedef void * threadparam_t;
#define STDPREFIX
#endif


#include <mix/net/Task.h>

namespace mix
{

class net_thread
{
public:
	net_thread(Task *pTask, bool detach = false, bool bDeleteOnFinish = true);
	virtual ~net_thread();

	static threadfunc_t STDPREFIX StartThread(threadparam_t);

	virtual void Run();

#ifdef _WIN32
	HANDLE GetThread() { return m_thread; }
	unsigned int GetThreadId() { return m_dwThreadId; }
#else
	pthread_t GetThread() { return m_threadId; }
	unsigned long int GetThreadId() { return m_threadId; }
#endif

	bool IsRunning();
	void SetRunning(bool x);
	bool IsDeleteOnFinish();
	void SetDeleteOnFinish(bool x = true);
	bool IsDestructor();

	virtual bool Start();

	virtual void Stop();

	virtual unsigned long int GetId();

	static unsigned long int GetCurrentThreadId();

	virtual void Join();
	virtual void Join(unsigned long millisTime);

	virtual void SetPriority(int priority);
	virtual int GetPriority();

	virtual void Pause();
	virtual void Resume();

protected:

#ifdef _WIN32
	HANDLE m_thread;
	unsigned m_dwThreadId;
#else
	static void CancelHandler(int sig);
	void InitCancelHandler();

	pthread_attr_t m_attr;

	bool m_pause;
	//bool m_suspend;
	pthread_mutex_t m_mutex;
	pthread_cond_t m_cond;

#endif

	net_thread(const net_thread& )  {}
	net_thread& operator=(const net_thread& ) { return *this; }

	unsigned long int m_threadId;

	bool m_running;
	bool m_bDeleteOnFinish;
	bool m_bDestructor;
	bool m_joined;
	bool m_detach;

	Task *m_pTask;
};

}

#endif /* THREAD_H_ */
