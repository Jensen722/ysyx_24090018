/*************************************************************************
	> File Name: memory/paddr.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 10时56分33秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<assert.h>
#include<stdint.h>
#include<string.h>

#define CONFIG_MSIZE 0x8000000
#define CONFIG_MBASE 0x80000000


static inline int host_read(void *addr){
  return *(int *)addr;
}

static uint8_t *pmem = NULL;
uint8_t* guest_to_host(int addr) {
  uint8_t* addr_ptr = pmem + addr - CONFIG_MBASE;
  printf("addr: %d addr_ptr: %p pmem: %p\n", addr - CONFIG_MBASE, addr_ptr, pmem);
if (addr_ptr < pmem || addr_ptr >= pmem + CONFIG_MSIZE) {assert(0);}
  return pmem + addr - CONFIG_MBASE; }
int pmem_read(int raddr){
  int aligned_addr = raddr & ~0x3u;
  int ret = host_read(guest_to_host(aligned_addr));
  return ret;
}
void pmem_write(int waddr, int wdata, char wmask){

}

void init_mem() {
  pmem = (uint8_t *)malloc(CONFIG_MSIZE);
  assert(pmem);
memset(pmem, rand(), CONFIG_MSIZE);
  printf("%x\n", *(pmem+CONFIG_MSIZE-1));
}


