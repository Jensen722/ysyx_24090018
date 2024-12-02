/*************************************************************************
	> File Name: memory/paddr.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 10时56分33秒
 ************************************************************************/

#include <common.h>
#include <debug.h>
#define CONFIG_MSIZE 0x8000000
#define CONFIG_MBASE 0x80000000


static inline int host_read(void *addr){
  return *(uint32_t *)addr;
}

static uint8_t *pmem = NULL;
uint8_t* guest_to_host(uint32_t addr) {
  uint8_t* addr_ptr = pmem + addr - CONFIG_MBASE;
  if (addr_ptr < pmem || addr_ptr >= pmem + CONFIG_MSIZE) {
    printf("out of bound\n");
    assert(0);
  }
  return pmem + addr - CONFIG_MBASE; 
}

int pmem_read(uint32_t raddr){
  int aligned_addr = raddr & ~0x3u;
  int ret = host_read(guest_to_host(aligned_addr));
  return ret;
}
void pmem_write(int waddr, int wdata, char wmask){

}

void init_mem() {
  pmem = (uint8_t *)malloc(CONFIG_MSIZE);
  assert(pmem);
memset(pmem, 0x00100073, CONFIG_MSIZE);
  printf("0x%08x\n", *(pmem+100-1));
}


