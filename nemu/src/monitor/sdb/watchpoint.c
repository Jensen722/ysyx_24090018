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
#include "colour.h"

#define NR_WP 32
#define MAX_LEN 64

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[MAX_LEN]; //change to fix bug
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
  if(free_ == NULL){
    printf(L_RED"ERROR: no more free watchpoint!\n"NONE);
    assert(0);
  }  

  /*delete the watchpoint from wp_pool in order.*/
  WP *p = NULL;
  p = free_;
  free_ = p->next;

  bool success = false;
  strncpy(p->expr, e, MAX_LEN - 1);
  p->cur_value = expr(p->expr, &success);
  p->new_value = p->cur_value;

  if(!success){
    printf(L_RED"ERROR: Failed to evalute expression.\n"NONE);
  }
  p->Enb = true;

  /*add this watchpoint to watchpoint lists in use*/
  p->next = head;
  head = p;

}


void free_wp(int N){
  WP *wp = NULL;
  WP *pre = NULL;

  for(WP *p = head; p; p = p->next){
    if(p->NO == N){
      wp = p;
      break;
    }
  }

  if(wp == NULL){
    printf(L_PURPLE "watchpoint %d not exist!\n"NONE, N);
    return;
  }

  /*delete the wp  watchpoint in use*/
  if(wp == head){
    head = wp->next;
  }

  for(pre = head; pre; pre = pre->next){
    if(pre->next == wp){
      pre->next = wp->next;
      break;
    } 
  }

  /*put wp watchpoint to wp_pool*/
  wp->next = free_;
  free_ = wp;
}

void info_wp(){
  if(head != NULL){
    printf("Num\t    Type\t Enb\t What\n");
    for(WP *wp = head; wp; wp = wp->next){
      printf("%2d\t watchpoint\t  %c\t %s\n", wp->NO, wp->Enb ? 'y' : 'n', wp->expr);
    }
  }
  else {
    printf(L_PURPLE "No watchpoint!\n" NONE);
  }
}

void disable_wp(int N){
  WP *wp = NULL;

  for(WP *p = head; p; p = p->next){
    if(p->NO == N){
      wp = p;
      break;
    }
  }

  if(wp == NULL){
    printf(L_PURPLE "watchpoint %d not exist!\n"NONE, N);
    return;
  }
  
  wp->Enb = false;
}

_Bool scan_wp(){
  bool success = false;

  for(WP *p = head; p; p = p->next){
   if(p->Enb){
      p->new_value = expr(p->expr, &success);
      if(p->new_value != p->cur_value){
        printf(L_PURPLE "Watchpoint %d triggered: %s\n" NONE, p->NO, p->expr);
        printf(L_PURPLE"Old value  = %u, New value = %u\n"NONE, p->cur_value, p->new_value);

        p->cur_value = p->new_value;

        return true;
      }
    }
  }

   return false;
}

