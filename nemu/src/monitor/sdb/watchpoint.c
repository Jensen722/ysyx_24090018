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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[200]; //change to fix bug
  uint32_t cur_value;
  uint32_t new_value;
  bool Enb;
  //Address
  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

void new_wp(char *e){
if (e == NULL) {
        printf("Error: Null expression passed to new_wp.\n");
    }//test

  if(free_ == NULL){
    printf("ERROR: no more free watchpoint!\n");
    assert(0);
  }  

  /*delete the watchpoint from wp_pool in order.*/
  WP *p = NULL;
  p = free_;
  free_ = p->next;

  /*add this watchpoint to watchpoing lists in use*/
  p->next = head;
  head = p;

  bool success = false;
  strcpy(p->expr, e);
  p->cur_value = expr(p->expr, &success);
  printf("cur_value = %u",p->cur_value);
  p->new_value = p->cur_value;
  if(!success){
    printf("ERROR: Failed to evalute expression.\n");
  }
  p->Enb = true;


  //return p;

}

WP *get_head_point(){
  return head;
}


void free_wp(int N){
  WP *wp = NULL;
  WP *pre = NULL;

  /*delete the wp  watchpoint in use*/
  for(pre = head; pre; pre = pre->next){
    if(pre->NO - 1 == N){
      wp = pre->next;
      pre->next = wp->next;
      return;
    } else {
      printf("ERROR: No this watchpoint.\n");
      assert(0);
    }

  }

  /*put wp watchpoint to wp_pool*/
  wp->next = free_;
  free_ = wp;
}


/* TODO: Implement the functionality of watchpoint */
 
/*
WP *add_wp(char *e){
  WP *wp = new_wp();
  wp->expr = e;
  wp->Enb = true;
  return wp;
}*/
