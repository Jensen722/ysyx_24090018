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

#define MAX_NUM 100
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

void put_str_to_buf(const char *str, const int str_len){
  if((buf_pos + str_len) > 65535){
    printf("ERROR: buf overflow.\n");
    assert(0);
  }
  for(int i = 0; i < str_len; i++){
    buf[buf_pos] = str[i];
    buf_pos++;
  }
}

void put_char_to_buf(const char c){
  if((buf_pos + 1) > 65535){
    printf("ERROR: buf overflow.\n");
    assert(0);
  }
  buf[buf_pos] = c;
  buf_pos += 1;
}

void gen_space(){
  int space_len = rand() % 2 + 1;
  char space[3];
  for(int i = 0; i < space_len; i++){
    space[i] = ' ';
  }
  put_str_to_buf(space, space_len);
}

void gen_rand_op(){
  const char op[] = "+-*/";
  char c =  op[(rand() % 4)];
  put_char_to_buf(c);
}

void gen(const char c){
  put_char_to_buf(c);
}

void gen_num(){
  int rand_num = rand() % MAX_NUM;
  char num_str[12];
  int str_len = 0;

  snprintf(num_str, sizeof(num_str), "%d", rand_num);
  for(int i = 0; num_str[i] != '\0'; i++){
     str_len = i + 1;
  }

  put_str_to_buf(num_str, str_len);
}

void gen_rand_expr() {
  switch(choose(3)){
    case 0: gen_num();gen_space();break;
    case 1: gen('('); gen_rand_expr(); gen(')');break;
    default: gen_rand_expr(); gen_space(); gen_rand_op(); gen_space(); gen_rand_expr();break;
  }
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
    memset(buf, '\0', sizeof(buf));  //reset buf
    buf_pos = 0; //reset buf_pos
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
