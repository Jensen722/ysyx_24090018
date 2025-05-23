#include <am.h>
#include <nemu.h>
#include <klib.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

void __am_audio_init() {
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  cfg->bufsize = inl(AUDIO_SBUF_SIZE_ADDR);
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  outl(AUDIO_FREQ_ADDR, ctrl->freq);
  outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

uint32_t am_audio_len = 0;
void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  int count = io_read(AM_AUDIO_STATUS).count;
  int bufsize = io_read(AM_AUDIO_CONFIG).bufsize;
  int len = ctl->buf.end - ctl->buf.start;

  uint8_t *buf_start = (uint8_t *)ctl->buf.start;

  //若当前流缓冲区的空闲空间少于即将写入的音频数据, 此次写入将会一直等待, 直到有足够的空闲空间
  int freespace = bufsize - count;
  while(freespace < len){
    count = io_read(AM_AUDIO_STATUS).count; //更新状态
    freespace = bufsize - count;
  }
 

  for(int i = 0; i < len; i++){
    uint32_t audio_waddr = AUDIO_SBUF_ADDR + (am_audio_len + i) % bufsize; //环形缓冲区
    outb(audio_waddr, *(buf_start + i));
  }
  am_audio_len = am_audio_len + len;
}
