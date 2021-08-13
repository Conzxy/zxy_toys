/*
 * @version: 2021-5-22
 * @author: Conzxy
 */

#ifndef _MUTEXLOCK_H
#define _MUTEXLOCK_H

#include <pthread.h>
#include <stdexcept>
#include <errorHandle.h>
#include <noncopyable.h>
#include "pthreadM.h"

namespace zxy{

class MutexLock : noncopyable
{
private:
	pthread_mutex_t mutex_;

public:
	MutexLock(){
		TCHECK(pthread_mutex_init(&mutex_, NULL));
	}
	
	~MutexLock(){
		TCHECK(pthread_mutex_destroy(&mutex_));
	}

	void lock(){
		TCHECK(pthread_mutex_lock(&mutex_));
	}

	void trylock(){
		TCHECK(pthread_mutex_trylock(&mutex_));
	}

	void unlock(){
		TCHECK(pthread_mutex_unlock(&mutex_));
	}

	void timedlock(timespec const* tout){
		TCHECK(pthread_mutex_timedlock(&mutex_, tout));
	}

	pthread_mutex_t& pthreadMutex(){
		return mutex_;
	}

};

//pthread_mutex_t simple RAII wrapper
class MutexGuard {
private:
	MutexLock& mutex_;
public:
	explicit MutexGuard(MutexLock& mutex)
		: mutex_{mutex}
	{ mutex_.lock(); }

	~MutexGuard()
	{ mutex_.unlock(); };
};

#define MUTEXGURAD(tag, lock)\
	zxy::MutexGuard mg_##tag(lock);

}//namespace zxy

#endif
