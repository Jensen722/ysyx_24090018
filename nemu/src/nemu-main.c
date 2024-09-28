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

#include <common.h>
#include <stdio.h>
#include <utils.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();
word_t eval(int, int);
word_t expr(char *, bool *);

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  init_monitor(argc, argv);
#endif

  
  /*test experssion*/ 
  FILE *fp = fopen("./tools/gen-expr/build/input", "r");
  assert(fp);

  char str[65536];
  bool success = true;
  word_t result_under_test = 0;
  uint32_t result = 1;
  for(; fgets(str, sizeof(str), fp);){
   char *res = strtok(strtok(str, "\n"), " ");
   char *e = strtok(NULL, "");

   result_under_test = expr(e, &success);
   result = expr(res, &success);
   printf("result_under_test: %u\n", result_under_test);
   printf("result: %u\n", result);

   if(success){
    if(result == result_under_test)
    {
     printf(ANSI_FMT("PASS!\n", ANSI_FG_GREEN));
    }
    else {printf(ANSI_FMT("NOT PASS!\n", ANSI_FG_RED));}
   }
  }
  fclose(fp); 

  /* Start engine. */
  engine_start();

  
  return is_exit_status_bad();
}
