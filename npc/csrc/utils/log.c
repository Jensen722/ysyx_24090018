/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/csrc/utils/log.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年02月20日 星期四 11时46分04秒
 ************************************************************************/

#include <common.h>

FILE *log_fp = NULL;

void init_log(const char *log_file) {
  log_fp = stdout;
  if (log_file != NULL) {
    FILE *fp = fopen(log_file, "w");
    //Assert(fp, "Can not open '%s'", log_file);
    log_fp = fp;
  }
  Log("Log is written to %s", log_file ? log_file : "stdout");
}

bool log_enable(){
  return true;
}
