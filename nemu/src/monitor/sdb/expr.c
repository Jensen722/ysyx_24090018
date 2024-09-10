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

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, PLUS, SUB, MUL, DIV, L_PAR, R_PAR, NUM,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"[0-9]+", NUM},      //number
  {"\\+", PLUS},         // plus
  {"==", TK_EQ},        // equal
  {"-", SUB},           //sub
  {"\\*", MUL},         //multiply
  {"/", DIV},           //divide
  {"\\(", L_PAR},       //left parenthesis (
  {"\\)", R_PAR},       //left parenthesis )
};

#define NR_REGEX ARRLEN(rules)
#define INT_MAX 2147473647

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        if(rules[i].token_type == TK_NOTYPE) {break;}

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        tokens[nr_token].type = rules[i].token_type;
        switch (rules[i].token_type) {
          case NUM:
             if(substr_len > 32){
                printf("the sub string is too long.\n");
                assert(0);
              } else{
                for(int j = 0; j < substr_len; j++){
                  tokens[nr_token].str[j] = *(substr_start + j);
              }
              } 
        }
        nr_token += 1;

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

word_t eval(int, int);
bool check_parentheses(int, int);
int get_main_operator_position(int, int);
int is_operator(int);
int precedence(int);

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  word_t result = eval(1, nr_token-1);
  *success = true;

  return result;
}

word_t eval(int p, int q){
   if(p > q) {
      assert(0);
   } else if (p == q){
     return strtoul(tokens[p].str, NULL, 10);
   } else if (check_parentheses(p, q) == true) {
      // The expression is surrounded by a matched pair of parentheses.
     // If that is the case, just throw away the parentheses. //
     return eval(p + 1, q - 1);
   } else {
assert(p >= 0 && p < nr_token);
assert(q >= 0 && q < nr_token);
     int op = get_main_operator_position(p, q);
     word_t val1 = eval(p, op - 1);
     word_t val2 = eval(op + 1, q);

     switch(tokens[op].type){
       case PLUS: return val1 + val2;
       case SUB: return val1 - val2;
       case MUL: return val1 * val2;
       case DIV: {
        if(val2 == 0){
          printf("div 0.\n");
          return -1;
        }
        return val1 / val2;
       }
       default: assert(0);
     }
   }
 }

bool check_parentheses(int p, int q){
  int par_count = 0;
  if((tokens[p].type == L_PAR) && (tokens[q].type == R_PAR)){
     for(int i = p; i <= q; i++){
        if(tokens[i].type == L_PAR) {
          par_count++;//printf("Left parenthesis found, par_count = %d\n", par_count);
        } else if(tokens[i].type == R_PAR){
             par_count--;//printf("Left parenthesis found, par_count = %d\n", par_count);
             /*if(par_count < 0){
                printf("Error: unmatched right parenthesis at index %d\n", p + i);
                return false;
            }*/
          }  //)2 + 3( may set right
     if(par_count == 0) {return i==q;}
      } 
   }

  return false;
  
}



int get_main_operator_position(int p, int q){
  int op = p;
  int min_precedence = INT_MAX;
  int parentheses_count = 0;

  for(int i = p; i <= q; i++){
    int type = tokens[i].type; 

    if(type == L_PAR){
      parentheses_count++;
    } else if(type == R_PAR){
        parentheses_count--;
        if(parentheses_count < 0){
          printf("error parentheses.\n");
          assert(0);
        }
    } else if(is_operator(type) && (parentheses_count == 0)){
        int current_precedence = precedence(type);
        if(current_precedence < min_precedence){
          min_precedence = current_precedence;
          op = i;
        } else if (current_precedence == min_precedence){
            op = i;
          }
      }
  }
  
  //assert(op);  //test

  return op;
}

int is_operator(int type){
  int is_operator = ((type == PLUS) || (type == SUB) || (type == MUL) || (type == DIV));
  return is_operator;
}

int precedence(int type){
  switch(type){
    case PLUS: return 1;
    case SUB:  return 1;
    case MUL:  return 2;
    case DIV:  return 2;
    default: return INT_MAX;
   }
}

