/*
 * @version: 0.1 2021-5-24
 * @author: Conzxy
 * some macros used to pthread test
 */

#ifndef _THREADM_H
#define _THREADM_H

#include "util/check.h"

#ifndef PTHREAD_CHECK

#undef ASSERT
#define ASSERT assert

#endif // PTHREAD_CHECK

#define TCHECK(ret) \
	({ \
		ASSERT(ret == 0); \
	 })


#endif // _PTHREADM_H
