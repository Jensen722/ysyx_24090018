/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/include/memory/paddr.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月20日 星期四 15时27分39秒
 ************************************************************************/

#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include <common.h>

#define PMEM_LEFT  ((paddr_t)CONFIG_MBASE)
#define PMEM_RIGHT ((paddr_t)CONFIG_MBASE + CONFIG_MSIZE - 1)
#define RESET_VECTOR (PMEM_LEFT + CONFIG_PC_RESET_OFFSET)


/* convert the guest physical address in the guest program to host virtual address in NPC */
uint8_t* guest_to_host(paddr_t paddr);
/* convert the host virtual address in NPC to guest physical address in the guest program */
paddr_t host_to_guest(uint8_t *haddr);

static inline bool in_pmem(paddr_t addr) {
  return addr - CONFIG_MBASE < CONFIG_MSIZE;
}


#ifdef __cplusplus
extern "C" {
#endif
int paddr_read(int addr, int len);
void paddr_write(int addr, int len, int data, char wmask);
#ifdef __cplusplus
}
#endif
#endif
