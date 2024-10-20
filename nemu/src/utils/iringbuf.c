/*************************************************************************
	> File Name: src/utils/iringbuf.c
	> Author: Jensen
	> Mail: 1624839259@qq.com 
	> Created Time: 2024年10月20日 星期日 15时34分18秒
 ************************************************************************/
#include <string.h>
#include <stdio.h>
#include <cpu/decode.h>
#define BUFF_MAX_LEN 16
#define LOGBUF_SIZE 128

typedef struct{
  char ringbuf[BUFF_MAX_LEN][LOGBUF_SIZE];
  int idx;
} RingBuff;

RingBuff rb = {.idx=0};

void inst_ringbuf_record(Decode *s){
  char *p = rb.ringbuf[rb.idx];
  
  p += snprintf(p, sizeof(rb.ringbuf[rb.idx]), FMT_WORD ":", s->pc);
  int ilen = s->snpc - s->pc;
  int i;
  uint8_t *inst = (uint8_t *)&s->isa.inst.val;
  for (i = ilen - 1; i >= 0; i --) {
    p += snprintf(p, 4, " %02x", inst[i]);
  } 
  int ilen_max = MUXDEF(CONFIG_ISA_x86, 8, 4);
  int space_len = ilen_max - ilen;
  if (space_len < 0) space_len = 0;
  space_len = space_len * 3 + 1;
  memset(p, ' ', space_len);
  p += space_len; 
#ifndef CONFIG_ISA_loongarch32r
  void disassemble(char *str, int size, uint64_t pc, uint8_t *code, int nbyte);
  disassemble(p, rb.ringbuf[rb.idx] + sizeof(rb.ringbuf[rb.idx]) - p,
      MUXDEF(CONFIG_ISA_x86, s->snpc, s->pc), (uint8_t *)&s->isa.inst.val, ilen);
#else
  p[0] = '\0'; // the upstream llvm does not support loongarch32r
#endif

  rb.idx = (rb.idx + 1) % BUFF_MAX_LEN;

};

void inst_ringbuf_display(){
  int rd_idx = (rb.idx - 1 + BUFF_MAX_LEN) % BUFF_MAX_LEN;

  printf("-->%s\n", rb.ringbuf[rd_idx]);
  for(int i = 1; i < BUFF_MAX_LEN; i++){
    if(strcmp(rb.ringbuf[(rd_idx+i) % BUFF_MAX_LEN], "") != 0){
      printf("   %s\n", rb.ringbuf[(rd_idx+i) % BUFF_MAX_LEN]);
    }
  }
};
