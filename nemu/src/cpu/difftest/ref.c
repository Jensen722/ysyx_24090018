/***************************************************************************************
* Copyright (c) 2014-2022 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include <isa.h>
#include <cpu/cpu.h>
#include <difftest-def.h>
#include <memory/paddr.h>

//声明一个跟dut一样的结构体类型，方便进行类型转换
struct diff_context_t {
  word_t gpr[16];
  word_t pc;
};

void diff_step(uint64_t n){
  cpu_exec(n);
}

void diff_get_regs(void* diff_context){
  struct diff_context_t* ctx = (struct diff_context_t *) diff_context;
  for(int i = 0; i < 16; i++){
    ctx->gpr[i] = cpu.gpr[i];
  }
  ctx->pc = cpu.pc;
}

void diff_set_regs(void* diff_context){
  struct diff_context_t* ctx = (struct diff_context_t*) diff_context;
  for(int i = 0; i < 16; i++){
    cpu.gpr[i] = ctx->gpr[i];
  }
  cpu.pc = ctx->pc;
}

void diff_memcpy(paddr_t addr, void *buf, size_t n) {
  for(size_t i = 0; i < n; i++){
     paddr_write(addr+i, 1, *((uint8_t *)buf+i));
     //printf("addr = 0x%08lx, cpy_mem = 0x%08x\n", addr+4*i, paddr_read(addr+4*i, 4));
  } 
}

__EXPORT void difftest_memcpy(paddr_t addr, void *buf, size_t n, bool direction) {
  if(direction == DIFFTEST_TO_REF){
    diff_memcpy(addr, buf, n);
  } else {
    assert(0);
  }
}

__EXPORT void difftest_regcpy(void *dut, bool direction) {
  if(direction == DIFFTEST_TO_REF) {
    diff_set_regs(dut);
  } else {
    diff_get_regs(dut);
  }
}

__EXPORT void difftest_exec(uint64_t n) {
  diff_step(n);
}

__EXPORT void difftest_raise_intr(word_t NO) {
  assert(0);
}

__EXPORT void difftest_init(int port) {
  void init_mem();
  init_mem();
  /* Perform ISA dependent initialization. */
  init_isa();
}
