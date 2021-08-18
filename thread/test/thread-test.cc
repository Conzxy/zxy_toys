#include "../thread.h"

using namespace zxy;

int main(int argc, char* argv[])
{
	Thread thr([]{
		printf("ThreadName: %s\n", zxy::CurrentThread::t_name);				
		});

	thr.start();
	thr.join();
}
