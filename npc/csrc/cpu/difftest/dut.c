/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/csrc/cpu/difftest/dut.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月02日 星期日 15时00分29秒
 ************************************************************************/

/*
#include <dlfcn.h>

void (*ref_difftest_memcpy)(paddr_t addr, void *buf, size_t n, bool direction) = NULL;
void (*ref_difftest_regcpy)(void *dut, bool direction) = NULL;
void (*ref_difftest_exec)(uint64_t n) = NULL;
void (*ref_difftest_raise_intr)(uint64_t NO) = NULL;


#ifdef CONFIG_DIFFTEST

void init_difftest(char *ref_so_file, long img_size, inti port){
  assert(ref_so_file != NULL);


  //66-83line就是为了能调用ref.c中的函数
  void *handle;
  handle = dlopen(ref_so_file, RTLD_LAZY);//加载动态库，只有在函数调用的时候才解析符号，并返回句柄handle
  assert(handle);

  ref_difftest_memcpy = dlsym(handle, "difftest_memcpy");
  assert(ref_difftest_memcpy);

  ref_difftest_regcpy = dlsym(handle, "difftest_regcpy");//从动态库handle中加载名为'difftest_memcpy'的符号
  assert(ref_difftest_regcpy);

  ref_difftest_exec = dlsym(handle, "difftest_exec");
  assert(ref_difftest_exec);

  ref_difftest_raise_intr = dlsym(handle, "difftest_raise_intr");
  assert(ref_difftest_raise_intr);

  void (*ref_difftest_init)(int) = dlsym(handle, "difftest_init");
  assert(ref_difftest_init);

  Log("Differential testing: %s", ANSI_FMT("ON", ANSI_FG_GREEN));
  Log("The result of every instruction will be compared with %s. "
      "This will help you a lot for debugging, but also significantly reduce the performance. "
      "If it is not necessary, you can turn it off in menuconfig.", ref_so_file);

  ref_difftest_init(port);
  ref_difftest_memcpy(RESET_VECTOR, guest_to_host(RESET_VECTOR), img_size, DIFFTEST_TO_REF);
  ref_difftest_regcpy();
}
#endif
*/
