/*************************************************************************
	> File Name: csrc/include/common.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 16时40分57秒
 ************************************************************************/

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>


#define CONFIG_MSIZE 0x8000000
#define CONFIG_MBASE 0x80000000
#include <generated/autoconf.h>
#include <macro.h>

#include <assert.h>
#include <stdlib.h>
typedef uint32_t word_t;
#define FMT_WORD "0x%08" PRIx32


typedef word_t vaddr_t;
typedef uint32_t paddr_t;
#define FMT_PADDR "0x%08" PRIx32
#include <debug.h>

#endif
