/*************************************************************************
	> File Name: init.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月20日 星期四 10时03分35秒
 ************************************************************************/

#include<stdio.h>

void sdb_mainloop();

void engine_start(){
  
  /* Receive commands from user. */
  sdb_mainloop();
}
