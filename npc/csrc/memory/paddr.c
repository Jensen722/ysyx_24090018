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


/*static inline int host_read(void *addr){
  return *(uint32_t *)addr;
}*/

static uint8_t *pmem = NULL;
uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - CONFIG_MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + CONFIG_MBASE; }

static int pmem_read(int addr, int len){
  word_t aligned_raddr = addr & ~0x3u;  //按4字节对齐
  word_t ret = host_read(guest_to_host(aligned_raddr), len);
  return ret;
}

static void pmem_write(int addr, int len, int wdata, char wmask) {
  word_t aligned_waddr = addr & ~0x3u;
  word_t new_wdata;
  if(wmask == 1){
    new_wdata = wdata & 0xFF;
  } else if(wmask == 3) {
    new_wdata = wdata & 0xFFFF;
  } else if(wmask ==  7){
    new_wdata = wdata & 0xFFFFFF;
  } else if(wmask == 15){
    new_wdata = wdata & 0xFFFFFFFF;
  } else {assert(0);}
  host_write(guest_to_host(addr), len, new_wdata);
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

extern "C"  int paddr_read(int addr, int len) {
#ifdef CONFIG_MTRACE
    printf(ANSI_FMT("[Read ] ", ANSI_FG_GREEN)"Address: 0x%X | Value_Read:  0x%08X\n", addr, pmem_read(addr, len));
#endif
  if (likely(in_pmem(addr))) return pmem_read(addr, len);
  out_of_bound(addr);
  return 0;
}

extern "C"  void paddr_write(int addr, int len, int wdata, char wmask) {
#ifdef CONFIG_MTRACE
    printf(ANSI_FMT("[Write] ", ANSI_FG_YELLOW)"Address: 0x%X | Value_Write: 0x%08X\n", addr, wdata);
#endif
  if (likely(in_pmem(addr))) { pmem_write(addr, len, wdata, wmask); return; }
  out_of_bound(addr);
}
