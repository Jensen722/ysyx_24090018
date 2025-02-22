/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/include/cpu/cpu.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月20日 星期四 11时26分21秒
 ************************************************************************/

#ifndef __CPU_CPU_H__
#define __CPU_CPU_H__

#include <common.h>

void cpu_exec(uint64_t n);

void set_npc_state(int state, vaddr_t pc, int halt_ret);


#define NPCTRAP(thispc, code) set_npc_state(NPC_END, thispc, code)

#endif
