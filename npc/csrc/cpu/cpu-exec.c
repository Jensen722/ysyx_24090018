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
#include <locale.h>
#include <utils.h>
#include <cpu/cpu.h>
//#include <isa.h>

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;
static Vtop* top;
vaddr_t pc;

#ifdef CONFIG_DIFFTEST
typedef struct {
  word_t gpr[16];
  vaddr_t pc;
} riscv32_CPU_state;
//regs
  riscv32_CPU_state cpu;
#endif


IFDEF(CONFIG_ITRACE, char logbuf[128]);
static bool g_print_step = false;
uint64_t g_nr_guest_inst = 0;
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

void difftest_step(vaddr_t pc);
static void trace_and_difftest() {
#ifdef CONFIG_ITRACE_COND
  //if (ITRACE_COND) { log_write("%s\n", logbuf); }
  log_write("%s\n", logbuf);
#endif
  if (g_print_step) { IFDEF(CONFIG_ITRACE, puts(logbuf)); }
  IFDEF(CONFIG_DIFFTEST, difftest_step(pc));

#ifdef CONFIG_WATCHPOINT
  bool trrigle = false;
  trrigle = scan_wp();
  if(trrigle){
    npc_state.state = NPC_STOP;
  }
#endif
}

static void exec_once(){
  pc = top->pc_o;
  top->inst_i = vaddr_read(top->pc_o, 4);
  single_cycle();

#ifdef CONFIG_DIFFTEST
  for(int i = 0; i < 16; i++){
    cpu.gpr[i] = top->rootp->top__DOT__RF__DOT__rf[i];
  }
  cpu.pc = top->pc_o;
#endif

  svLogicVecVal inst;
  svSetScope(svGetScopeFromName("TOP.top.EXU"));
  if (!svSetScope(svGetScopeFromName("TOP.top.EXU"))) {
    printf("Error: Failed to set scope.\n");
    return;
  }
  get_inst(&inst);
// 提取 32 位值
  uint32_t inst_value = (uint32_t)(inst.aval & 0xFFFFFFFF);
  printf("inst_o = 0x%08x\n", inst_value);

  svBit ebreak;
  svSetScope(svGetScopeFromName("TOP.top.EXU"));
  top->get_ebreak(&ebreak);
  if(ebreak) {NPCTRAP(pc, top->rootp->top__DOT__RF__DOT__rf[10]);printf("Finish executing instruction!\n");} //

#ifdef CONFIG_ITRACE
  char *p = logbuf;
  p += snprintf(p, sizeof(logbuf), FMT_WORD ":", pc);
  int ilen = 4;
  int i;
  uint8_t *inst_ch = (uint8_t *)&inst_value;
  for (i = ilen - 1; i >= 0; i --) {
    p += snprintf(p, 4, " %02x", inst_ch[i]);
  }
  int ilen_max = 4;
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len;

  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, logbuf + sizeof(logbuf) - p,
      pc, (uint8_t *)&inst_value, ilen);

#endif
}

void isa_reg_display();
void assert_fail_msg(){
  isa_reg_display();
}

void execute(uint64_t n){
  for(;n > 0; n --){
    exec_once();
    g_nr_guest_inst ++;
    trace_and_difftest();
  }
}

static void statistic() {
#define NUMBERIC_FMT MUXDEF(CONFIG_TARGET_AM, "%", "%'") PRIu64
  Log("total guest instructions = " NUMBERIC_FMT, g_nr_guest_inst);
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


//difftest 这部分应该要迁移至dut.c
#include <dlfcn.h>
#include <memory/paddr.h>
#include <difftest-def.h>

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;


  riscv32_CPU_state *ncpu = &cpu;
#ifdef CONFIG_DIFFTEST

void init_difftest(char *ref_so_file, long img_size, int port){
  assert(ref_so_file != NULL);


  //66-83line就是为了能调用ref.c中的函数
  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);//加载动态库，只有在函数调用的时候才解析符号，并返回句柄handle
  assert(handle);

  ref_difftest_memcpy = (void (*)(paddr_t, void*, size_t, bool))dlsym(handle, "difftest_memcpy");
  riscv32_CPU_state *ncpu = &cpu;
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = (void (*)(void*, bool))dlsym(handle, "difftest_regcpy");//从动态库handle中加载名为'difftest_memcpy'的符号
  assert(ref_difftest_regcpy);

  ref_difftest_exec = (void (*)(uint64_t))dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = (void (*)(uint64_t))dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = (void (*)(int))dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in menuconfig.", ref_so_file);

  for(int i = 0; i < 16; i++){
    cpu.gpr[i] = top->rootp->top__DOT__RF__DOT__rf[i];
  }
  cpu.pc = top->pc_o;

  ref_difftest_regcpy(&cpu, DIFFTEST_TO_REF);
  ref_difftest_init(port);
  ref_difftest_memcpy(RESET_VECTOR, guest_to_host(RESET_VECTOR), img_size, DIFFTEST_TO_REF);
}

bool isa_difftest_checkregs(riscv32_CPU_state *ref_r, vaddr_t pc) {
  for(int i = 0; i < 16; i++){
    if(cpu.gpr[i] != ref_r->gpr[i]){
      printf("regs not match at pc = %x\n", pc);
      return false;
    } 
  }
  return true;
}

static void checkregs(riscv32_CPU_state *ref, vaddr_t pc) { //如果寄存器状态不一致，则记录pc值，打印出寄存器的值
  if (!isa_difftest_checkregs(ref, pc)) {
    npc_state.state = NPC_ABORT;
    npc_state.halt_pc = pc;
    isa_reg_display();
  }
}

void difftest_step(vaddr_t pc) {
  riscv32_CPU_state ref_r;
  ref_difftest_exec(1); //正常执行ref一条指令
  ref_difftest_regcpy(&ref_r, DIFFTEST_TO_DUT); //将ref的寄存器拷贝至DUT中的虚拟寄存器结构体

  checkregs(&ref_r, pc);
}

#endif
