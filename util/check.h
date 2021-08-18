#ifndef _ZXY_CHECK_H
#define _ZXY_CHECK_H

#include "string/errlog.h"
#include <stdlib.h>
#include <string.h>

namespace zxy {

// if set the argument in the end of errlog(),
// it print a unexpected newline
// I don't know why
#define ASSERT(ret) \
	({if ( __builtin_expect(ret == 0, 0))\
	 {\
		errlog("errnum: ", ret, "; error Message: ", strerror(ret),\
				";", __FILE__, ";", __LINE__, ";", __func__); \
		errlog.flush(); \
		fflush(stderr); \
		abort();\
	 }})

} // namespace zxy

#endif // _ZXY_CHECK_H
