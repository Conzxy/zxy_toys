/*
 * @version: 0.1 2021-5-23
 * @author: Conzxy
 * implement of thread.h
 */

#include "thread.h"
#include "atomic.h"
#include <noncopyable.h>
#include <stdio.h>
#include <assert.h>
#include <errorHandle.h>

using namespace std;

namespace zxy{

namespace detail{
	class ThreadData : noncopyable{
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
		try
		{
			func_();
		}
		catch(std::exception const& stdex){
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
			fprintf(stderr, "reason: %s\n", stdex.what());
			abort();
		}
		catch(...){
			fprintf(stderr, "exception caught in Thread %s\n", name_.c_str());
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
		  is_barriered_{false},
		  pthreadId_{0},
		  name_{name}
	{
		setDefaultname();
		numCreated_.increment();
	}

	Thread::~Thread(){
		if(is_started_ && !is_joined_ && !is_barriered_){
			pthread_detach(pthreadId_);
		}
	}

	void Thread::start(){
		assert(!is_started_);
		is_started_ = true;
		
		auto data = new detail::ThreadData{func_, name_};
		int err;
		if((err = pthread_create(&pthreadId_, NULL, &detail::startThread, data)) != 0){
			is_started_ = false;
			delete data;

			err_exit(err, "Failed in pthread_create");
		}
			
	}

	void Thread::join(){
		assert(is_started_);
		assert(!is_joined_);

		is_joined_ = true;

		pthread_join(pthreadId_, NULL);
	}

	void Thread::setDefaultname(){
		if(!name_.empty()) return ;
		
		char name[20];
		snprintf(name, sizeof(name), "thread%d", numCreated_.get());
		name_ = name;
	}

}//namespace zxy
