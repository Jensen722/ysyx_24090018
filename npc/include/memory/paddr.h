/*************************************************************************
	> File Name: include/memory/paddr.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 09时53分30秒
 ************************************************************************/
#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

int pmem_read(int addr);
void pmem_write(int addr, int data, char wmask);

#endif
