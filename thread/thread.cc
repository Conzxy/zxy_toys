#include "thread.h"
#include "atomic.h"
#include "noncopyable.h"
#include "util/check.h"
#include "current-thread.h"
#include <stdio.h>
#include <sys/prctl.h>

using namespace std;

namespace zxy{

namespace detail{

class ThreadData : noncopyable
{
public:
	using Threadfunc = Thread::Threadfunc;

	ThreadData(Threadfunc func, string const& name);
	~ThreadData(){}

	void run() const;
private:
	Threadfunc func_;
	string     name_;
};

ThreadData::ThreadData(Threadfunc func, string const& name)
	: func_{std::move(func)}, 
	  name_{name}
{}

void ThreadData::run() const{
	(void)CurrentThread::tid();
	CurrentThread::t_name = name_.empty() ? "zxyThread" : name_.c_str();
	// process control
	// set calling thread name
	// note: the second argument just up to 16 bytes(including terminating null byte)
	::prctl(PR_SET_NAME, CurrentThread::t_name);
	
	try
	{
		func_();
		CurrentThread::t_name = "finished";

	}
	catch(std::exception const& stdex){
		errlog("exception caught in Thread ", name_.c_str(), 
				"\nreason: ", stdex.what(), "\n");
		abort();
	}
	catch(...){
		errlog("exception caught in Thread ", name_.c_str(), "\n");
		throw;	//rethrow
	}
}

void* startThread(void* arg){
	ThreadData* data = static_cast<ThreadData*>(arg);
	data->run();
	delete data;
	
	return nullptr;
}

}//namespace detail

AtomicInt32 Thread::numCreated_{};

Thread::Thread(Threadfunc func, string const& name)
	: func_{std::move(func)},
	  is_started_{false},
	  is_joined_{false},
	  pthreadId_{0},
	  name_{name}
{
	numCreated_.increment();
	setDefaultname();
}

Thread::~Thread(){
	if(is_started_ && !is_joined_) {
		pthread_detach(pthreadId_);
	}
}

void Thread::start(){
	ASSERT(!is_started_);
	is_started_ = true;
	
	auto data = new detail::ThreadData{func_, name_};
	int err;
	if((err = pthread_create(&pthreadId_, NULL, &detail::startThread, data)) != 0){
		is_started_ = false;
		delete data;

		perror("Failed in pthread_create");
		exit(1);
	}
		
}

void Thread::join(){
	ASSERT(is_started_);
	ASSERT(!is_joined_);

	is_joined_ = true;

	pthread_join(pthreadId_, NULL);
}

void Thread::setDefaultname(){
	if(!name_.empty()) return ;
	
	fsstream << "Thread" << numCreated_.get();
	name_ = fsstream.data();

	fsstream.reset();
}

}//namespace zxy
