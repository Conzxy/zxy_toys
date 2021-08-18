#ifndef _ZXY_COUNTDOWN_LATCH_H
#define _ZXY_COUNTDOWN_LATCH_H

#include "mutexlock.h"
#include "condition.h"
#include "util/noncopyable.h"

// simple facility instead of pthread_barrier
// common usage:
// 1) main thread wait other thread complete their task
namespace zxy {

class CountDownLatch : noncopyable
{
public:
	explicit CountDownLatch(int count)
		: count_(count), mutex_(), cond_(mutex_)
	{ }

	void wait()
	{
		MutexGuard guard(mutex_);
		if(count_ > 0)
			cond_.wait();
	}

	void countdown() noexcept
	{
		MutexGuard guard(mutex_);
		if(--count_ == 0)
			cond_.notifyAll();
	}
private:
	int count_;
	MutexLock mutex_;
	Condition cond_;
}; 

} // namespace zxy

#endif // _ZXY_COUNTDOWN_LATCH_H
