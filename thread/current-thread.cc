#include "current-thread.h"
#include <cstring>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>
#include "string/lexical-cast.h"

namespace zxy::CurrentThread {

__thread int t_tid = 0;
__thread char t_tidString[32] = {};
__thread int t_tidLength = 0;
__thread char const* t_name = nullptr; 

// because main thread is not created by pthread_create()
// need explicit cache
struct MainThreadInit {
	MainThreadInit()
	{
		CurrentThread::t_name = "main";
		CurrentThread::tid();

	}
};

MainThreadInit mainThreadInit{};

pid_t gettid() 
{ return ::syscall(SYS_gettid); }

void cacheTid() noexcept
{
	if( __builtin_expect(t_tid == 0, 1) ) {
		t_tid = gettid();
		strcpy(t_tidString, lexical_cast<char const*>(t_tid));
	}
}

bool isMainThread() noexcept
{ return CurrentThread::t_tid == ::getpid(); }

} // namespace zxy::CurrentThread
