/*************************************************************************
	> File Name: sdb.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月19日 星期三 21时50分48秒
 ************************************************************************/

#include <cpu/cpu.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <memory/paddr.h>
#include <utils.h>

static int is_batch_mode = false;

void init_regex();
void init_wp_pool();
word_t expr(char *e, bool *success);
void new_wp(char *);
void free_wp(int);
void disable_wp(int);
void info_wp();
void isa_reg_display();

/*We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(npc) ");

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
  npc_state.state = NPC_QUIT;
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

//add new command: x  ->scan memory from starting address
static int cmd_x(char *args) {
  /* extract the first argument */
  char *arg1 = strtok(NULL, " ");
  /* extract the second argument */
  char *arg2 = strtok(NULL, "");
  char e[50];
  bool success = false;

  if(arg1 == NULL || arg2 == NULL){
    printf("Usage: x N expr //example: x 10 0x80000000\n");
    return 0;
  }

  uint32_t N = strtoul(arg1, NULL, 10);

  strncpy(e, arg2, 50 - 1);
  word_t result = expr(e, &success);

  printf("+-------------------------+\n");
  printf("%-11s %10s\n", "ADDR", "HEX VALUE");
  printf("+-------------------------+\n");
  for(int i = 0; i < N; i++){
    printf("0x%08x:  0x%08x\n", result, paddr_read(result, 4));
    result = result + 4;
  }
  printf("+-------------------------+\n");

  return 0;
}

static int cmd_w(char *args){
  char *expr = args;
  if(expr == NULL){
    printf(ANSI_FMT("Enter the expression to be monitored!\n", ANSI_FG_MAGENTA));
    return 0;
  }
  new_wp(expr);
  return 0;
}

//add new command: info  ->display information about register or watchpoint
static int cmd_info(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  if(arg == NULL){
    printf("Usage: info w or info r\n");
    return 0;
  }

  if(strcmp(arg, "r") == 0){
    isa_reg_display();
  } else if(strcmp(arg, "w") == 0){
    info_wp();
  } else{
   printf(ANSI_FMT("Unknown argument '%s', please input argument 'r' or 'w'\n", ANSI_FG_MAGENTA), arg);
  }

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

static int cmd_p(char *args){
  char e[50];
  if(args == NULL){
    printf(ANSI_FMT("Enter the expression to be calculate!\n", ANSI_FG_MAGENTA));
    return 0;
  }
  bool success = false;
  strncpy(e, args, 50 - 1);
  word_t result = expr(e, &success);
  printf(ANSI_FMT("%s = 0x%08X\n", ANSI_FG_BLUE), e, result);
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

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}

void init_sdb(){

  /*初始化表达式*/
  init_regex();

  /*初始化监视点池*/
  init_wp_pool();
}
