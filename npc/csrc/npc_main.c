#include <debug.h>

void cpu_exec(uint64_t n);
void sim_init();
void reset(uint64_t n);
void sim_exit();
void init_monitor(int argc, char *argv[]);
void engine_start();

int main(int argc, char *argv[]) {
  sim_init();
  init_monitor(argc, argv);
  reset(1); 


  /* Start engine. */
  engine_start();
  sim_exit();
}

