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

  FILE *fp = fopen("./tools/gen-expr/build/input", "r");
  assert(fp);

  char str[65536];
  bool *success = false;
  word_t result_under_test = 0;
  uint32_t result = 1;
  for(; fgets(str, sizeof(str), fp);){
   char *e = NULL;
   for(int i = 0; str[i] != '\0'; i++){
     *(e + i) = str[i];
   }
   result_under_test = expr(e, success);
   result = eval(0, 0);
   if(success){
    if(result == result_under_test)
    {
      printf("PASS!\n");
    }
   }
  }


  fclose(fp);

  /* Start engine. */
  engine_start();

  
  return is_exit_status_bad();
}
