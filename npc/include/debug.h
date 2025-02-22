/*************************************************************************
	> File Name: include/debug.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 17时01分36秒
 ************************************************************************/

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <common.h>
#include <stdio.h>
#include <utils.h>
#include <assert.h>

#define Log(format, ...) \
    _Log(ANSI_FMT("[%s:%d %s] " format, ANSI_FG_BLUE) "\n", \
        __FILE__, __LINE__, __func__, ## __VA_ARGS__)

#define Assert(cond, format, ...) \
  do { \
      fflush(stdout), fprintf(stderr, ANSI_FMT(format, ANSI_FG_RED) "\n", ##  __VA_ARGS__); \
      extern FILE* log_fp;fflush(log_fp); \
      extern void assert_fail_msg(); \
      assert_fail_msg(); \
      assert(cond); \
  } while (0)

#define panic(format, ...) Assert(0, format, ## __VA_ARGS__)

#define TODO() panic("please implement me")

#endif

