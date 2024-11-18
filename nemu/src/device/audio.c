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

#include <common.h>
#include <device/map.h>
#include <SDL2/SDL.h>

enum {
  reg_freq,
  reg_channels,
  reg_samples,
  reg_sbuf_size,
  reg_init,
  reg_count,
  nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;

void printf_sbuf(){
  for(int i = 0;i< CONFIG_SB_SIZE;i ++){\
    printf("sbuf[%d]: 0x%x\n", i, *(sbuf+i));
  }
}
//SDL音频回调函数
//将sbuf中的音频数据拷贝至SDL库的缓冲区
static int audio_pos;
static int audio_len = CONFIG_SB_SIZE;
static void audio_play_callback(void *userdata, uint8_t *stream, int len){
  /*if(buf_rd_pos + len > audio_base[reg_count]){
    len = audio_base[reg_count] - buf_rd_pos;
  }
  
  printf("sbuf: %x\n", *(sbuf+buf_rd_pos));
  memcpy(stream, sbuf + buf_rd_pos, len);
  buf_rd_pos += len;*/
 SDL_memset(stream, 0, len);
    if (audio_len == 0)
        return;
    len = (len > audio_len ? audio_len : len);

    //SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);
    memcpy(stream, sbuf+audio_pos, len);
    audio_pos += len;
    audio_len -= len;
}

//SDL库播放音频
void audio_play(){
SDL_AudioSpec desired = {};

desired.format = AUDIO_S16SYS;
desired.userdata = NULL;
desired.silence = 0;
desired.freq = audio_base[reg_freq];
desired.channels = audio_base[reg_channels];
desired.samples = audio_base[reg_samples];
desired.callback = audio_play_callback;

int ret = SDL_InitSubSystem(SDL_INIT_AUDIO);
if(ret == 0){
SDL_OpenAudio(&desired, NULL);
SDL_PauseAudio(0);
}
}

static void audio_io_handler(uint32_t offset, int len, bool is_write) {
  if(!is_write){
    audio_base[reg_sbuf_size] = CONFIG_SB_SIZE;
    audio_base[reg_count] = audio_pos;
  } 
}

void init_audio() {
  uint32_t space_size = sizeof(uint32_t) * nr_reg;
  audio_base = (uint32_t *)new_space(space_size);
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
  add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

  sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
  add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
}
