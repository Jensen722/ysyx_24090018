/*************************************************************************
	> File Name: memory/paddr.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年12月02日 星期一 10时56分33秒
 ************************************************************************/

#include <memory/host.h>
#include <memory/paddr.h>
#include <common.h>
#include <debug.h>


static inline int host_read(void *addr){
  return *(uint32_t *)addr;
}

static uint8_t *pmem = NULL;
uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static word_t pmem_read(paddr_t addr, int len){
  word_t aligned_addr = addr & ~0x3u;
  word_t ret = host_read(guest_to_host(aligned_addr), len);
  return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
  host_write(guest_to_host(addr), len, data);
}

static void out_of_bound(paddr_t addr) {
  //panic("address = " FMT_PADDR " is out of bound of pmem [" FMT_PADDR ", " FMT_PADDR "] at pc = " /*FMT_WORD*/,
      //addr, PMEM_LEFT, PMEM_RIGHT/*, cpu.pc*/);
}

void init_mem() {
  pmem = (uint8_t *)malloc(CONFIG_MSIZE);
  assert(pmem);
  Log("physical memory area [" FMT_PADDR ", " FMT_PADDR "]", PMEM_LEFT, PMEM_RIGHT);
}

word_t paddr_read(paddr_t addr, int len) {
  if (likely(in_pmem(addr))) return pmem_read(addr, len);
  out_of_bound(addr);
  return 0;
}

void paddr_write(paddr_t addr, int len, word_t data) {
  if (likely(in_pmem(addr))) { pmem_write(addr, len, data); return; }
  out_of_bound(addr);
}
