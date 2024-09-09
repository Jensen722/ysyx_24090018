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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static int buf_pos;

static uint32_t choose(uint32_t n){
  return rand() % n;
}

/*void put_char_to_buf(const char *str){
  int str_len = sizeof(str);
  if((buf_pos + str_len) > 65535){
    printf("ERROR: buf overflow.\n");
    assert(0);
  }
  for(int i = 0; i < str_len; i++){
    buf[buf_pos] = str[i];
    buf_pos++;
  }
}*/

void gen_rand_op(){
  int char_len;
  const char op[] = "+-*/";
  char_len = 1;
  if((buf_pos + char_len) == 65535){
    printf("ERROR: buffer overflow.\n");
    assert(0);
  } 
  buf[buf_pos] =  op[(rand() % 4)];
  buf_pos += char_len;
}

void gen(const char c){
  int char_len = 1;
  
  if((buf_pos + char_len) >= 65535) {
    if(c == '('){
      printf("ERROR: buffer overflow.\n");
      assert(0);
     }
  }
  buf[buf_pos] = c;
  buf_pos += char_len;
}

#define MAX_NUM 100
void gen_num(){
  int char_len = 0;
  int rand_num = rand() % MAX_NUM;
  char num_str[12];
  snprintf(num_str, sizeof(num_str), "%d", rand_num);
  if((buf_pos + char_len) > 65535){
    printf("ERROR:buffer overflow.\n");
    assert(0);
  }
  char_len = sizeof(num_str);
  for(int i = 0; num_str[i] != '\0'; i++){
    buf[buf_pos] = num_str[i];
    buf_pos = buf_pos + 1;
  }

  
}

void gen_rand_expr() {
  switch(choose(3)){
    case 0: gen_num();break;
    case 1: gen('('); gen_rand_expr(); gen(')');break;
    default: gen_rand_expr(); gen_rand_op(); gen_rand_expr();break;
  }

  buf[65535] = '\0';
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    memset(buf, '\0', sizeof(buf));
    buf_pos = 0;
    gen_rand_expr();

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
