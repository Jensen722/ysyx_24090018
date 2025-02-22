/*************************************************************************
	> File Name: cpu-exec.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月19日 星期三 21时56分00秒
 ************************************************************************/
#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vtop.h"
#include <time.h>
#include "Vtop__Dpi.h"
#include "Vtop___024root.h"  // 包含完整的类定义

#include <debug.h>
#include <memory/vaddr.h>
#include <utils.h>
#include <cpu/cpu.h>

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;
static Vtop* top;


static bool g_print_step = false;
bool *scan_wp();

void step_and_dump_wave(){
  top->eval();
  contextp->timeInc(1);
  tfp->dump(contextp->time());
}

void sim_init(){
  contextp = new VerilatedContext;
  tfp = new VerilatedVcdC;
  top = new Vtop;
  contextp->traceEverOn(true);
  top->trace(tfp, 0);
  tfp->open("./build/wave.vcd");
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}

static void single_cycle() {
  top->clk = 0; step_and_dump_wave();
  top->clk = 1; step_and_dump_wave();
}

void reset(uint64_t n) {
  top->rst = 0;
  while (n -- > 0) single_cycle();
  top->rst = 1;
}

static void trace_and_difftest() {
/*#ifdef CONFIG_ITRACE_COND
  if (ITRACE_COND) { log_write("%s\n", _this->logbuf); }
#endif
  if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(_this->logbuf)); }
  IFDEF(CONFIG_DIFFTEST, difftest_step(_this->pc, dnpc));*/

#ifdef CONFIG_WATCHPOINT
  bool trrigle = false;
  trrigle = scan_wp();
  if(trrigle){
    npc_state.state = NPC_STOP;
  }
#endif
}

static void exec_once(){
  top->inst_i = vaddr_read(top->pc_o, 4);
  printf("pc=0x%x, inst = 0x%08x\n", top->pc_o, vaddr_read(top->pc_o, 4));
  single_cycle();
  svBit ebreak;
  svSetScope(svGetScopeFromName("TOP.top.EXU"));
  get_ebreak(&ebreak);
  if(ebreak) {NPCTRAP(1,1);printf("Finish executing instruction!\n");} //
}

void isa_reg_display();
void assert_fail_msg(){
  isa_reg_display();
}

void execute(uint64_t n){
  for(;n > 0; n --){
    exec_once();
    trace_and_difftest();
  }
}

static void statistic() {
  Log("Exit!");
}

void cpu_exec(uint64_t n){
  g_print_step = (n < MAX_INST_TO_PRINT);
  switch (npc_state.state) {
    case NPC_END: case NPC_ABORT:
      printf("Program execution has ended. To restart the program, exit NPC and run again.\n");
      return;
    default: npc_state.state = NPC_RUNNING;
  }

  execute(n);

  switch (npc_state.state) {
    case NPC_RUNNING: npc_state.state = NPC_STOP; break;

    case NPC_END: case NPC_ABORT:
      Log("npc: %s at pc = " FMT_WORD,
          (npc_state.state == NPC_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
           (npc_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
            ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
          npc_state.halt_pc);
      // fall through
    case NPC_QUIT: statistic();
  }
}

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
    printf("%-10s | 0x%08X | %-10u\n", regs[i], top->rootp->top__DOT__RF__DOT__rf[i], top->rootp->top__DOT__RF__DOT__rf[i]);
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
      return top->rootp->top__DOT__RF__DOT__rf[i];
    }
  }

  *success = false;
  return 0;
}
