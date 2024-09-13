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
#include <colour.h>

#define NR_REGEX ARRLEN(rules)
#define INT_MAX 2147473647
enum {
  TK_NOTYPE = 256, TK_EQ, TK_PLUS, TK_SUB, TK_MUL, TK_DIV, TK_LPAR, TK_RPAR, TK_DNUM, TK_HNUM, TK_REG, TK_NEQ, TK_AND, TK_DEREF,

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},        // spaces
  {"0x[0-9a-fA-F]{1,8}", TK_HNUM},        //hexadecimal number
  {"[0-9]+", TK_DNUM},      //number
  {"\\+", TK_PLUS},         //TK_PLUS
  {"==", TK_EQ},            //equal
  {"-", TK_SUB},            //sub
  {"\\*", TK_MUL},          //multiply
  {"/", TK_DIV},            //divide
  {"\\(", TK_LPAR},         //left parenthesis (
  {"\\)", TK_RPAR},         //left parenthesis )
  {"\\$..", TK_REG},        //reg_name
  {"!=", TK_NEQ},           //not equal !=
  {"&&", TK_AND},           //and &&
};

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

/*recognize token and put token into tokens[] */
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

        /* Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        tokens[nr_token].type = rules[i].token_type;
        switch (rules[i].token_type) {
          case TK_DNUM:
          case TK_HNUM:
          //case TK_REG:
             if(substr_len > 32){
                printf(L_PURPLE "warning: "NONE "the TK_SUB string is too long " L_PURPLE "[-Woverflow]\n" NONE);
                assert(0);
              } else{
                memset(tokens[nr_token].str, '\0', 32); //important!reset str! or you will get wrong str.
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
word_t isa_reg_str2val(const char *, bool *);
word_t vaddr_read(vaddr_t, int);

/*calculate the result of experssion*/
word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  for(int i = 0; i < nr_token; i++){
    if(tokens[i].type == TK_MUL && (i == 0 || is_operator(tokens[i-1].type) || tokens[i-1].type == TK_LPAR)){
      tokens[i].type = TK_DEREF;
    }
  }


  word_t result = eval(0, nr_token - 1);
  *success = true;

  return result;
}

/*calculate the result of experssion through recursion*/
word_t eval(int p, int q){
   if(p > q) {
      assert(0);
   } else if (p == q){
     bool success = false;
     if(tokens[p].type == TK_DNUM){
       return strtoul(tokens[p].str, NULL, 10);
     } else if(tokens[p].type == TK_HNUM){
          return strtoul(tokens[p].str, NULL, 16);
       }

     return isa_reg_str2val(tokens[p].str, &success);

   } else if (check_parentheses(p, q) == true) {
      // The expression is surrounded by a matched pair of parentheses.
     // If that is the case, just throw away the parentheses. //
     return eval(p + 1, q - 1);
   } else {
     int op = get_main_operator_position(p, q);
     if(tokens[op].type == TK_DEREF){
        word_t val2 = eval(op + 1, q);
        return vaddr_read(val2, 4);
     }
     word_t val1 = eval(p, op - 1);
     word_t val2 = eval(op + 1, q);

     switch(tokens[op].type){
       case TK_PLUS: return val1 + val2;
       case TK_SUB: return val1 - val2;
       case TK_MUL: return val1 * val2;
       case TK_DIV: {
        if(val2 == 0){
          printf(L_PURPLE "warning: " NONE "TK_DIVision by zero " L_PURPLE "[-Wdiv-by-zero]\n" NONE);
          return -1;
        }
        return val1 / val2;
       }
       case TK_EQ: return (val1 == val2);
       case TK_NEQ: return (val1 != val2);
       case TK_AND: return (val1 && val2);
       default: assert(0);
     }
   }
 }

/*Parentheses Matching Check*/
bool check_parentheses(int p, int q){
  int par_count = 0;
  if((tokens[p].type == TK_LPAR) && (tokens[q].type == TK_RPAR)){
     for(int i = p + 1; i <= q - 1; i++){
        if(tokens[i].type == TK_LPAR) {
          par_count++;
        } else if(tokens[i].type == TK_RPAR){
             par_count--;
             if(par_count < 0){  //to exclude situation ) ( 
                return false;
            }
          }
      } 
     if(par_count == 0) {return true;}
   }

  return false;
  
}



int get_main_operator_position(int p, int q){
  int op = p;
  int min_precedence = INT_MAX;
  int parentheses_count = 0;

  for(int i = p; i <= q; i++){
    int type = tokens[i].type; 

    if(type == TK_LPAR){
      parentheses_count++;
    } else if(type == TK_RPAR){
        parentheses_count--;
    } else if(is_operator(type) && (parentheses_count == 0)){  //only operator and operator not in parentheses(parentheses == 0) can be main operator
        int current_precedence = precedence(type);
        if(current_precedence < min_precedence){  //compare precedence of operator
          min_precedence = current_precedence;
          op = i;
        } else if (current_precedence == min_precedence && (is_operator(type) != TK_DEREF)){
            op = i;
          }
      }
  }

  return op;
}

int is_operator(int type){
  int is_operator = ((type == TK_PLUS) || (type == TK_SUB) || (type == TK_MUL) || (type == TK_DIV) || (type == TK_EQ) || (type == TK_NEQ) || (type == TK_AND) || type == TK_DEREF );
  return is_operator;
}

int precedence(int type){
  switch(type){
    case TK_AND: return 1;
    case TK_EQ: return 2;
    case TK_NEQ: return 2;
    case TK_PLUS: return 3;
    case TK_SUB:  return 3;
    case TK_MUL:  return 4;
    case TK_DIV:  return 4;
    case TK_DEREF:return 5;
    default: return INT_MAX;
   }
}

