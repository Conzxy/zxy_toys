/*
 * @version: 0.1 2021-5-24
 * @author: Conzxy
 * some macros used to pthread test
 */

#ifndef _THREADM_H
#define _THREADM_H

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>

constexpr int gErrLine = 256;

//pthread_attr_t 待补充
#define TCHECK(ret)	\
		({ __typeof__(ret) err = ret; \
		   if(err != 0){\
		       char buf[gErrLine];\
			   snprintf(buf, gErrLine-1, "Error Message:%s\n", strerror(err));\
			   fputs(buf, stdout);\
		   }\
		   assert(err == 0);	})

#define TCHECK2(ret, option) \
		({ __typeof__(ret) err = ret; \
		  if(err != 0 && err != option){\
			char buf[gErrLine];	\
			snprintf(buf, gErrLine - 1, "Error Message:%s\n", strerror(err)); \
			fputs(buf, stdout);\
			assert(err == 0);\
		  }})
		  
#endif
