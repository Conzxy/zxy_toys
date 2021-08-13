/*
 * @version: 0.1 2021-5-28
 * @author: Conzxy
 * simple wrapper of pthread_barrier
 */

#ifndef _BARRIER_H
#define _BARRIER_H

#include <noncopyable.h>
#include <pthread.h>
#include "pthreadM.h"

namespace zxy{

class Barrier : public noncopyable {
public:
	Barrier()
	{}
	
	~Barrier()
	{
		TCHECK(pthread_barrier_destroy(&barrier_));
	}

	void start(unsigned int cnt){
		TCHECK(pthread_barrier_init(&barrier_, NULL, cnt));
	}

	void wait(){
		TCHECK2(pthread_barrier_wait(&barrier_), PTHREAD_BARRIER_SERIAL_THREAD);		
	}
	
private:
	pthread_barrier_t barrier_;
};

}

#endif
