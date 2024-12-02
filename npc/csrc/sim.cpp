#include "verilated.h"
#include "verilated_vcd_c.h"
#include "Vtop.h"
#include <time.h>
#include "Vtop__Dpi.h"

VerilatedContext* contextp = NULL;
VerilatedVcdC* tfp = NULL;

static Vtop* top;

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
  tfp->open("wave.vcd");
}

void sim_exit(){
  step_and_dump_wave();
  tfp->close();
}
static void single_cycle() {
  top->clk = 0; step_and_dump_wave();
  top->clk = 1; step_and_dump_wave();
}

static void reset(int n) {
  top->rst = 0;
  while (n -- > 0) single_cycle();
  top->rst = 1;
}


//static uint32_t inst_mem[1000] = {}; 
/*static void put_inst(){
  inst_mem[0] = 0x00500093; //addi x1, x0, 5
  inst_mem[1] = 0x00a08113; //addi x2, x1, 10
  inst_mem[2] = 0x2b878793; //addi x15, x15, 696
  inst_mem[3] = 0x00710293; //addi x5, x3, 7
  inst_mem[4] = 0xff410113; //addi sp, sp, -12
  inst_mem[5] = 0xfe010113; //addi sp, sp, -32
  inst_mem[6] = 0x01c50513; //addi a0, a0, 28
  inst_mem[7] = 0x00848493; //addi s1, s1, 8
  inst_mem[8] = 0x00100073; //ebreak
}
static uint32_t pmem_read(uint32_t raddr){
  int idx = (raddr - 0x80000000) / 4;
  return inst_mem[idx];
}*/
int pmem_read(uint32_t raddr);
void init_monitor(int argc, char *argv[]);
int main(int argc, char *argv[]) {
  init_monitor(argc, argv);
  //printf("inst = 0x%08x\n", pmem_read(0x80000000));
  sim_init();

  reset(1); 

  while(1) {
    top->inst_i = pmem_read(top->pc_o);
    //printf("inst = 0x%08x\n", pmem_read(top->pc_o));
    single_cycle();

    svBit ebreak;
    svSetScope(svGetScopeFromName("TOP.top.EXU"));
    get_ebreak(&ebreak);
    if(ebreak) {printf("Finish executing instruction!\n");break;}
  }


  sim_exit();
}


