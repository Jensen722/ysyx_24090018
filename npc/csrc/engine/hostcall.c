/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/csrc/engine/hostcall.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月21日 星期五 16时09分57秒
 ************************************************************************/
#include <utils.h>
void set_npc_state(int state, vaddr_t pc, int halt_ret) {
  //difftest_skip_ref();
  npc_state.state = state;
  npc_state.halt_pc = pc;
  npc_state.halt_ret = halt_ret;
}


