/*************************************************************************
	> File Name: memory/paddr.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 09时26分43秒
 ************************************************************************/

#include <memory/host.h>

#if   defined(CONFIG_PMEM_MALLOC)
static uint8_t *pmem = NULL;
#else // CONFIG_PMEM_GARRAY
static uint8_t pmem[CONFIG_MSIZE] PG_ALIGN = {};
#endif

int* guest_to_host(int addr) { return pmem + addr - CONFIG_MBASE; }

int pmem_read(int raddr){
  int aligned_addr = raddr & ~0x3u;
  int ret = host_read(guest_to_host(aligned_addr));
  return ret;
}

void pmem_write(int waddr, int wdata, char wmask){

}

void init_mem() {
#if   defined(CONFIG_PMEM_MALLOC)
  pmem = malloc(CONFIG_MSIZE);
  assert(pmem);
#endif
}
