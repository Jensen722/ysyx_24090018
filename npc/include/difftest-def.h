/*************************************************************************
	> File Name: /home/jensen/ysyx-workbench/npc/include/difftest-def.h
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2025年03月03日 星期一 10时36分08秒
 ************************************************************************/


#ifndef __DIFFTEST_DEF_H__
#define __DIFFTEST_DEF_H__

#include <stdint.h>
#include <macro.h>
#include <generated/autoconf.h>

#define __EXPORT __attribute__((visibility("default")))
enum { DIFFTEST_TO_DUT, DIFFTEST_TO_REF };

#define RISCV_GPR_TYPE MUXDEF(CONFIG_RV64, uint64_t, uint32_t)
#define RISCV_GPR_NUM  16
#define DIFFTEST_REG_SIZE (sizeof(RISCV_GPR_TYPE) * (RISCV_GPR_NUM + 1)) // GPRs + pc

#endif
