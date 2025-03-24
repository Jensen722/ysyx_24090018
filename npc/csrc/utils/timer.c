/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/csrc/utils/timer.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: Mon 17 Mar 2025 10:41:39 PM CST
 ************************************************************************/

#include <common.h>
#include <sys/time.h>

static uint64_t boot_time = 0;

static uint64_t get_time_internal() {
  struct timeval now;
  gettimeofday(&now, NULL);
  uint64_t us = now.tv_sec * 1000000 + now.tv_usec;

  return us;
}

uint64_t get_time() {
  if (boot_time == 0) boot_time = get_time_internal();
  uint64_t now = get_time_internal();
  return now - boot_time;
}

void init_rand() {
  srand(get_time_internal());
}
