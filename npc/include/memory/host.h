/*************************************************************************
	> File Name: include/memory/host.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 09时37分09秒
 ************************************************************************/

#ifndef __MEMORY_HOST_H__
#define __MEMORY_HOST_H__

static inline int host_read(void *addr){
  return *(int *)addr;
}

#endif
