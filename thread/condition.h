/*
 * @version: 0.1 2021-5-24 
 * @author: Conzxy
 * simple wrapper of condition variable
 */

#ifndef _CONDVARIABLE_H
#define _CONDVARIABLE_H

#include <pthread.h>
#include "pthreadM.h"
#include <noncopyable.h>
#include "mutexlock.h"

namespace zxy{

class Condition : public noncopyable {
public:
	Condition(MutexLock& mutex)
		: mutex_{mutex}
	{
		TCHECK(pthread_cond_init(&cond_, NULL));
	}

	~Condition(){
		TCHECK(pthread_cond_destroy(&cond_));
	}

	void wait(){
		TCHECK(pthread_cond_wait(&cond_, &mutex_.pthreadMutex()));
	}

	bool waitForSeconds(double seconds);

	void notify(){
		TCHECK(pthread_cond_signal(&cond_));
	}

	void notifyAll(){
		TCHECK(pthread_cond_broadcast(&cond_));
	}

private:
	MutexLock& mutex_;	
	pthread_cond_t cond_;
};

}//namespace zxy
#endif
