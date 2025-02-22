/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/csrc/monitor/sdb/wp.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月22日 星期六 14时08分28秒
 ************************************************************************/

#include "utils.h"

#define NR_WP 32
#define MAX_LEN 64

typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[MAX_LEN]; 
  uint32_t cur_value;
  uint32_t new_value;
  bool Enb;
  //Address
  /* TODO: Add more members if necessary */

} WP;

word_t expr(char *e, bool *success);

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
    printf(ANSI_FMT("ERROR: no more free watchpoint!\n", ANSI_FG_RED));
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
    printf(ANSI_FMT("ERROR: Failed to evalute expression.\n", ANSI_FG_RED));
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
    printf(ANSI_FMT("watchpoint %d not exist!\n", ANSI_FG_MAGENTA), N);
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
    printf(ANSI_FMT("No watchpoint!\n", ANSI_FG_MAGENTA));
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
    printf(ANSI_FMT("watchpoint %d not exist!\n", ANSI_FG_MAGENTA), N);
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
        printf(ANSI_FMT("Watchpoint %d triggered: %s\n", ANSI_FG_MAGENTA), p->NO, p->expr);
        printf(ANSI_FMT("Old value  = 0x%x, New value = 0x%x\n", ANSI_FG_MAGENTA), p->cur_value, p->new_value);

        p->cur_value = p->new_value;

        return true;
      }
    }
  }

   return false;
}

