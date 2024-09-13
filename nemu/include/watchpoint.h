/*************************************************************************
	> File Name: ./src/monitor/sdb/watchpoint.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年09月13日 星期五 15时12分53秒
 ************************************************************************/

#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__
typedef struct watchpoint {
  int NO;
  struct watchpoint *next;
  char expr[65536];
  uint32_t cur_value;
  uint32_t new_value;
  bool Enb;
} WP;

void init_wp_pool();
WP *new_wp(char *);
WP *get_head_point();
void free_wp(int);
#endif
