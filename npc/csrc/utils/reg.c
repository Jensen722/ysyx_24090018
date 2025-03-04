/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/csrc/utils/reg.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月21日 星期五 19时05分56秒
 ************************************************************************/
/*#include <common.h>
#include "Vtop.h"
#include "Vtop___024root.h"  // 包含完整的类定义

Vtop *ntop = top;
const char *regs[] = {
  "$0", "ra", "sp", "gp", "tp", "t0", "t1", "t2",
  "s0", "s1", "a0", "a1", "a2", "a3", "a4", "a5",
  "a6", "a7", "s2", "s3", "s4", "s5", "s6", "s7",
  "s8", "s9", "s10", "s11", "t3", "t4", "t5", "t6"
};
void isa_reg_display(){
  int i;

  printf("+------------------------------------+\n");
  printf("%-10s | %-10s | %-10s\n", "Register", "Hex Value", "Dec Value");
  printf("+------------------------------------+\n");
  for(i = 0; i < 16; i++){
    printf("%-10s | 0x%08X | %-10u\n", regs[i], ntop->rootp->top__DOT__RF__DOT__rf[i], ntop->rootp->top__DOT__RF__DOT__rf[i]);
  }
  printf("+------------------------------------+\n");
}

word_t isa_reg_str2val(const char *s, bool *success) {
  //$pc
  if(strcmp(s+1, "pc") == 0){
    *success = true;
    return top->pc_o;
  }

  //32 normal regs
  for(int i = 0; i < 16; i++){
    if(strcmp(s+1, regs[i]) == 0){
      *success = true;
      return ntop->rootp->top__DOT__RF__DOT__rf[i];
    }
  }

  *success = false;
  return 0;
}*/
