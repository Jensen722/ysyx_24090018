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
#include <readline/readline.h>
#include <readline/history.h>
#include "sdb.h"
#include <memory/vaddr.h>

#include "colour.h"

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
word_t pmem_read(paddr_t addr, int len);
word_t expr(char *, bool *);
void new_wp(char *);
void free_wp(int);
void disable_wp(int);
void info_wp();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}


static int cmd_q(char *args) {
  return -1;
}

//add new command: si  ->single-step execution
static int cmd_si(char *args) {
 uint32_t N = 0;
 /* extract the first argument */
 char *arg = strtok(NULL, " ");

 if(arg == NULL){
  N = 1;
 } else{
  N = strtoul(arg, NULL, 10);
 } 

 cpu_exec(N);

 return 0;
};


//add new command: info  ->display information about register or watchpoint
static int cmd_info(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");

  if(strcmp(arg, "r") == 0){
    isa_reg_display();
  } else if(strcmp(arg, "w") == 0){
    info_wp();
  } else{
    printf(L_PURPLE"Unknown argument '%s', please input argument 'r' or 'w'\n"NONE, arg);
  }

  return 0;
}

static int cmd_p(char *args){
  char e[50];
  if(args == NULL){
    printf(L_PURPLE"Enter the expression to be calculate!\n"NONE);
    return 0;
  }
  bool success = false;
  strncpy(e, args, 50 - 1);
  word_t result = expr(e, &success);
  printf(L_PURPLE "result = %u\n" NONE, result);
  return 0;
}

static int cmd_w(char *args){
  char *expr = args;
  if(expr == NULL){
    printf(L_PURPLE"Enter the expression to be monitored!\n"NONE);
    return 0;
  }
  new_wp(expr);
  return 0;
}

static int cmd_disable(char *args){
  char *arg = strtok(NULL, " ");
  int N = strtol(arg, NULL, 10);
  disable_wp(N);
  
  return 0;
}

static int cmd_d(char *args){
  char *arg = strtok(NULL, " ");
  int N = strtol(arg, NULL, 10);
  free_wp(N);
  return 0;
}


//add new command: x  ->scan memory from starting address
static int cmd_x(char *args) {
  /* extract the first argument */
  char *arg1 = strtok(NULL, " ");
  /* extract the second argument */
  char *arg2 = strtok(NULL, "");
  char e[50];
  bool success = false;

  if(arg1 == NULL || arg2 == NULL){
    printf("Usage: x N expr //example: x 10 0x80000000");
  }

  uint32_t N = strtoul(arg1, NULL, 10);

  strncpy(e, arg2, 50 - 1);
  word_t result = expr(e, &success);
  //vaddr_t expr = strtoul(arg2, NULL, 16);

  printf("+-------------------------+\n");
  printf("%-10s %10s\n", "ADDR", "HEX VALUE");
  printf("+-------------------------+\n");
  for(int i = 0; i < N; i++){
    printf("0x%08X:  0x%08X\n", result, vaddr_read(result, 4));
    result = result + 4;
  }
  printf("+-------------------------+\n");

  return 0;
}

static int cmd_help(char *args);

static struct {
  const char *name;
  const char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display information about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single-step execution", cmd_si},
  { "info", "Display information about registers or watchpoint", cmd_info},
  { "x", "Memory scanning", cmd_x},
  { "w", "Add watchpoint", cmd_w},
  { "d", "Delete watchpoint", cmd_d},
  { "disable", "Disable watchpoint", cmd_disable},
  { "p", "Calculate expression", cmd_p},
  /* TODO: Add more commands */

};

#define NR_CMD ARRLEN(cmd_table)

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

void sdb_set_batch_mode() {
  is_batch_mode = true;
}

void sdb_mainloop() {
  if (is_batch_mode) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef CONFIG_DEVICE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if(strcmp(cmd, "q") == 0) {
          nemu_state.state = NEMU_QUIT; } //fix the bug
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb() {
  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();
}
